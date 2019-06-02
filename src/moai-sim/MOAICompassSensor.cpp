// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICompassSensor.h>
#include <moai-sim/MOAIInputMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getHeading
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
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued when the heading changes.

	@in		MOAICompassSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAICompassSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICompassSensor, "U" )
	
	self->mCallback.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAICompassSensor
//================================================================//

//----------------------------------------------------------------//
void MOAICompassSensor::EnqueueCompassEvent ( ZLIndex deviceID, ZLIndex sensorID, float heading ) {

	MOAIInputMgr& inputMgr = MOAIInputMgr::Get ();
	if ( inputMgr.WriteEventHeader < MOAICompassSensor >( deviceID, sensorID )) {
		inputMgr.Write < float >( heading );
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
void MOAICompassSensor::ParseEvent ( ZLStream& eventStream ) {

	this->mHeading = eventStream.Read < float >( 0.0f );
	
	if ( this->mCallback ) {
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mHeading );
		state.DebugCall ( 1, 0 );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAICompassSensor::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAISensor, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAICompassSensor::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAISensor, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getHeading",			_getHeading },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
