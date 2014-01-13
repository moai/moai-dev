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
				(*iter)->Process();
	}
}

//----------------------------------------------------------------//
MOAIWebSocketMgr::MOAIWebSocketMgr () {
}

//----------------------------------------------------------------//
MOAIWebSocketMgr::~MOAIWebSocketMgr () {
}
