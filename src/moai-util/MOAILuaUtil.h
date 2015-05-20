// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAUTIL_H
#define	MOAIJSONPARSER_H

//================================================================//
// MOAILuaUtil
//================================================================//
// TODO: doxygen
class MOAILuaUtil :
	public MOAIGlobalClass < MOAILuaUtil, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int			_convert				( lua_State* L );
	static int			_getHeader				( lua_State* L );
	

public:
	
	DECL_LUA_SINGLETON ( MOAILuaUtil )
	
	//----------------------------------------------------------------//
						MOAILuaUtil				();
						~MOAILuaUtil			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif