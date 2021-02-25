// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASK_H
#define MOAITASK_H

class MOAITaskQueue;
class MOAITaskSubscriber;

//================================================================//
// MOAITask
//================================================================//
class MOAITask :
	public virtual ZLRefCountedObject {
protected:

	friend class MOAITaskSubscriber;
	friend class MOAITaskQueue;

	u32						mPriority;
	MOAITaskQueue*			mQueue;
	MOAITaskSubscriber*		mSubscriber;
	//MOAILuaMemberRef		mOnFinish;
	//MOAILuaStrongRef		mLatch;

	ZLLeanLink < MOAITask* >	mLink;

	//----------------------------------------------------------------//
	//static int		_setCallback		( lua_State* L );

	//----------------------------------------------------------------//
	virtual void	Execute					() = 0;
	virtual void	Publish					();

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	// IMPORTANT: Using PRIORITY_IMMEDIATE will 'publish' the task results on the
	//            task-thread rather than the sync-thread, so use this priority
	//            only if the code is fully thread safe.
	//
	// SEMANTICS:
	// PRIORITY_IMMEDIATE: Immediate publishing of the task results on task thread
	// PRIORITY_HIGH:      Results are published on the synchronization thread w/o time slicing (previous default behavior)
	// PRIORITY_LOW:       Results are published on the synchronization thread only if time is left
	enum {
		PRIORITY_IMMEDIATE,
		PRIORITY_HIGH,
		PRIORITY_LOW
	};

	GET_SET ( u32, Priority, mPriority )
	
	//----------------------------------------------------------------//
	//void			LatchRelease			();
	//void			LatchRetain				();
					MOAITask				();
	virtual			~MOAITask				();
	void			Start					( MOAITaskQueue& queue, MOAITaskSubscriber& subscriber );
};

#endif
