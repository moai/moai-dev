// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-client/MOAIWebSocketMgr.h>

//================================================================//
// MOAIUrlMgrNaCl
//================================================================//

//----------------------------------------------------------------//
void MOAIWebSocketMgr::AddHandle ( MOAIWebSocket& task ) {
	
	//task.LatchRetain ();
	this->mTasks.push_back ( &task );
}

//----------------------------------------------------------------//
void MOAIWebSocketMgr::Process () {

	for( STLList < MOAIWebSocket* >::iterator iter = this->mTasks.begin(); iter != this->mTasks.end(); ++iter ) {
	
//		if(( *iter )->IsReady () ) {
//
//			( *iter )->NaClFinish ();
//			( *iter )->mLatchRelease ();

//			STLList < MOAIHttpTaskNaCl* >::iterator old_iter = iter;
//			old_iter--;
//			this->mTasks.erase ( iter );
//			iter = old_iter;
//		}
			(*iter)->Process();
			
			//printf("Poll.\n");
	}
}

//----------------------------------------------------------------//
MOAIWebSocketMgr::MOAIWebSocketMgr () {
}

//----------------------------------------------------------------//
MOAIWebSocketMgr::~MOAIWebSocketMgr () {
}
