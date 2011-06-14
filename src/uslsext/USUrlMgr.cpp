// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USHttpTask.h>
#include <uslsext/USHttpTask_impl.h>
#include <uslsext/USUrlMgr.h>

//================================================================//
// USUrlMgr
//================================================================//

static CURLM* sMultiHandle = 0;
static bool sMore = false;
static STLMap < CURL*, USHttpTask* > sHandleMap;

//----------------------------------------------------------------//
void USUrlMgr::AddHandle ( USHttpTask& task ) {

	if ( !task.mInfo ) return;

	CURL* handle = task.mInfo->mEasyHandle;
	if ( !handle ) return;
	
	curl_multi_add_handle ( sMultiHandle, handle );
	sHandleMap [ handle ] = &task;
	sMore = true;
}

//----------------------------------------------------------------//
bool USUrlMgr::More () {

	return sMore;
}

//----------------------------------------------------------------//
void USUrlMgr::Process () {

	this->mDataIOThread.Publish ();

	if ( !sMore ) return;
	sMore = false;

	if ( !sMultiHandle ) return;
	
	// pump the multi handle
	int stillRunning;
	while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform ( sMultiHandle, &stillRunning ));

	int msgsInQueue;
	CURLMsg* msg;
	do {
		
		msg = curl_multi_info_read ( sMultiHandle, &msgsInQueue );
	
		if ( msg && ( msg->msg == CURLMSG_DONE )) {
		
			CURL* handle = msg->easy_handle;
			if ( sHandleMap.contains ( handle )) {
				sHandleMap [ handle ]->Finish ();
				sHandleMap.erase ( handle );
			}
		}
	}
	while ( msg );

	// bail if nothing left running
	if ( !stillRunning ) return;
	
	sMore = true;
}

//----------------------------------------------------------------//
USUrlMgr::USUrlMgr () {

	sMultiHandle = curl_multi_init ();
}

//----------------------------------------------------------------//
USUrlMgr::~USUrlMgr () {

	if ( sMultiHandle ) {
		curl_multi_cleanup ( sMultiHandle );
	}
	
	sHandleMap.clear ();
}

