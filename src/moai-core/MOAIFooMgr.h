// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFOOMGR_H
#define MOAIFOOMGR_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIFooMgr
//================================================================//
/**	@name	MOAIFooMgr
	@text	Example singleton for extending Moai using MOAILuaObject.
			Copy this object, rename it and add your own stuff.
			Just don't forget to register it with the runtime
			using the REGISTER_LUA_CLASS macro (see moaicore.cpp).
*/
class MOAIFooMgr :
	public MOAIGlobalClass < MOAIFooMgr, MOAILuaObject > {
private:
	
	//----------------------------------------------------------------//
	static int		_singletonHello		( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIFooMgr )

	//----------------------------------------------------------------//
					MOAIFooMgr			();
					~MOAIFooMgr			();
	void			RegisterLuaClass	( MOAILuaState& state );
};

#endif
