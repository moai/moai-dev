// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIClass.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getClass
	@text	Return reference to the object's class table.

	@in		userdata object
	@out	table class
*/
int MOAIClass::_getClass ( lua_State* L ) {
	
	USLuaState state ( L );
	USLuaObject* object = ( USLuaObject* )state.GetPtrUserData ( 1 );

	if ( object ) {
		object->PushLuaClassTable ( state );
	}
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getName
	@text	Return the name of the class.

	@in		userdata object
	@out	string classname
*/
int MOAIClass::_getName ( lua_State* L ) {
	
	USLuaState state ( L );
	USLuaObject* object = ( USLuaObject* )state.GetPtrUserData ( 1 );

	if ( object ) {
		cc8* classname = object->TypeName ();
		lua_pushstring ( state, classname );
	}
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isInstance
	@text	Check to see if object is a valid instance of a Moai class.

	@in		userdata object
	@out	boolean isInstance
*/
int MOAIClass::_isInstance ( lua_State* L ) {
	
	USLuaState state ( L );
	USLuaObject* object = ( USLuaObject* )state.GetPtrUserData ( 1 );

	if ( object ) {
		state.Push ( true );
	}
	else {
		state.Push ( false );
	}
	return 1;
}

//================================================================//
// MOAIClass
//================================================================//

//----------------------------------------------------------------//
MOAIClass::MOAIClass () {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIClass::~MOAIClass () {
}

//----------------------------------------------------------------//
void MOAIClass::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getClass",			_getClass },
		{ "getName",			_getName },
		{ "isInstance",			_isInstance },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

