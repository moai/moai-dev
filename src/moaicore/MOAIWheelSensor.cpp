// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIWheelSensor.h>
#include <moaicore/MOAILogMessages.h>


//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getValue
	@text	Returns the current value of the wheel, based on delta events

	@in		MOAIWheelSensor self
	@out	number value
*/
int MOAIWheelSensor::_getValue ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWheelSensor, "U" )
	
	lua_pushnumber ( state, self->mValue );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDelta
	@text	Returns the delta of the wheel

	@in		MOAIWheelSensor self
	@out	number delta
*/
int MOAIWheelSensor::_getDelta ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWheelSensor, "U" )
	
	lua_pushnumber ( state, self->mDelta );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets or clears the callback to be issued on a wheel delta event

	@in		MOAIWheelSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAIWheelSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWheelSensor, "U" )
	
	self->mCallback.SetStrongRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIWheelSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIWheelSensor::HandleEvent ( USStream& eventStream ) {
	
	this->mDelta = eventStream.Read < float >( 0.0f );
	this->mValue += this->mDelta;
	
	if ( this->mCallback ) {
		MOAILuaStateHandle state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mDelta );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
MOAIWheelSensor::MOAIWheelSensor () :
	mValue ( 0 ),
	mDelta ( 0 ) {
	
	RTTI_SINGLE ( MOAISensor )

}

//----------------------------------------------------------------//
MOAIWheelSensor::~MOAIWheelSensor () {
}

//----------------------------------------------------------------//
void MOAIWheelSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIWheelSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getValue",				_getValue },
		{ "getDelta",				_getDelta },
		{ "setCallback",			_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIWheelSensor::Reset () {

	this->mDelta = 0;
}

//----------------------------------------------------------------//
void MOAIWheelSensor::WriteEvent ( USStream& eventStream, float value ) {

	eventStream.Write < float >( value );
}
