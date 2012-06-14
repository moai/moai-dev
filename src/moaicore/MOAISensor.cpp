// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISensor.h>

//================================================================//
// lua
//================================================================//

//================================================================//
// MOAISensor
//================================================================//

//----------------------------------------------------------------//
MOAISensor::MOAISensor () {

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