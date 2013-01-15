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
}

//----------------------------------------------------------------//
MOAITaskSubscriber::~MOAITaskSubscriber () {
}

//----------------------------------------------------------------//
void MOAITaskSubscriber::Publish () {

	double startTime = USDeviceTime::GetTimeInSeconds ();

	// Publish all high-priority tasks
	USLeanLink < MOAITaskBase* >* i = this->mCompletedTasks.Head ();
	while ( i ) {

		this->mMutex.Lock ();
		USLeanLink < MOAITaskBase* >* link = i;
		MOAITaskBase *task = link->Data ();
		i = i->Next ();
		this->mCompletedTasks.PopFront ();
		this->mMutex.Unlock ();

		task->Publish ();

		delete link;
	}

	double curTime = USDeviceTime::GetTimeInSeconds ();
	double timeElapsed = curTime - startTime;

	// Use the remaining time to publish lower priority tasks
	// TODO: Avoid thread starvation
	USLeanLink < MOAITaskBase* >* l = this->mCompletedTasksLatent.Head ();
	while ( l && ( timeElapsed < mLatentPublishDuration )) {

		this->mMutex.Lock ();
		USLeanLink < MOAITaskBase* >* link = l;
		MOAITaskBase *task = link->Data ();
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
void MOAITaskSubscriber::PushTask ( USLeanLink< MOAITaskBase* >& link ) {

	this->mMutex.Lock ();
	this->mCompletedTasks.PushBack ( link );
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
void MOAITaskSubscriber::PushTaskLatent ( USLeanLink< MOAITaskBase* >& link ) {

	this->mMutex.Lock ();
	this->mCompletedTasksLatent.PushBack ( link );
	this->mMutex.Unlock ();
}
