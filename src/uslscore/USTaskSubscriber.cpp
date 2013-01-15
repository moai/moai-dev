// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USTask.h>
#include <uslscore/USTaskSubscriber.h>
#include <uslscore/USDeviceTime.h>

//================================================================//
// USTaskSubscriber
//================================================================//

//----------------------------------------------------------------//
void USTaskSubscriber::Publish () {

	double startTime = USDeviceTime::GetTimeInSeconds ();

	// Publish all high-priority tasks
	USLeanLink < USTaskBase* >* i = this->mCompletedTasks.Head ();
	while ( i ) {

		this->mMutex.Lock ();
		USLeanLink < USTaskBase* >* link = i;
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
	// TODO: Avoid thread starvation
	USLeanLink < USTaskBase* >* l = this->mCompletedTasksLatent.Head ();
	while ( l && ( timeElapsed < mLatentPublishDuration )) {

		this->mMutex.Lock ();
		USLeanLink < USTaskBase* >* link = l;
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
void USTaskSubscriber::PushTask ( USLeanLink< USTaskBase* >& link ) {

	this->mMutex.Lock ();
	this->mCompletedTasks.PushBack ( link );
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
void USTaskSubscriber::PushTaskLatent ( USLeanLink< USTaskBase* >& link ) {

	this->mMutex.Lock ();
	this->mCompletedTasksLatent.PushBack ( link );
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
USTaskSubscriber::USTaskSubscriber () :
	mLatentPublishDuration ( 0.1 ) {
}

//----------------------------------------------------------------//
USTaskSubscriber::~USTaskSubscriber () {
}
