// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFOOMGR_H
#define MOAIFOOMGR_H

#include <moai-core/MOAILua.h>

#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

//================================================================//
// MOAIFooMgr
//================================================================//
/**	@lua	MOAIFooMgr
	@text	Example singleton for extending Moai using MOAILuaObject.
			Copy this object, rename it and add your own stuff.
			Just don't forget to register it with the runtime
			using the REGISTER_LUA_CLASS macro (see moaicore.cpp).
*/
class MOAIFooMgr :
	public virtual ZLContextClass,
	public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_singletonHello		( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass	( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_SINGLETON ( MOAIFooMgr )

	//----------------------------------------------------------------//
					MOAIFooMgr			( ZLContext& context );
					~MOAIFooMgr			();
};

#endif
