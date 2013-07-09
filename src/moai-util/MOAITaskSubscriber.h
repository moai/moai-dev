// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKSUBSCRIBER_H
#define MOAITASKSUBSCRIBER_H

#include <moai-util/MOAIMutex.h>

class MOAITask;

//================================================================//
// MOAITaskSubscriber
//================================================================//
// TODO: Roll this into MOAIThread. There should really be one subscriber
// per thread. Tasks should automatically be published back to the thread
// that started them, unless explicitely overridden.
//
// Of course, the above assumes a model where multiple threads can run Lua.
// For now only the main thread can run Lua, so there really is only one
// task subscriber - the Lua thread.
//
// For now we'll make a singleton to hold the main thread's subscriber.
// This used to be in the Sim, but now with the refactor the Sim is in
// another module. We *could* move all the task/thread stuff to the Sim's
// module, but if we support threading more generally, we'll just have
// to undo that work. We'd also have to subsclass each object that supports
// async into a normal 'sync' version and a derived 'asynch' version - yuck!
class MOAITaskSubscriber {
private:

	friend class MOAITask;
	friend class MOAITaskQueue;

	ZLLeanList < MOAITask* >	mCompletedTasks;
	ZLLeanList < MOAITask* >	mCompletedTasksLatent;
	
	MOAIMutex					mMutex;
	double						mLatentPublishDuration;

	//----------------------------------------------------------------//
	void			Process					();
	void			PushTask				( MOAITask& task );
	void			PushTaskLatent			( MOAITask& task );

public:

	GET_SET ( double, LatentPublishDuration, mLatentPublishDuration )

	//----------------------------------------------------------------//
					MOAITaskSubscriber		();
					~MOAITaskSubscriber		();
	void			Publish					();
};

//================================================================//
// MOAIMainThreadTaskSubscriber
//================================================================//
// This is a temporary measure until everything runs in a thread and each
// thread has its own task subscriber.
class MOAIMainThreadTaskSubscriber :
	public MOAITaskSubscriber,
	public MOAIGlobalClass < MOAIMainThreadTaskSubscriber > {
}; 

#endif
