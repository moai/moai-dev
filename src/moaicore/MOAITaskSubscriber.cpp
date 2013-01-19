// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITask.h>
#include <moaicore/MOAITaskSubscriber.h>

//================================================================//
// MOAITaskSubscriber
//================================================================//

//----------------------------------------------------------------//
MOAITaskSubscriber::MOAITaskSubscriber () :
	mLatentPublishDuration ( 0.1 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITaskSubscriber::~MOAITaskSubscriber () {
}

//----------------------------------------------------------------//
void MOAITaskSubscriber::Publish () {

	double startTime = USDeviceTime::GetTimeInSeconds ();
	USLeanList < MOAITask* >::Iterator taskIt = 0;

	// Publish all high-priority tasks
	taskIt = this->mCompletedTasks.Head ();
	while ( taskIt ) {
		
		MOAITask* task = taskIt->Data ();
		taskIt = taskIt->Next ();

		this->mMutex.Lock ();
		this->mCompletedTasks.PopFront ();
		this->mMutex.Unlock ();

		task->Publish ();
		task->Release ();
	}

	double curTime = USDeviceTime::GetTimeInSeconds ();
	double timeElapsed = curTime - startTime;

	// Use the remaining time to publish lower priority tasks
	// TODO: Avoid thread starvation
	taskIt = this->mCompletedTasksLatent.Head ();
	while ( taskIt && ( timeElapsed < this->mLatentPublishDuration )) {

		MOAITask* task = taskIt->Data ();
		taskIt = taskIt->Next ();

		this->mMutex.Lock ();
		this->mCompletedTasksLatent.PopFront ();
		this->mMutex.Unlock ();

		task->Publish ();
		task->Release ();

		curTime = USDeviceTime::GetTimeInSeconds ();
		timeElapsed = curTime - startTime;
	}
}

//----------------------------------------------------------------//
void MOAITaskSubscriber::PushTask ( MOAITask& task ) {

	this->mMutex.Lock ();
	this->mCompletedTasks.PushBack ( task.mLink );
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
void MOAITaskSubscriber::PushTaskLatent ( MOAITask& task ) {

	this->mMutex.Lock ();
	this->mCompletedTasksLatent.PushBack ( task.mLink );
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
void MOAITaskSubscriber::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "new",							MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITaskSubscriber::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
