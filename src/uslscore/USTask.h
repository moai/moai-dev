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

	//----------------------------------------------------------------//
	void			Start				();
					USTaskBase			();
	virtual			~USTaskBase			();

public:

	friend class USTaskThread;

	//----------------------------------------------------------------//
					
};

//================================================================//
// USTask
//================================================================//
template < typename TYPE >
class USTask :
	public USTaskBase {
private:

	USCallback < TYPE* > mCallback;

	//----------------------------------------------------------------//
	void Publish () {
		this->mCallback.Call (( TYPE* )this );
		delete this;
	}

public:

	GET ( USCallback < TYPE* >&, Callback, mCallback )

	//----------------------------------------------------------------//
	template < typename TARGET >
	void SetDelegate ( TARGET* target, UNARY_SELECTOR_DECL ( TARGET, TYPE*, func )) {
		this->mCallback.template Set < TARGET >( target, func );
	}
};

#endif
