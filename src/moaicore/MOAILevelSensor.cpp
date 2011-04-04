// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILevelSensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getLevel
	@text	Description of method coming soon.

	@in		MOAILevelSensor self
	@out	number x
	@out	number y
	@out	number z
*/
int MOAILevelSensor::_getLevel ( lua_State* L ) {
	LUA_SETUP ( MOAILevelSensor, "U" )
	
	lua_pushnumber ( state, self->mX );
	lua_pushnumber ( state, self->mY );
	lua_pushnumber ( state, self->mZ );
	
	return 3;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets the callback to be issued when the level changes.

	@in		MOAILevelSensor self
	@in		function callback
	@out	nil
*/
int MOAILevelSensor::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAILevelSensor, "U" )
	
	self->mCallback.SetRef ( state, 2, false );
	
	return 0;
}

//================================================================//
// MOAILevelSensor
//================================================================//

//----------------------------------------------------------------//
void MOAILevelSensor::HandleEvent ( USStream& eventStream ) {

	this->mX = eventStream.Read < float >();
	this->mY = eventStream.Read < float >();
	this->mZ = eventStream.Read < float >();
	
	if ( this->mCallback ) {
		USLuaStateHandle state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mX );
		lua_pushnumber ( state, this->mY );
		lua_pushnumber ( state, this->mZ );
		state.DebugCall ( 3, 0 );
	}
}

//----------------------------------------------------------------//
MOAILevelSensor::MOAILevelSensor () :
	mX ( 0.0f ),
	mY ( 0.0f ),
	mZ ( 0.0f ) {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAILevelSensor::~MOAILevelSensor () {
}

//----------------------------------------------------------------//
void MOAILevelSensor::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAILevelSensor::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getLevel",			_getLevel },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILevelSensor::WriteEvent ( USStream& eventStream, float x, float y, float z ) {

	eventStream.Write < float >( x );
	eventStream.Write < float >( y );
	eventStream.Write < float >( z );
}
