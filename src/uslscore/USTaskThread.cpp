// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USTask.h>
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
		USTaskBase *task = link->Data ();
		this->mMutex.Unlock ();
	
		task->Execute ();
		
		this->mMutex.Lock ();
		this->mCompletedTasks.PushBack ( *link );
		this->mMutex.Unlock ();
	}
}

//----------------------------------------------------------------//
void USTaskThread::Publish () {

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
