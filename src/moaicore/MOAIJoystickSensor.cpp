// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIJoystickSensor.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// MOAIJoystickEvent
//================================================================//
class MOAIJoystickEvent {
public:

	float	mX;
	float	mY;
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getVector
	@text	Returns the joystick vector.

	@in		MOAIJoystickSensor self
	@out	number x
	@out	number y
*/
int MOAIJoystickSensor::_getVector ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIJoystickSensor, "U" )
	
	lua_pushnumber ( state, self->mX );
	lua_pushnumber ( state, self->mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets or clears the callback to be issued when the joystick vector changes.

	@in		MOAIJoystickSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAIJoystickSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIJoystickSensor, "U" )
	
	self->mOnStick.SetStrongRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIJoystickSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIJoystickSensor::HandleEvent ( USStream& eventStream ) {

	this->mX = eventStream.Read < float >( 0.0f );
	this->mY = eventStream.Read < float >( 0.0f );
	
	if ( this->mOnStick ) {
		MOAILuaStateHandle state = this->mOnStick.GetSelf ();
		lua_pushnumber ( state, this->mX );
		lua_pushnumber ( state, this->mY );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
MOAIJoystickSensor::MOAIJoystickSensor () {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAIJoystickSensor::~MOAIJoystickSensor () {
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getVector",		_getVector },
		{ "setCallback",	_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::WriteEvent ( USStream& eventStream, float x, float y ) {

	eventStream.Write < float >( x );
	eventStream.Write < float >( y );
}
