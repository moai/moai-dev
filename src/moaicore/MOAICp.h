// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICP_H
#define	MOAICP_H
#if USE_CHIPMUNK

#include <chipmunk/chipmunk.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAICp
//================================================================//
/**	@name	MOAICp
	@text	Singleton for Chipmunk global configuration.
*/
class MOAICp :
	public MOAIGlobalClass < MOAICp, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int		_getBiasCoefficient			( lua_State* L );
	static int		_getCollisionSlop			( lua_State* L );
	static int		_getContactPersistence		( lua_State* L );
	static int		_setBiasCoefficient			( lua_State* L );
	static int		_setCollisionSlop			( lua_State* L );
	static int		_setContactPersistence		( lua_State* L );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_SINGLETON ( MOAICp )
	
	//----------------------------------------------------------------//
					MOAICp					();
					~MOAICp					();
	void			RegisterLuaClass		( MOAILuaState& state );
};

#endif
#endif
