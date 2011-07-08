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
	@text	Sets the callback to be issued when the heading changes.

	@in		MOAICompassSensor self
	@in		function callback
	@out	nil
*/
int MOAICompassSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICompassSensor, "U" )
	
	self->mCallback.SetRef ( state, 2, false );
	
	return 0;
}

//================================================================//
// MOAICompassSensor
//================================================================//

//----------------------------------------------------------------//
void MOAICompassSensor::HandleEvent ( USStream& eventStream ) {

	this->mHeading = eventStream.Read < float >();
	
	if ( this->mCallback ) {
		USLuaStateHandle state = this->mCallback.GetSelf ();
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
void MOAICompassSensor::RegisterLuaClass ( USLuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICompassSensor::RegisterLuaFuncs ( USLuaState& state ) {

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
