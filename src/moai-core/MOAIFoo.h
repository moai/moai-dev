// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFOO_H
#define MOAIFOO_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIFoo
//================================================================//
/**	@name	MOAIFoo
	@text	Example class for extending Moai using MOAILuaObject.
			Copy this object, rename it and add your own stuff.
			Just don't forget to register it with the runtime
			using the REGISTER_LUA_CLASS macro (see moaicore.cpp).
*/
class MOAIFoo :
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_classHello			( lua_State* L );
	static int		_instanceHello		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIFoo )

	//----------------------------------------------------------------//
					MOAIFoo				();
					~MOAIFoo			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
