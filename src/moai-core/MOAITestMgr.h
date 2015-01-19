// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITESTMGR_H
#define	MOAITESTMGR_H

#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAITestMgr
//================================================================//
// TODO: doxygen
class MOAITestMgr :
	public MOAIGlobalClass < MOAITestMgr, MOAILuaObject > {
private:

	MOAILuaStrongRef	mStagingFunc;
	MOAILuaStrongRef	mTestingFunc;

	STLString	mProjectDir;
	STLString	mStagingDir;
	STLString	mTestingDir;

	//----------------------------------------------------------------//
	static int		_assert					( lua_State* L );
	static int		_beginTest				( lua_State* L );
	static int		_comment				( lua_State* L );
	static int		_endTest				( lua_State* L );
	static int		_runTests				( lua_State* L );
	static int		_setProjectDir			( lua_State* L );
	static int		_setStagingFunc			( lua_State* L );
	static int		_setStagingDir			( lua_State* L );
	static int		_setTestingFunc			( lua_State* L );
	static int		_setTestingDir			( lua_State* L );
	static int		_test					( lua_State* L );
	
	//----------------------------------------------------------------//
	void			Test					( lua_State* L, int idx );

public:

	DECL_LUA_SINGLETON ( MOAITestMgr )

	//----------------------------------------------------------------//
					MOAITestMgr				();
					~MOAITestMgr			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			RunTests				();
};

#endif
