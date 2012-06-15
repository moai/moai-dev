// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFooMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	_singletonHello
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
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( MOAIFooMgrBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFooMgr::~MOAIFooMgr () {
}

//----------------------------------------------------------------//
void MOAIFooMgr::RegisterLuaClass ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// MOAIFooBase::RegisterLuaClass ( state );

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "singletonHello",		_singletonHello },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
