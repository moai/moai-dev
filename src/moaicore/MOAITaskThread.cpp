// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITask.h>
#include <moaicore/MOAITaskSubscriber.h>
#include <moaicore/MOAITaskThread.h>

//================================================================//
// MOAITaskThread main
//================================================================//

//----------------------------------------------------------------//
void MOAITaskThread::_main ( void* param, MOAIThreadState& threadState ) {

	MOAITaskThread* taskThread = ( MOAITaskThread* )param;
	
	while ( threadState.IsRunning ()) {
		taskThread->Process ();
		MOAIThread::Sleep ();
	}
}

//================================================================//
// MOAITaskThread
//================================================================//

//----------------------------------------------------------------//
MOAITaskThread::MOAITaskThread () {
}

//----------------------------------------------------------------//
MOAITaskThread::~MOAITaskThread () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAITaskThread::Process () {

	USLeanLink< MOAITaskBase* >* i = this->mPendingTasks.Head ();

	while ( i ) {

		this->mMutex.Lock ();
		USLeanLink< MOAITaskBase* >* link = i;
		i = i->Next ();
		this->mPendingTasks.PopFront ();
		MOAITaskBase* task = link->Data ();
		this->mMutex.Unlock ();
	
		task->Execute ();

		const u32 priority = task->GetPriority ();
		switch ( priority ) {

		case MOAITaskBase::PRIORITY_IMMEDIATE:

			task->Publish ();
			delete link;
			break;

		default:
		case MOAITaskBase::PRIORITY_HIGH:
			
			task->mSubscriber->PushTask ( *link );
			break;

		case MOAITaskBase::PRIORITY_LOW:

			task->mSubscriber->PushTaskLatent ( *link );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAITaskThread::PushTask ( MOAITaskBase& task ) {

	this->mMutex.Lock ();
	this->mPendingTasks.PushBack ( *new USLeanLink < MOAITaskBase * >( &task ));
	this->mMutex.Unlock ();
	
	this->mThread.Start ( _main, this, 0 );
}

//----------------------------------------------------------------//
void MOAITaskThread::Stop () {

	this->mThread.Stop ();
	this->mThread.Join ();
}
