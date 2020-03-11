// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAILocationSensor.h>
#include <moai-sim/MOAIInputMgr.h>

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
void MOAILocationSensor::EnqueueLocationEvent ( ZLIndex deviceID, ZLIndex sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	MOAIInputMgr& inputMgr = MOAIInputMgr::Get ();
	if ( inputMgr.WriteEventHeader < MOAILocationSensor >( deviceID, sensorID )) {
		inputMgr.Write < double >( longitude );
		inputMgr.Write < double >( latitude );
		inputMgr.Write < double >( altitude );
		inputMgr.Write < float >( hAccuracy );
		inputMgr.Write < float >( vAccuracy );
		inputMgr.Write < float >( speed );
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
	
	RTTI_BEGIN ( MOAILocationSensor )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAILocationSensor >)
		RTTI_EXTEND ( MOAISensor )
	RTTI_END
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

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAILocationSensor::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAILocationSensor::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getLocation",		_getLocation },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
