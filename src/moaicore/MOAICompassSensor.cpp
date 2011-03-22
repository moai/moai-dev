// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICompassSensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICompassSensor::_getHeading ( lua_State* L ) {
	LUA_SETUP ( MOAICompassSensor, "U" )
	
	lua_pushnumber ( state, self->mHeading );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICompassSensor::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAICompassSensor, "U" )
	
	self->mCallback.SetRef ( state, 2, false );
	
	return 0;
}

//================================================================//
// MOAICompassSensor
//================================================================//

//----------------------------------------------------------------//
void MOAICompassSensor::HandleEvent ( USStream& eventStream ) {

	this->mHeading = eventStream.Read < float >();
	
	if ( this->mCallback ) {
		USLuaStateHandle state = this->mCallback.GetSelf ();
		lua_pushnumber ( state, this->mHeading );
		state.DebugCall ( 1, 0 );
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
void MOAICompassSensor::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAICompassSensor::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "getHeading",			_getHeading },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICompassSensor::WriteEvent ( USStream& eventStream, float heading ) {

	eventStream.Write < float >( heading );
}
