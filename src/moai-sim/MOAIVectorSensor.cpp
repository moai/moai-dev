// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIInputQueue.h>
#include <moai-sim/MOAIVectorSensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getVector
	@text	Polls the current status of the vector sensor.

	@in		MOAIVectorSensor self
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIVectorSensor::_getVector ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorSensor, "U" )
	
	lua_pushnumber ( state, self->mX );
	lua_pushnumber ( state, self->mY );
	lua_pushnumber ( state, self->mZ );
	
	return 3;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets or clears the callback to be issued when the level changes.

	@in		MOAIMotionSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAIVectorSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorSensor, "U" )
	
	self->mCallback.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIVectorSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorSensor::EnqueueVectorEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, float x, float y, float z ) {

	if ( queue.WriteEventHeader < MOAIVectorSensor >( deviceID, sensorID )) {
		queue.Write < float >( x );
		queue.Write < float >( y );
		queue.Write < float >( z );
	}
}

//----------------------------------------------------------------//
MOAIVectorSensor::MOAIVectorSensor () :
	mX ( 0.0f ),
	mY ( 0.0f ),
	mZ ( 0.0f ) {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAIVectorSensor::~MOAIVectorSensor () {
}

//----------------------------------------------------------------//
void MOAIVectorSensor::ParseEvent ( ZLStream& eventStream ) {

	this->mX = eventStream.Read < float >( 0.0f );
	this->mY = eventStream.Read < float >( 0.0f );
	this->mZ = eventStream.Read < float >( 0.0f );
	
	if ( this->mCallback ) {
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mX );
		lua_pushnumber ( state, this->mY );
		lua_pushnumber ( state, this->mZ );
		state.DebugCall ( 3, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIVectorSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVectorSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getVector",			_getVector },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
