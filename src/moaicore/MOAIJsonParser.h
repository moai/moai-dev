// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIJSONPARSER_H
#define	MOAIJSONPARSER_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIJsonParser
//================================================================//
/**	@name	MOAIJsonParser
	@text	Converts between Lua and JSON.
*/
class MOAIJsonParser :
	public MOAIGlobalClass < MOAIJsonParser, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int			_decode					( lua_State* L );
	static int			_encode					( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIJsonParser )
	
	//----------------------------------------------------------------//
						MOAIJsonParser			();
						~MOAIJsonParser			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
