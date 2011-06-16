// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIJSONPARSER_H
#define	MOAIJSONPARSER_H

//================================================================//
// MOAIXmlParser
//================================================================//
/**	@name	MOAIJsonParser
	@text	Converts between Lua and JSON.
*/
class MOAIJsonParser :
	public USGlobalClass < MOAIJsonParser, USLuaObject > {
private:

	//----------------------------------------------------------------//
	static int			_encode					( lua_State* L );
	static int			_decode					( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIJsonParser )
	
	//----------------------------------------------------------------//
						MOAIJsonParser			();
						~MOAIJsonParser			();
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
};

#endif
