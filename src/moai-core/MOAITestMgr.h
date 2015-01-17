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

	//----------------------------------------------------------------//
//	static int		_beginTest			( lua_State* L );
//	static int		_checkFilter		( lua_State* L );
//	static int		_comment			( lua_State* L );
//	static int		_endTest			( lua_State* L );
//	static int		_failure			( lua_State* L );
//	static int		_getTestList		( lua_State* L );
//	static int		_setFilter			( lua_State* L );
//	static int		_setStagingFunc		( lua_State* L );
//	static int		_setTestFunc		( lua_State* L );
//	static int		_staging			( lua_State* L );
//	static int		_success			( lua_State* L );

	//----------------------------------------------------------------//

public:

	DECL_LUA_SINGLETON ( MOAITestMgr )

	//----------------------------------------------------------------//
					MOAITestMgr				();
					~MOAITestMgr			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
