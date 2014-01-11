// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWEBSOCKET_H
#define MOAIWEBSOCKET_H

#include <moai-core/MOAILua.h>

//================================================================//
// MOAIFoo
//================================================================//
/**	@name	MOAIFoo
	@text	Example class for extending Moai using MOAILuaObject.
			Copy this object, rename it and add your own stuff.
			Just don't forget to register it with the runtime
			using the REGISTER_LUA_CLASS macro (see moaicore.cpp).
*/
class MOAIWebSocket :
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_classHello			( lua_State* L );
	static int		_instanceHello		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIWebSocket )

	//----------------------------------------------------------------//
					MOAIWebSocket				();
					~MOAIWebSocket			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
