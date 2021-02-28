// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFOO_H
#define MOAIFOO_H

#include <moai-core/MOAILua.h>

#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

//================================================================//
// MOAIFoo
//================================================================//
/**	@lua	MOAIFoo
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

	//----------------------------------------------------------------//
	void			_RegisterLuaClass	( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs	( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIFoo )

	//----------------------------------------------------------------//
					MOAIFoo				( ZLContext& context );
					~MOAIFoo			();
};

#endif
