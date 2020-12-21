// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIMotionSensor.h>
#include <moai-sim/MOAIInputMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getLevel
	@text	Polls the current status of the level sensor.

	@in		MOAIMotionSensor self
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIMotionSensor::_getLevel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMotionSensor, "U" )
	
	lua_pushnumber ( state, self->mX );
	lua_pushnumber ( state, self->mY );
	lua_pushnumber ( state, self->mZ );
	
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued when the level changes.

	@in		MOAIMotionSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAIMotionSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMotionSensor, "U" )
	
	self->mCallback.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIMotionSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIMotionSensor::EnqueueLevelEvent ( ZLIndex deviceID, ZLIndex sensorID, float x, float y, float z ) {

	MOAIInputMgr& inputMgr = MOAIInputMgr::Get ();
	if ( inputMgr.WriteEventHeader < MOAIMotionSensor >( deviceID, sensorID )) {
		inputMgr.Write < float >( x );
		inputMgr.Write < float >( y );
		inputMgr.Write < float >( z );
	}
}


//----------------------------------------------------------------//
MOAIMotionSensor::MOAIMotionSensor () :
	mX ( 0.0f ),
	mY ( 0.0f ),
	mZ ( 0.0f ) {
	
	RTTI_BEGIN ( MOAIMotionSensor )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMotionSensor >)
		RTTI_EXTEND ( MOAISensor )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMotionSensor::~MOAIMotionSensor () {
}

//----------------------------------------------------------------//
void MOAIMotionSensor::ParseEvent ( ZLStream& eventStream ) {

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

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMotionSensor::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIMotionSensor::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getLevel",			_getLevel },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
