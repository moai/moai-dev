// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMotionSensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getLevel
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
/**	@name	setCallback
	@text	Sets or clears the callback to be issued when the level changes.

	@in		MOAIMotionSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAIMotionSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMotionSensor, "U" )
	
	self->mCallback.SetStrongRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIMotionSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIMotionSensor::HandleEvent ( USStream& eventStream ) {

	this->mX = eventStream.Read < float >( 0.0f );
	this->mY = eventStream.Read < float >( 0.0f );
	this->mZ = eventStream.Read < float >( 0.0f );
	
	if ( this->mCallback ) {
		MOAILuaStateHandle state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mX );
		lua_pushnumber ( state, this->mY );
		lua_pushnumber ( state, this->mZ );
		state.DebugCall ( 3, 0 );
	}
}

//----------------------------------------------------------------//
MOAIMotionSensor::MOAIMotionSensor () :
	mX ( 0.0f ),
	mY ( 0.0f ),
	mZ ( 0.0f ) {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAIMotionSensor::~MOAIMotionSensor () {
}

//----------------------------------------------------------------//
void MOAIMotionSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIMotionSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getLevel",			_getLevel },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMotionSensor::WriteEvent ( USStream& eventStream, float x, float y, float z ) {

	eventStream.Write < float >( x );
	eventStream.Write < float >( y );
	eventStream.Write < float >( z );
}
