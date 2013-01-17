// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKQUEUE_H
#define MOAITASKQUEUE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIMutex.h>
#include <moaicore/MOAIThread.h>
#include <moaicore/MOAITask.h>

class MOAITaskSubscriber;

//================================================================//
// MOAITaskQueue
//================================================================//
class MOAITaskQueue :
	public virtual MOAILuaObject {
protected:

	friend class MOAITask;

	MOAIMutex					mMutex;
	USLeanList < MOAITask* >	mPendingTasks;

	//----------------------------------------------------------------//
	virtual void	PushTask				( MOAITask& task );
	void			Process					();

public:

	DECL_LUA_FACTORY ( MOAITaskQueue )

	//----------------------------------------------------------------//
					MOAITaskQueue			();
					~MOAITaskQueue			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
