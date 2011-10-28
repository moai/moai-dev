// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USHttpTask.h>
#include <uslsext/USHttpTask_impl.h>
#include <uslsext/USUrlMgr.h>

//================================================================//
// USUrlMgrOpaque
//================================================================//
class USUrlMgrOpaque {
public:

	STLMap < CURL*, USHttpTask* > mHandleMap;
	CURLM*	mMultiHandle;
	bool	mMore;

	//----------------------------------------------------------------//
	USUrlMgrOpaque () :
		mMultiHandle ( 0 ),
		mMore ( false ) {
		
		this->mMultiHandle = curl_multi_init ();
	}
	
	//----------------------------------------------------------------//
	~USUrlMgrOpaque () {

		if ( this->mMultiHandle ) {
			curl_multi_cleanup ( this->mMultiHandle );
		}
	}
};

//================================================================//
// USUrlMgr
//================================================================//



//----------------------------------------------------------------//
void USUrlMgr::AddHandle ( USHttpTask& task ) {

	if ( !task.mInfo ) return;

	CURL* handle = task.mInfo->mEasyHandle;
	if ( !handle ) return;
	
	curl_multi_add_handle ( this->mOpaque->mMultiHandle, handle );
	this->mOpaque->mHandleMap [ handle ] = &task;
	this->mOpaque->mMore = true;
}

//----------------------------------------------------------------//
bool USUrlMgr::More () {

	return this->mOpaque->mMore;
}

//----------------------------------------------------------------//
void USUrlMgr::Process () {

	STLMap < CURL*, USHttpTask* >& handleMap = this->mOpaque->mHandleMap;
	CURLM* multiHandle = this->mOpaque->mMultiHandle;

	this->mDataIOThread.Publish ();

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
				handleMap [ handle ]->Finish ();
				handleMap.erase ( handle );
			}
		}
	}
	while ( msg );

	// bail if nothing left running
	if ( !stillRunning ) return;
	
	this->mOpaque->mMore = true;
}

//----------------------------------------------------------------//
USUrlMgr::USUrlMgr () {

	this->mOpaque = new USUrlMgrOpaque ();
}

//----------------------------------------------------------------//
USUrlMgr::~USUrlMgr () {
	
	delete this->mOpaque;
}

