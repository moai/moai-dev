// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-http-client/MOAIHttpTaskCurl.h>
#include <moai-http-client/MOAIUrlMgrCurl.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBCURL

//================================================================//
// MOAIUrlMgrCurl
//================================================================//

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::AddHandle ( MOAIHttpTaskCurl& task ) {

	MOAIAutoLock autolock( this->mLock );

	CURL* handle = task.mEasyHandle;
	if ( !handle ) return;
	
	task.LatchRetain ();
	
	curl_multi_add_handle ( this->mMultiHandle, handle );
	this->mHandleMap [ handle ] = &task;
	this->mMore = true;
	
	this->Start ();
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::RemoveHandle ( MOAIHttpTaskCurl& task ) {
	
	{
		MOAIAutoLock autolock( this->mLock );

		CURL* handle = task.mEasyHandle;
		if ( !handle ) return;
		
		if ( this->mHandleMap.contains ( handle )) {
			
			curl_multi_remove_handle ( this->mMultiHandle, handle );
			
			this->mHandleMap.erase ( handle );
			task.LatchRelease ();
		}
	}
	
	StopIfDone();
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::MOAIUrlMgrCurl () :
	mMultiHandle ( 0 ),
	mMore ( false ),
	mThread ( NULL ) {
		
	this->mMultiHandle = curl_multi_init ();
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::~MOAIUrlMgrCurl () {
	
	this->StopIfDone ( true );
	
	if ( this->mMultiHandle ) {
		curl_multi_cleanup ( this->mMultiHandle );
	}
}

//----------------------------------------------------------------//
static void Loop( void* param, MOAIThreadState& threadState ) {
	
	MOAIUrlMgrCurl *manager = (MOAIUrlMgrCurl*)param;
	
	while (threadState.GetState() == MOAIThreadState::RUNNING) {
		
		manager->ProcessAsync();
	}
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::ProcessAsync () {
	
	struct timeval select_timeout;
	fd_set readfds, writefds, excfds;
	int max_fd = -1;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&excfds);
	
	{
		MOAIAutoLock autolock( this->mLock );
		
		long int curl_timeout;
		
		// process multi handle
		this->ProcessHandle();
		
		// ask how much we should wait
		CURLMcode mode = curl_multi_timeout( this->mMultiHandle, &curl_timeout );
		if ( mode == CURLM_CALL_MULTI_PERFORM ) return;
			
		if ( curl_timeout == 0 ) {
			return;
		} else if ( curl_timeout == -1 ) {
			select_timeout.tv_sec  = 0;
			select_timeout.tv_usec = 10000;
		} else {
			select_timeout.tv_sec  =  curl_timeout / 1000;
			select_timeout.tv_usec = (curl_timeout % 1000) * 1000;
		}
		
		// ask curl for set to wait on
		curl_multi_fdset( this->mMultiHandle, &readfds, &writefds, &excfds, &max_fd );
	}
	
	// only wait if last curl_multi_perform said nothing was pending
	if ( !this->mMore ) {
		
		select( max_fd+1, &readfds, &writefds, &excfds, &select_timeout );
	}
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::Start () {
	
	if ( !this->mThread ) {
		this->mThread = new MOAIThread();
		this->mThread->Start ( Loop, this, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::StopIfDone ( bool force ) {
	
	this->mLock.Lock();
	
	if ( ( this->mHandleMap.empty () || force ) && this->mThread ) {
		
		MOAIThread *thread = this->mThread;
		thread->Stop();
		
		// release lock before joining
		// in case thread loop is also waiting on it
		this->mThread = NULL;
		this->mLock.Unlock();
		
		thread->Join();
		delete thread;
		
		return;
	}
	
	this->mLock.Unlock();
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::ProcessHandle () {

	CURLM* multiHandle = this->mMultiHandle;

	if ( !multiHandle ) return;
	
	// pump the multi handle
	int stillRunning;
	while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform ( multiHandle, &stillRunning ));

	this->mMore = stillRunning == 1 ? true : false;
}

//----------------------------------------------------------------//
bool MOAIUrlMgrCurl::Process () {
	
	StopIfDone ();
	
	if ( this->mHandleMap.empty ()) return false;
	
	STLMap < CURL*, MOAIHttpTaskCurl* >& handleMap = this->mHandleMap;
	CURLM* multiHandle = this->mMultiHandle;

	int msgsInQueue;
	CURLMsg* msg;
	do {
		
		msg = curl_multi_info_read ( multiHandle, &msgsInQueue );
	
		if ( msg && ( msg->msg == CURLMSG_DONE )) {
			
//			MOAIAutoLock autolock( this->mLock );
			this->mLock.Lock ();
			
			CURL* handle = msg->easy_handle;
			if ( handleMap.contains ( handle )) {
				
				MOAIHttpTaskCurl* task = handleMap [ handle ];
				
				curl_multi_remove_handle ( this->mMultiHandle, handle );
				handleMap.erase ( handle );
				
				// lock only the STLMap?
				this->mLock.Unlock ();
				
				task->CurlFinish ();
				task->LatchRelease ();
			}
			else {
				this->mLock.Unlock ();
			}
		}
	}
	while ( msg );

	return true;
}

#endif