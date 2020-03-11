// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITESTMGR_H
#define	MOAITESTMGR_H

#include <moai-core/MOAIEventSource.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAITestResult
//================================================================//
class MOAITestResult {
public:

	STLString			mName;
	bool				mPassed;
	STLString			mErrorMsg;
	MOAITestResult*		mParent;

	typedef STLList < STLString >::iterator CommentsIt;
	STLList < STLString > mComments;

	typedef STLList < MOAITestResult >::iterator ChildrenIt;
	STLList < MOAITestResult > mChildren;

	//----------------------------------------------------------------//
	void*			ToJson					();
	void			Push					( lua_State* L );
};

//================================================================//
// MOAITestMgr
//================================================================//
// TODO: doxygen
class MOAITestMgr :
	public ZLContextClass < MOAITestMgr >,
	public virtual MOAILuaObject {
private:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaStrongRef	mStepFunc;

	STLString			mSuiteName;
	
	MOAITestResult*		mRoot;
	MOAITestResult*		mCurrent;

	bool				mStandalone;

	//----------------------------------------------------------------//
#ifndef _assert
	static int		_assert					( lua_State* L );
#endif
	static int		_comment				( lua_State* L );
	static int		_error					( lua_State* L );
	static int		_popTest				( lua_State* L );
	static int		_pushTest				( lua_State* L );
	static int		_results				( lua_State* L );
	static int		_setStepFunc			( lua_State* L );
	static int		_setTimeout				( lua_State* L );
	static int		_standalone				( lua_State* L );
	static int		_suite					( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Abort					();
	void			Comment					( cc8* msg );
	void			Error					( cc8* msg );
	static void		OnAlarm					( int signum );
	void			PopTest					();
	void			PushResults				( lua_State* L );
	void			PushTest				( cc8* name );
	void			SetTimeout				();
	void			SetTimeout				( float seconds );
	void			WriteLog				();

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_SINGLETON ( MOAITestMgr )

	//----------------------------------------------------------------//
					MOAITestMgr				();
					~MOAITestMgr			();
	void			Step					();
};

#endif
