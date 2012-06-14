// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USTASKTHREAD_H
#define USTASKTHREAD_H

#include <uslscore/USLeanList.h>
#include <uslscore/USMutex.h>
#include <uslscore/USThread.h>
#include <uslscore/USTask.h>

//================================================================//
// USTaskThread
//================================================================//
class USTaskThread {
private:

	USLeanList < USTaskBase* >	mPendingTasks;
	USLeanList < USTaskBase* >	mCompletedTasks;
	
	USThread					mThread;
	USMutex						mMutex;
	
	//----------------------------------------------------------------//
	static void		_main					( void* param, USThreadState& threadState );

	//----------------------------------------------------------------//
	void			PushTask				( USTaskBase& task );
	void			Process					();

public:

	friend class USTaskBase;

	//----------------------------------------------------------------//
	void			Publish					();
	void			Stop					();
					USTaskThread			();
					~USTaskThread			();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* NewTask () {
	
		TYPE* task = new TYPE ();
		USTaskBase* typeCheck = task;
		typeCheck->mThread = this;
		return task;
	}
};

#endif
