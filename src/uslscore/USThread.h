// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef THREAD_H
#define THREAD_H

#include <uslscore/USMutex.h>

class USThreadImpl;

//================================================================//
// USThreadState
//================================================================//
class USThreadState {
private:

	USMutex		mMutex;
	u32			mState;

public:

	enum {
		NEW,
		RUNNING,
		PAUSED,
		STOPPED,
	};

	//----------------------------------------------------------------//
	u32			GetState			();
	bool		IsPaused			();
	bool		IsRunning			();
	bool		IsStopped			();
	void		SetState			( u32 state );
				USThreadState		();
				~USThreadState		();
};

//================================================================//
// USThread
//================================================================//
class USThread {
public:

	typedef void ( *Func )( void*, USThreadState& threadState );

	//----------------------------------------------------------------//
	Func				GetMainFunc			();
	void*				GetParam			();
	USThreadState*		GetState			();
	bool				IsCurrent			() const;
	bool				IsRunning			() const;
	void				Join				();
	static void			Sleep				();
	void				Start				( Func main, void* param, u32 stackSize );
	void				Stop				();
						USThread			();
						~USThread			();

private:

	USThreadState		mThreadState;
	USThread::Func		mMain;
	void*				mParam;
	USThreadImpl*		mImpl;

	//----------------------------------------------------------------//
	void			Clear			();
					USThread		( const USThread& ) {}
	USThread&		operator =		( const USThread& ) { return *this; }
};

#endif
