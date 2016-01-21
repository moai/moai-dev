// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAITaskQueue.h>
#include <moai-util/MOAITaskSubscriber.h>

//================================================================//
// MOAITaskQueue main
//================================================================//

//----------------------------------------------------------------//
void MOAITaskQueue::_main ( void* param, MOAIThreadState& threadState ) {
	UNUSED ( threadState );

	(( MOAITaskQueue* )param )->Main ();
}

//================================================================//
// MOAITaskQueue
//================================================================//

//----------------------------------------------------------------//
void MOAITaskQueue::Main () {

	this->mCondition.Lock ();
	
	while ( this->mIsRunning ) {
		
		this->Process ();
		
		this->mCondition.Wait ();
	}
	
	this->mCondition.Unlock ();
}

//----------------------------------------------------------------//
MOAITaskQueue::MOAITaskQueue () :
	mIsRunning ( false ) {

	RTTI_SINGLE ( MOAITaskQueue )
}

//----------------------------------------------------------------//
MOAITaskQueue::~MOAITaskQueue () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAITaskQueue::Process () {

	ZLLeanList < MOAITask* >::Iterator taskIt = this->mPendingTasks.Head ();

	while ( taskIt ) {
		
		MOAITask* task = taskIt->Data ();
		taskIt = taskIt->Next ();

		this->mPendingTasks.PopFront ();
	
		task->Execute ();

		const u32 priority = task->GetPriority ();
		switch ( priority ) {

		case MOAITask::PRIORITY_IMMEDIATE:

			task->Publish ();
			task->Release ();
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

	bool start = false;

	task.Retain ();

	this->mCondition.Lock ();
	
	this->mPendingTasks.PushBack ( task.mLink );
	
	if ( !this->mIsRunning ) {
		this->mIsRunning = true;
		start = true;
	}
	
	this->mCondition.Signal ();
	this->mCondition.Unlock ();
	
	if ( start ) {
		this->mThread.Start ( _main, this, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITaskQueue::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITaskQueue::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITaskQueue::Stop () {

	bool stop = false;

	this->mCondition.Lock ();
	if ( this->mIsRunning ) {
		this->mIsRunning = false;
		stop = true;
		this->mCondition.Signal ();
	}
	this->mCondition.Unlock ();

	if ( stop ) {
		this->mThread.Join ();
	}
}
