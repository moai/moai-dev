// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USDeviceTime.h>
#include <uslscore/USTask.h>
#include <uslscore/USTaskSubscriber.h>
#include <uslscore/USTaskThread.h>

//================================================================//
// USTaskThread main
//================================================================//

//----------------------------------------------------------------//
void USTaskThread::_main ( void* param, USThreadState& threadState ) {

	USTaskThread* taskThread = ( USTaskThread* )param;
	
	while ( threadState.IsRunning ()) {
		taskThread->Process ();
		USThread::Sleep ();
	}
}

//================================================================//
// USTaskThread
//================================================================//

//----------------------------------------------------------------//
void USTaskThread::Process () {

	USLeanLink< USTaskBase* >* i = this->mPendingTasks.Head ();

	while ( i ) {

		this->mMutex.Lock ();
		USLeanLink< USTaskBase* >* link = i;
		i = i->Next ();
		this->mPendingTasks.PopFront ();
		USTaskBase* task = link->Data ();
		this->mMutex.Unlock ();
	
		task->Execute ();

		const u32 priority = task->GetPriority ();
		switch ( priority ) {

		case USTaskBase::PRIORITY_IMMEDIATE:

			task->Publish ();
			delete link;
			break;

		default:
		case USTaskBase::PRIORITY_HIGH:
			
			task->mSubscriber->PushTask ( *link );
			break;

		case USTaskBase::PRIORITY_LOW:

			task->mSubscriber->PushTaskLatent ( *link );
			break;
		}
	}
}

//----------------------------------------------------------------//
void USTaskThread::PushTask ( USTaskBase& task ) {

	this->mMutex.Lock ();
	this->mPendingTasks.PushBack ( *new USLeanLink < USTaskBase * >( &task ));
	this->mMutex.Unlock ();
	
	this->mThread.Start ( _main, this, 0 );
}

//----------------------------------------------------------------//
void USTaskThread::Stop () {

	this->mThread.Stop ();
	this->mThread.Join ();
}

//----------------------------------------------------------------//
USTaskThread::USTaskThread () {
}

//----------------------------------------------------------------//
USTaskThread::~USTaskThread () {

	this->Stop ();
}
