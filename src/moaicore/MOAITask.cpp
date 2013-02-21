// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITask.h>
#include <moaicore/MOAITaskQueue.h>

//================================================================//
// MOAITask
//================================================================//

//----------------------------------------------------------------//
MOAITask::MOAITask () :
	mPriority ( PRIORITY_HIGH ),
	mQueue ( 0 ),
	mSubscriber ( 0 ) {
	
	this->mLink.Data ( this );
}

//----------------------------------------------------------------//
MOAITask::~MOAITask () {
}

//----------------------------------------------------------------//
void MOAITask::Publish () {
}

//----------------------------------------------------------------//
void MOAITask::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "new",							MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITask::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITask::Start ( MOAITaskQueue& queue, MOAITaskSubscriber& subscriber ) {

	this->mQueue = &queue;
	this->mSubscriber = &subscriber;

	queue.PushTask ( *this );
}
