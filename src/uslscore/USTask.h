// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USTASK_H
#define USTASK_H

#include <uslscore/USAccessors.h>
#include <uslscore/USDelegate.h>
#include <uslscore/USMutex.h>

class USTaskThread;

//================================================================//
// USTaskBase
//================================================================//
class USTaskBase {
private:

	USTaskThread*	mThread;

	//----------------------------------------------------------------//
	virtual void	Execute				() = 0;
	virtual void	Publish				() = 0;

protected:

	u32				mPriority;

	//----------------------------------------------------------------//
	void			Start				();
					USTaskBase			();
	virtual			~USTaskBase			();

public:

	friend class USTaskThread;

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
// USTask
//================================================================//
template < typename TYPE >
class USTask :
	public USTaskBase {
private:

	USCallback < TYPE* > mPublicationCallback;

	//----------------------------------------------------------------//
	void Publish () {
		this->mPublicationCallback.Call (( TYPE* )this );
		delete this;
	}

public:

	GET ( USCallback < TYPE* >&, PublicationCallback, mPublicationCallback )

	//----------------------------------------------------------------//
	template < typename TARGET >
	void SetPublicationDelegate ( TARGET* target, UNARY_SELECTOR_DECL ( TARGET, TYPE*, func )) {
		this->mPublicationCallback.template Set < TARGET >( target, func );
	}
};

#endif
