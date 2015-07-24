// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKQUEUE_H
#define MOAITASKQUEUE_H

#include <moai-util/MOAIConditionVariable.h>
#include <moai-util/MOAIThread.h>
#include <moai-util/MOAITask.h>

class MOAITaskSubscriber;

//================================================================//
// MOAITaskQueue
//================================================================//
class MOAITaskQueue :
	public virtual MOAILuaObject {
private:

	friend class MOAITask;

	MOAIThread		mThread; // TODO: inherit?
	
	MOAIConditionVariable		mCondition;
	ZLLeanList < MOAITask* >	mPendingTasks;
	bool						mIsRunning;
	
	//----------------------------------------------------------------//
	static void		_main					( void* param, MOAIThreadState& threadState );

	//----------------------------------------------------------------//
	void			Main					();
	void			PushTask				( MOAITask& task );
	void			Process					();

public:

	friend class MOAITaskBase;

	DECL_LUA_FACTORY ( MOAITaskQueue )

	//----------------------------------------------------------------//
					MOAITaskQueue			();
					~MOAITaskQueue			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Stop					();
};

#endif
