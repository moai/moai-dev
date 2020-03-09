// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIFooMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	singletonHello
	@text	Prints the string 'MOAIFooMgr singleton foo!' to the console.

	@out	nil
*/
int MOAIFooMgr::_singletonHello ( lua_State* L ) {
	UNUSED ( L );

	printf ( "MOAIFooMgr singleton foo!\n" );
	
	return 0;
}

//================================================================//
// MOAIFooMgr
//================================================================//

//----------------------------------------------------------------//
MOAIFooMgr::MOAIFooMgr () {
	
	// register all classes MOAIFooMgr derives from
	// we need this for custom RTTI implementation
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIFooMgr )
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( MOAIFooMgrBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFooMgr::~MOAIFooMgr () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFooMgr::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );

	// call any initializers for base classes here:
	// MOAI_CALL_SUPER_ONCE ( composer, MOAIFooBase, MOAILuaObject_RegisterLuaClass ( composer, state ));

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "singletonHello",		_singletonHello },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
