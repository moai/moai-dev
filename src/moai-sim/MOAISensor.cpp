// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAISensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISensor::_getTimestamp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISensor, "U" )
	
	lua_pushnumber ( state, self->mTimestamp );
	
	return 1;
}

//================================================================//
// MOAISensor
//================================================================//

//----------------------------------------------------------------//
MOAISensor::MOAISensor () :
	mType ( 0 ),
	mTimestamp ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAISensor::~MOAISensor () {
}

//----------------------------------------------------------------//
void MOAISensor::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "new",					MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getTimestamp",			_getTimestamp },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
