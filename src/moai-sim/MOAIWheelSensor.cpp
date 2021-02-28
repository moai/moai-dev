// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIWheelSensor.h>
#include <moai-sim/MOAIInputMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getValue
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
/**	@lua	getDelta
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
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued on a wheel delta event

	@in		MOAIWheelSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAIWheelSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWheelSensor, "U" )
	
	self->mCallback.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIWheelSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIWheelSensor::EnqueueWheelEvent ( MOAIInputMgr& inputMgr, ZLIndex deviceID, ZLIndex sensorID, float value ) {

	if ( inputMgr.WriteEventHeader < MOAIWheelSensor >( deviceID, sensorID )) {
		inputMgr.Write < float >( value );
	}
}

//----------------------------------------------------------------//
MOAIWheelSensor::MOAIWheelSensor ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAISensor ( context ),
	mValue ( 0 ),
	mDelta ( 0 ) {
		
	RTTI_BEGIN ( MOAIWheelSensor )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIWheelSensor >)
		RTTI_EXTEND ( MOAISensor )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIWheelSensor::~MOAIWheelSensor () {
}

//----------------------------------------------------------------//
void MOAIWheelSensor::ParseEvent ( ZLStream& eventStream ) {
	
	this->mDelta = eventStream.Read < float >( 0.0f );
	this->mValue += this->mDelta;
	
	if ( this->mCallback ) {
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mDelta );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIWheelSensor::ResetState () {

	this->mDelta = 0;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIWheelSensor::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIWheelSensor::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getValue",				_getValue },
		{ "getDelta",				_getDelta },
		{ "setCallback",			_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
