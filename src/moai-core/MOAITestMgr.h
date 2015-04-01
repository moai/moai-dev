// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITESTMGR_H
#define	MOAITESTMGR_H

#include <moai-core/MOAIEventSource.h>
#include <moai-core/MOAIGlobals.h>
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
	void			Push					( lua_State* L );
};

//================================================================//
// MOAITestMgr
//================================================================//
// TODO: doxygen
class MOAITestMgr :
	public MOAIGlobalClass < MOAITestMgr, MOAIGlobalEventSource > {
private:

	MOAILuaStrongRef	mCallFunc;
	MOAILuaStrongRef	mStagingFunc;
	MOAILuaStrongRef	mTestingFunc;

	STLString			mProjectDir;
	STLString			mStagingDir;
	STLString			mTestingDir;

	STLString			mSuiteName;
	
	MOAITestResult*		mRoot;
	MOAITestResult*		mCurrent;

	//----------------------------------------------------------------//
#ifndef _assert
	static int		_assert					( lua_State* L );
#endif
	static int		_comment				( lua_State* L );
	static int		_error					( lua_State* L );
	static int		_pop_test				( lua_State* L );
	static int		_push_test				( lua_State* L );
	static int		_results				( lua_State* L );
	static int		_setProjectDir			( lua_State* L );
	static int		_setStagingDir			( lua_State* L );
	static int		_setStagingFunc			( lua_State* L );
	static int		_setTestingDir			( lua_State* L );
	static int		_setTestingFunc			( lua_State* L );
	static int		_suite					( lua_State* L );
	
	//----------------------------------------------------------------//
	void			PushResults				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAITestMgr )

	//----------------------------------------------------------------//
					MOAITestMgr				();
					~MOAITestMgr			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
