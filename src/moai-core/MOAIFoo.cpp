// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIFoo.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	classHello
	@text	Class (a.k.a. static) method. Prints the string 'MOAIFoo class foo!' to the console.

	@out	nil
*/
int MOAIFoo::_classHello ( lua_State* L ) {
	UNUSED ( L );
	
	printf ( "MOAIFoo class foo!\n" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	instanceHello
	@text	Prints the string 'MOAIFoo instance foo!' to the console.
	
	@in		MOAIFoo self
	@out	nil
*/
int MOAIFoo::_instanceHello ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFoo, "U" ) // this macro initializes the 'self' variable and type checks arguments
	
	printf ( "MOAIFoo instance foo!\n" );
	
	return 0;
}

//================================================================//
// MOAIFoo
//================================================================//

//----------------------------------------------------------------//
MOAIFoo::MOAIFoo () {
	
	// register all classes MOAIFoo derives from
	// we need this for custom RTTI implementation
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIFoo )
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( MOAIFooBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFoo::~MOAIFoo () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFoo::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );

	// call any initializers for base classes here:
	// MOAI_CALL_SUPER_ONCE ( composer, MOAIFooBase, MOAILuaObject_RegisterLuaClass ( composer, state ));

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "classHello",		_classHello },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFoo::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );

	// call any initializers for base classes here:
	// MOAI_CALL_SUPER_ONCE ( composer, MOAIFooBase, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "instanceHello",	_instanceHello },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

