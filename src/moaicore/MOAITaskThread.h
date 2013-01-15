// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKTHREAD_H
#define MOAITASKTHREAD_H

#include <moaicore/MOAIMutex.h>
#include <moaicore/MOAIThread.h>
#include <moaicore/MOAITask.h>

class MOAITaskSubscriber;

//================================================================//
// MOAITaskThread
//================================================================//
class MOAITaskThread {
private:

	USLeanList < MOAITaskBase* >	mPendingTasks;
	
	MOAIThread		mThread;
	MOAIMutex		mMutex;
	
	//----------------------------------------------------------------//
	static void		_main					( void* param, MOAIThreadState& threadState );

	//----------------------------------------------------------------//
	void			PushTask				( MOAITaskBase& task );
	void			Process					();

public:

	friend class MOAITaskBase;

	//----------------------------------------------------------------//
	void			Stop					();
					MOAITaskThread			();
					~MOAITaskThread			();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* NewTask ( MOAITaskSubscriber& subscriber ) {
	
		TYPE* task = new TYPE ();
		MOAITaskBase* base = task;
		base->mThread = this;
		base->mSubscriber = &subscriber;
		return task;
	}
};

#endif
