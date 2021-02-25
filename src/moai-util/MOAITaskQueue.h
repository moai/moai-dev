// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKQUEUE_H
#define MOAITASKQUEUE_H

#include <zl-core/ZLThread.h>
#include <moai-util/MOAITask.h>

class MOAITaskSubscriber;

//================================================================//
// MOAITaskQueue
//================================================================//
class MOAITaskQueue :
	public virtual MOAILuaObject {
private:

	friend class MOAITask;

	ZLThread					mThread;
	
	ZLConditionVariable			mCondition;
	ZLLeanList < MOAITask* >	mPendingTasks;
	bool						mIsRunning;
	
	//----------------------------------------------------------------//
	static void		_main					( void* param, ZLThreadState& threadState );

	//----------------------------------------------------------------//
	void			Main					();
	void			PushTask				( MOAITask& task );
	void			Process					();

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	friend class MOAITaskBase;

	DECL_LUA_FACTORY ( MOAITaskQueue )

	//----------------------------------------------------------------//
					MOAITaskQueue			();
					~MOAITaskQueue			();
	void			Stop					();
};

#endif
