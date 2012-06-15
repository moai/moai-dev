// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIHttpTaskCurl.h>
#include <moaicore/MOAIUrlMgrCurl.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef USE_CURL

//================================================================//
// MOAIUrlMgrCurl
//================================================================//

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::AddHandle ( MOAIHttpTaskCurl& task ) {

	CURL* handle = task.mEasyHandle;
	if ( !handle ) return;
	
	task.Retain ();
	task.LockToRefCount ();
	
	curl_multi_add_handle ( this->mMultiHandle, handle );
	this->mHandleMap [ handle ] = &task;
	this->mMore = true;
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::MOAIUrlMgrCurl () :
	mMultiHandle ( 0 ),
	mMore ( false ) {
		
	this->mMultiHandle = curl_multi_init ();
}

//----------------------------------------------------------------//
MOAIUrlMgrCurl::~MOAIUrlMgrCurl () {
	
	if ( this->mMultiHandle ) {
		curl_multi_cleanup ( this->mMultiHandle );
	}
}

//----------------------------------------------------------------//
void MOAIUrlMgrCurl::Process () {

	STLMap < CURL*, MOAIHttpTaskCurl* >& handleMap = this->mHandleMap;
	CURLM* multiHandle = this->mMultiHandle;

	if ( !this->mMore ) return;
	this->mMore = false;

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
				MOAIHttpTaskCurl* task = handleMap [ handle ];
				handleMap.erase ( handle );
				
				task->CurlFinish ();
				task->Release ();
			}
		}
	}
	while ( msg );

	// bail if nothing left running
	if ( !stillRunning ) return;
	
	this->mMore = true;
}

#endif