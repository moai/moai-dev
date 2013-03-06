// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASKSUBSCRIBER_H
#define MOAITASKSUBSCRIBER_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAIMutex.h>

class MOAITask;

//================================================================//
// MOAITaskSubscriber
//================================================================//
class MOAITaskSubscriber :
	public virtual MOAILuaObject {
private:

	friend class MOAITask;
	friend class MOAITaskQueue;

	USLeanList < MOAITask* >	mCompletedTasks;
	USLeanList < MOAITask* >	mCompletedTasksLatent;
	
	MOAIMutex					mMutex;
	double						mLatentPublishDuration;

	//----------------------------------------------------------------//
	void			Process					();
	void			PushTask				( MOAITask& task );
	void			PushTaskLatent			( MOAITask& task );

public:

	GET_SET ( double, LatentPublishDuration, mLatentPublishDuration )

	DECL_LUA_FACTORY ( MOAITaskSubscriber )

	//----------------------------------------------------------------//
	void			Publish					();
					MOAITaskSubscriber		();
					~MOAITaskSubscriber		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
