// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLTHREAD_H
#define ZLTHREAD_H

#include <zl-core/ZLMutex.h>

class ZLThreadImpl;
class ZLThreadLocalImpl;

//================================================================//
// ZLThreadState
//================================================================//
class ZLThreadState {
private:

	ZLMutex			mMutex;
	u32				mState;

public:

	enum {
		NEW,
		RUNNING,
		PAUSED,
		STOPPED,
	};

	//----------------------------------------------------------------//
	u32			GetState				();
	bool		IsPaused				();
	bool		IsRunning				();
	bool		IsStopped				();
				ZLThreadState			();
				~ZLThreadState			();
	void		SetState				( u32 state );
};

//================================================================//
// ZLThread
//================================================================//
class ZLThread {
public:
	
	typedef void ( *Func )( void*, ZLThreadState& threadState );

	static ZLThreadLocalImpl	gThreadLocalStorage;

private:

	ZLThreadState				mThreadState;
	ZLThread::Func				mMain;
	void*						mParam;
	ZLThreadImpl*				mImpl;

//	std::string					mName;

	//----------------------------------------------------------------//
	void					Clear				();
							ZLThread			( const ZLThread& ) {}
	
	//----------------------------------------------------------------//
	ZLThread& operator = ( const ZLThread& ) {
		return *this;
	}

public:

//	GET_SET ( std::string, Name, mName )

	//----------------------------------------------------------------//
	Func					GetMainFunc			();
	static ZLThread*		GetCurrentThread	();
	void*					GetParam			();
	ZLThreadState*			GetState			();
	bool					IsCurrent			() const;
	bool					IsRunning			() const;
	void					Join				();
							ZLThread			();
							~ZLThread			();
	static void				Sleep				();
	void					Start				( Func main, void* param, u32 stackSize );
	void					Stop				();
};

#endif
