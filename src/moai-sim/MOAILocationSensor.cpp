// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAILocationSensor.h>
#include <moai-sim/MOAIInputQueue.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getLocation
	@text	Returns the current information about the physical location.

	@in		MOAILocationSensor self
	@out	number longitude
	@out	number latitude
	@out	number haccuracy		The horizontal (long/lat) accuracy.
	@out	number altitude
	@out	number vaccuracy		The vertical (altitude) accuracy.
	@out	number speed
*/
int MOAILocationSensor::_getLocation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILocationSensor, "U" )
	
	lua_pushnumber ( state, self->mLongitude );
	lua_pushnumber ( state, self->mLatitude );
	lua_pushnumber ( state, self->mHAccuracy );
	lua_pushnumber ( state, self->mAltitude );
	lua_pushnumber ( state, self->mVAccuracy );
	lua_pushnumber ( state, self->mSpeed );
	
	return 6;
}

//----------------------------------------------------------------//
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued when the location changes.

	@in		MOAILocationSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAILocationSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILocationSensor, "U" )
	
	self->mCallback.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAILocationSensor
//================================================================//

//----------------------------------------------------------------//
void MOAILocationSensor::EnqueueLocationEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	if ( queue.WriteEventHeader < MOAILocationSensor >( deviceID, sensorID )) {
		queue.Write < double >( longitude );
		queue.Write < double >( latitude );
		queue.Write < double >( altitude );
		queue.Write < float >( hAccuracy );
		queue.Write < float >( vAccuracy );
		queue.Write < float >( speed );
	}
}

//----------------------------------------------------------------//
MOAILocationSensor::MOAILocationSensor () :
	mLongitude ( 0.0 ),
	mLatitude ( 0.0 ),
	mAltitude ( 0.0 ),
	mHAccuracy ( 0.0f ),
	mVAccuracy ( 0.0f ),
	mSpeed ( 0.0f ) {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAILocationSensor::~MOAILocationSensor () {
}

//----------------------------------------------------------------//
void MOAILocationSensor::ParseEvent ( ZLStream& eventStream ) {

	this->mLongitude	= eventStream.Read < double >( 0.0 );
	this->mLatitude		= eventStream.Read < double >( 0.0 );
	this->mAltitude		= eventStream.Read < double >( 0.0 );
	this->mHAccuracy	= eventStream.Read < float >( 0.0f );
	this->mVAccuracy	= eventStream.Read < float >( 0.0f );
	this->mSpeed		= eventStream.Read < float >( 0.0f );
	
	if ( this->mCallback ) {
		MOAIScopedLuaState state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mLongitude );
		lua_pushnumber ( state, this->mLatitude );
		lua_pushnumber ( state, this->mHAccuracy );
		lua_pushnumber ( state, this->mAltitude );
		lua_pushnumber ( state, this->mVAccuracy );
		lua_pushnumber ( state, this->mSpeed );
		state.DebugCall ( 6, 0 );
	}
}

//----------------------------------------------------------------//
void MOAILocationSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAILocationSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getLocation",		_getLocation },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
