// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USTask.h>
#include <uslscore/USTaskThread.h>
#include <uslscore/USDeviceTime.h>

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
		USTaskBase *task = link->Data ();
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

			this->mMutex.Lock ();
			this->mCompletedTasks.PushBack ( *link );
			this->mMutex.Unlock ();
			break;

		case USTaskBase::PRIORITY_LOW:

			this->mMutex.Lock ();
			this->mCompletedTasksLatent.PushBack ( *link );
			this->mMutex.Unlock ();
			break;
		}
	}
}

//----------------------------------------------------------------//
void USTaskThread::Publish () {

	double startTime = USDeviceTime::GetTimeInSeconds ();

	// Publish all high-priority tasks
	USLeanLink < USTaskBase* >* i = this->mCompletedTasks.Head ();
	while ( i ) {

		this->mMutex.Lock ();
		USLeanLink< USTaskBase* >* link = i;
		USTaskBase *task = link->Data ();
		i = i->Next ();
		this->mCompletedTasks.PopFront ();
		this->mMutex.Unlock ();

		task->Publish ();

		delete link;
	}

	double curTime = USDeviceTime::GetTimeInSeconds ();
	double timeElapsed = curTime - startTime;

	// Use the remaining time to publish lower priority tasks
	// ToDo: Avoid thread starvation
	USLeanLink < USTaskBase* >* l = this->mCompletedTasksLatent.Head ();
	while ( l && (timeElapsed < mLatentPublishDuration) ) {

		this->mMutex.Lock ();
		USLeanLink< USTaskBase* >* link = l;
		USTaskBase *task = link->Data ();
		l = l->Next ();
		this->mCompletedTasksLatent.PopFront ();
		this->mMutex.Unlock ();

		task->Publish ();

		delete link;

		curTime = USDeviceTime::GetTimeInSeconds ();
		timeElapsed = curTime - startTime;
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
USTaskThread::USTaskThread () :
	mLatentPublishDuration ( 0.1 ) {
}

//----------------------------------------------------------------//
USTaskThread::~USTaskThread () {

	this->Stop ();
}
