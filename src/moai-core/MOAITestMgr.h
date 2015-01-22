// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITESTMGR_H
#define	MOAITESTMGR_H

#include <moai-core/MOAIEventSource.h>
#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAITestResults
//================================================================//
class MOAITestResults {
public:

	typedef STLList < STLString >::iterator CommentsIt;
	STLList < STLString > mComments;

	STLString		mTestName;
	bool			mPassed;
	STLString		mErrorMsg;
	
	//----------------------------------------------------------------//
	void			Push		( lua_State* L );
};

//================================================================//
// MOAITestMgr
//================================================================//
// TODO: doxygen
class MOAITestMgr :
	public MOAIGlobalClass < MOAITestMgr, MOAIGlobalEventSource > {
private:

	MOAILuaStrongRef	mStagingFunc;
	MOAILuaStrongRef	mTestingFunc;

	STLString	mProjectDir;
	STLString	mStagingDir;
	STLString	mTestingDir;

	typedef STLList < MOAITestResults >::iterator TestResultsListIt;
	STLList < MOAITestResults > mTestResultsList;

	STLString			mTestSuite;
	MOAITestResults*	mCurrentTestResults;

	//----------------------------------------------------------------//
	static int		_assert					( lua_State* L );
	static int		_comment				( lua_State* L );
	static int		_handleError			( lua_State* L );
	static int		_results				( lua_State* L );
	static int		_runTests				( lua_State* L );
	static int		_setProjectDir			( lua_State* L );
	static int		_setStagingDir			( lua_State* L );
	static int		_setStagingFunc			( lua_State* L );
	static int		_setTestingDir			( lua_State* L );
	static int		_setTestingFunc			( lua_State* L );
	static int		_test					( lua_State* L );
	
	//----------------------------------------------------------------//
	void			HandleAssertFailed		( cc8* message );
	void			HandleBeginTest			( cc8* name );
	void			HandleComment			( cc8* comment );
	void			HandleEndTest			();
	void			PushResultsList			( lua_State* L );
	void			RunTests				();

public:

	DECL_LUA_SINGLETON ( MOAITestMgr )

	//----------------------------------------------------------------//
					MOAITestMgr				();
					~MOAITestMgr			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
