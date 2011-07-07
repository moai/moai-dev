// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICLASS_H
#define	MOAICLASS_H

//================================================================//
// MOAIClass
//================================================================//
/**	@name MOAIClass
	@text Contains utility methods for reflection.
*/
class MOAIClass :
	public USGlobalClass < MOAIClass, USLuaObject > {
private:

	//----------------------------------------------------------------//
	static int			_getClass				( lua_State* L );
	static int			_getName				( lua_State* L );
	static int			_isInstance				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIClass )

	//----------------------------------------------------------------//
						MOAIClass				();
						~MOAIClass				();
	void				RegisterLuaClass		( USLuaState& state );
};

#endif
