// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAITask.h>
#include <moai-util/MOAITaskSubscriber.h>
#include <moai-util/MOAITaskQueue.h>

//================================================================//
// MOAITaskQueue
//================================================================//

//----------------------------------------------------------------//
MOAITaskQueue::MOAITaskQueue () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITaskQueue::~MOAITaskQueue () {
}

//----------------------------------------------------------------//
void MOAITaskQueue::Process () {

	ZLLeanList < MOAITask* >::Iterator taskIt = this->mPendingTasks.Head ();

	while ( taskIt ) {
		
		MOAITask* task = taskIt->Data ();
		taskIt = taskIt->Next ();

		this->mMutex.Lock ();
		this->mPendingTasks.PopFront ();
		this->mMutex.Unlock ();
	
		task->Execute ();

		const u32 priority = task->GetPriority ();
		switch ( priority ) {

		case MOAITask::PRIORITY_IMMEDIATE:

			task->Publish ();
			task->LatchRelease ();
			break;

		default:
		case MOAITask::PRIORITY_HIGH:
			
			task->mSubscriber->PushTask ( *task );
			break;

		case MOAITask::PRIORITY_LOW:

			task->mSubscriber->PushTaskLatent ( *task );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAITaskQueue::PushTask ( MOAITask& task ) {

	task.LatchRetain ();

	this->mMutex.Lock ();
	this->mPendingTasks.PushBack ( task.mLink );
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
void MOAITaskQueue::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITaskQueue::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

