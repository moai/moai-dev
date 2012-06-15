// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICompassSensor.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getHeading
	@text	Returns the current heading according to the built-in compass.

	@in		MOAICompassSensor self
	@out	number heading
*/
int MOAICompassSensor::_getHeading ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICompassSensor, "U" )
	
	lua_pushnumber ( state, self->mHeading );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets or clears the callback to be issued when the heading changes.

	@in		MOAICompassSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAICompassSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICompassSensor, "U" )
	
	self->mCallback.SetStrongRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAICompassSensor
//================================================================//

//----------------------------------------------------------------//
void MOAICompassSensor::HandleEvent ( USStream& eventStream ) {

	this->mHeading = eventStream.Read < float >( 0.0f );
	
	if ( this->mCallback ) {
		MOAILuaStateHandle state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mHeading );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
MOAICompassSensor::MOAICompassSensor () :
	mHeading ( 0.0f ) {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAICompassSensor::~MOAICompassSensor () {
}

//----------------------------------------------------------------//
void MOAICompassSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICompassSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getHeading",			_getHeading },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICompassSensor::WriteEvent ( USStream& eventStream, float heading ) {

	eventStream.Write < float >( heading );
}
