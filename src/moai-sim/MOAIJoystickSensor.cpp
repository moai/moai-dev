// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIJoystickSensor.h>
#include <moai-sim/MOAIInputMgr.h>

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
/**	@lua	getVector
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
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued when the joystick vector changes.

	@in		MOAIJoystickSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAIJoystickSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIJoystickSensor, "U" )
	
	self->mOnStick.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIJoystickSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIJoystickSensor::EnqueueJoystickEvent( ZLIndex deviceID, ZLIndex sensorID, float x, float y ) {

	MOAIInputMgr& inputMgr = MOAIInputMgr::Get ();
	if ( inputMgr.WriteEventHeader < MOAIJoystickSensor >( deviceID, sensorID )) {
		inputMgr.Write < float >( x );
		inputMgr.Write < float >( y );
	}
}

//----------------------------------------------------------------//
MOAIJoystickSensor::MOAIJoystickSensor () {

	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAIJoystickSensor, MOAISensor )
}

//----------------------------------------------------------------//
MOAIJoystickSensor::~MOAIJoystickSensor () {
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::ParseEvent ( ZLStream& eventStream ) {

	this->mX = eventStream.Read < float >( 0.0f );
	this->mY = eventStream.Read < float >( 0.0f );
	
	if ( this->mOnStick ) {
		MOAIScopedLuaState state = this->mOnStick.GetSelf ();
		lua_pushnumber ( state, this->mX );
		lua_pushnumber ( state, this->mY );
		state.DebugCall ( 2, 0 );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIJoystickSensor::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAISensor, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAISensor, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getVector",		_getVector },
		{ "setCallback",	_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
