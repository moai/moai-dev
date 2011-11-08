// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAIHttpTaskInfo_curl.h>
#include <moaicore/MOAIUrlMgr.h>

//================================================================//
// MOAIUrlMgrOpaque
//================================================================//
class MOAIUrlMgrOpaque {
public:

	STLMap < CURL*, MOAIHttpTask* > mHandleMap;
	CURLM*	mMultiHandle;
	bool	mMore;

	//----------------------------------------------------------------//
	MOAIUrlMgrOpaque () :
		mMultiHandle ( 0 ),
		mMore ( false ) {
		
		this->mMultiHandle = curl_multi_init ();
	}
	
	//----------------------------------------------------------------//
	~MOAIUrlMgrOpaque () {

		if ( this->mMultiHandle ) {
			curl_multi_cleanup ( this->mMultiHandle );
		}
	}
};

//================================================================//
// MOAIUrlMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIUrlMgr::AddHandle ( MOAIHttpTask& task ) {

	if ( !task.mInfo ) return;

	CURL* handle = task.mInfo->mEasyHandle;
	if ( !handle ) return;
	
	task.Retain ();
	task.LockToRefCount ();
	
	curl_multi_add_handle ( this->mOpaque->mMultiHandle, handle );
	this->mOpaque->mHandleMap [ handle ] = &task;
	this->mOpaque->mMore = true;
}

//----------------------------------------------------------------//
bool MOAIUrlMgr::More () {

	return this->mOpaque->mMore;
}

//----------------------------------------------------------------//
void MOAIUrlMgr::Process () {

	STLMap < CURL*, MOAIHttpTask* >& handleMap = this->mOpaque->mHandleMap;
	CURLM* multiHandle = this->mOpaque->mMultiHandle;

	if ( !this->mOpaque->mMore ) return;
	this->mOpaque->mMore = false;

	if ( !multiHandle ) return;
	
	// pump the multi handle
	int stillRunning;
	while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform ( multiHandle, &stillRunning ));

	int msgsInQueue;
	CURLMsg* msg;
	do {
		
		msg = curl_multi_info_read ( multiHandle, &msgsInQueue );
	
		if ( msg && ( msg->msg == CURLMSG_DONE )) {
		
			CURL* handle = msg->easy_handle;
			if ( handleMap.contains ( handle )) {
				MOAIHttpTask* task = handleMap [ handle ];
				handleMap.erase ( handle );
				
				task->Finish ();
				task->Release ();
			}
		}
	}
	while ( msg );

	// bail if nothing left running
	if ( !stillRunning ) return;
	
	this->mOpaque->mMore = true;
}

//----------------------------------------------------------------//
MOAIUrlMgr::MOAIUrlMgr () {

	this->mOpaque = new MOAIUrlMgrOpaque ();
}

//----------------------------------------------------------------//
MOAIUrlMgr::~MOAIUrlMgr () {
	
	delete this->mOpaque;
}

