// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILocationSensor.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getLocation
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
/**	@name	setCallback
	@text	Sets or clears the callback to be issued when the location changes.

	@in		MOAILocationSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAILocationSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILocationSensor, "U" )
	
	self->mCallback.SetStrongRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAILocationSensor
//================================================================//

//----------------------------------------------------------------//
void MOAILocationSensor::HandleEvent ( USStream& eventStream ) {

	this->mLongitude	= eventStream.Read < double >( 0.0 );
	this->mLatitude		= eventStream.Read < double >( 0.0 );
	this->mAltitude		= eventStream.Read < double >( 0.0 );
	this->mHAccuracy	= eventStream.Read < float >( 0.0f );
	this->mVAccuracy	= eventStream.Read < float >( 0.0f );
	this->mSpeed		= eventStream.Read < float >( 0.0f );
	
	if ( this->mCallback ) {
		MOAILuaStateHandle state = this->mCallback.GetSelf ();
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
void MOAILocationSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAILocationSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getLocation",		_getLocation },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILocationSensor::WriteEvent ( USStream& eventStream, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	eventStream.Write < double >( longitude );
	eventStream.Write < double >( latitude );
	eventStream.Write < double >( altitude );
	eventStream.Write < float >( hAccuracy );
	eventStream.Write < float >( vAccuracy );
	eventStream.Write < float >( speed );
}
