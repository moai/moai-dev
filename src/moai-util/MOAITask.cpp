// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAITask.h>
#include <moai-util/MOAITaskQueue.h>

//================================================================//
// MOAITask
//================================================================//

//----------------------------------------------------------------//
void MOAITask::LatchRelease () {

	this->mLatch.Clear ();
}

//----------------------------------------------------------------//
void MOAITask::LatchRetain () {

	assert ( !this->mLatch );
	this->GetRef ( this->mLatch );
}

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
