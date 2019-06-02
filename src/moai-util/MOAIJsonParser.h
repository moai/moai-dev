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
	public ZLContextClass < MOAIJsonParser >,
	public virtual MOAILuaObject {
private:

	//----------------------------------------------------------------//
	static int			_decode					( lua_State* L );
	static int			_encode					( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIJsonParser )
	
	//----------------------------------------------------------------//
						MOAIJsonParser			();
						~MOAIJsonParser			();
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
};

#endif
#endif

