// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITASK_H
#define MOAITASK_H

#include <moaicore/MOAIMutex.h>

class MOAITaskThread;
class MOAITaskSubscriber;

//================================================================//
// MOAITaskBase
//================================================================//
class MOAITaskBase {
private:

	friend class MOAITaskThread;
	friend class MOAITaskSubscriber;

	MOAITaskThread*		mThread;
	MOAITaskSubscriber*	mSubscriber;

	//----------------------------------------------------------------//
	virtual void	Execute				() = 0;
	virtual void	Publish				() = 0;

protected:

	u32				mPriority;

	//----------------------------------------------------------------//
					MOAITaskBase		();
	virtual			~MOAITaskBase		();
	void			Start				();
					
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
};

//================================================================//
// MOAITask
//================================================================//
template < typename TYPE >
class MOAITask :
	public MOAITaskBase {
private:

	USCallback < TYPE* > mCompletionCallback;

	//----------------------------------------------------------------//
	void Publish () {
		this->mCompletionCallback.Call (( TYPE* )this );
		delete this;
	}

public:

	GET ( USCallback < TYPE* >&, CompletionCallback, mCompletionCallback )

	//----------------------------------------------------------------//
	template < typename TARGET >
	void SetCompletionDelegate ( TARGET* target, UNARY_SELECTOR_DECL ( TARGET, TYPE*, func )) {
		this->mCompletionCallback.template Set < TARGET >( target, func );
	}
};

#endif
