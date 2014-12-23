// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRACE_H
#define	MOAITRACE_H

#include <moai-core/headers.h>

#include <sstream>
#include <string>
#include <vector>

//================================================================//
// MOAIFuncTrace
//================================================================//
class MOAIFuncTrace {
private:

	friend class MOAITrace;
	
	uint			mCount; // number of times this function was called
	
	double			mElapsedRunTime;
	
	STLString		mName;
	STLString		mNameWhat;
	STLString		mWhat;

	STLString		mSource;
	STLString		mShortSource;

	uint			mFirstLine;
	uint			mLastLine;

public:

	//----------------------------------------------------------------//
	MOAIFuncTrace () :
		mCount ( 0 ),
		mElapsedRunTime ( 0 ) {
	}
};

//================================================================//
// MOAIFuncTraceStackFrame
//================================================================//
class MOAIFuncTraceStackFrame {
public:
	
	const void*		mFunc;
	double			mLastRunAt;
};

//================================================================//
// MOAIThreadTrace
//================================================================//
class MOAIThreadTrace {
public:
	
	const void*		mThread;
	ZLLeanStack < MOAIFuncTraceStackFrame > mStack;
	
	double			mLastSleptAt;
	
	//----------------------------------------------------------------//
	MOAIThreadTrace () :
		mThread ( 0 ),
		mLastSleptAt ( 0 ) {
	}
};

//================================================================//
// MOAILuaHookListener
//================================================================//
class MOAILuaHookListener {
public:

	//----------------------------------------------------------------//
						MOAILuaHookListener		() {}
	virtual				~MOAILuaHookListener	() {}
	virtual void		OnHook					( lua_State* L, lua_Debug* ar ) = 0;
};

//================================================================//
// MOAITrace
//================================================================//
/**	@lua	MOAITrace
	@text	Internal debugging and hooking class.
*/
class MOAITrace :
	public MOAIGlobalClass < MOAITrace, MOAILuaObject > {
protected:

	typedef STLMap < const void*, MOAIFuncTrace >::iterator FuncTraceIt;
	STLMap < const void*, MOAIFuncTrace >	mFuncTrace;

	STLMap < const void*, MOAIThreadTrace >	mThreadTrace;

	bool					mIsActive;
	bool					mEnableTrace;
	
	double					mElapsedRunTime; // total elapsed time *not* doing debugger stuff
	double					mLastRunAt; // timestamp for resuming run
	
	lua_State*				mCurrentThread;
	MOAIThreadTrace*		mCurrentThreadTrace;
	
	bool					mVerbose;
	
	MOAILuaHookListener*	mListener;
	
	//----------------------------------------------------------------//
	static void			_callback				( lua_State* L, lua_Debug* ar );
	
	//----------------------------------------------------------------//
	static int			_reportTrace			( lua_State* L );
	static int			_run					( lua_State* L );
	static int			_setVerbose				( lua_State* L );
	static int			_start					( lua_State* L );

	//----------------------------------------------------------------//
	void				Callback				( lua_State* L, lua_Debug* ar );
	void				HandleTrace				( lua_State* L, lua_Debug* ar );

public:

	DECL_LUA_SINGLETON ( MOAITrace )

	GET_SET ( MOAILuaHookListener*, Listener, mListener )
	GET_SET ( bool, Verbose, mVerbose )

	//----------------------------------------------------------------//
						MOAITrace				();
	virtual				~MOAITrace				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ReportTrace				();
	void				Start					();
	void				Start					( MOAILuaHookListener* listener );
};

#endif
