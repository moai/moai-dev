// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIJSONPARSER_H
#define	MOAIJSONPARSER_H
#if MOAI_WITH_JANSSON

//================================================================//
// MOAIJsonParser
//================================================================//
/**	@lua	MOAIJsonParser
	@text	Converts between Lua and JSON.
*/
class MOAIJsonParser :
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:

	//----------------------------------------------------------------//
	static int			_decode					( lua_State* L );
	static int			_encode					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAIJsonParser )
	
	//----------------------------------------------------------------//
						MOAIJsonParser			();
						~MOAIJsonParser			();
};

#endif
#endif

