// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKTHREAD_H
#define MOAITASKTHREAD_H

#include <moai-util/MOAIThread.h>
#include <moai-util/MOAITaskQueue.h>

//================================================================//
// MOAITaskThread
//================================================================//
class MOAITaskThread :
	public MOAITaskQueue {
private:

	MOAIThread		mThread; // TODO: inherit?
	
	//----------------------------------------------------------------//
	static void		_main					( void* param, MOAIThreadState& threadState );

	//----------------------------------------------------------------//
	void			PushTask				( MOAITask& task );

public:

	friend class MOAITaskBase;

	DECL_LUA_FACTORY ( MOAITaskThread )

	//----------------------------------------------------------------//
					MOAITaskThread			();
					~MOAITaskThread			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Stop					();
};

#endif
