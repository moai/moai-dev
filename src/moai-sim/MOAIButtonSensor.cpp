// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIButtonSensor.h>
#include <moai-sim/MOAIInputQueue.h>

//================================================================//
// MOAIButtonEvent
//================================================================//
class MOAIButtonEvent {
public:

	bool	mDown;
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	down
	@text	Checks to see if the button was pressed during the last iteration.

	@in		MOAIButtonSensor self
	@out	boolean wasPressed
*/
int MOAIButtonSensor::_down ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonDown ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isDown
	@text	Checks to see if the button is currently down.

	@in		MOAIButtonSensor self
	@out	boolean isDown
*/
int MOAIButtonSensor::_isDown ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonIsDown ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isUp
	@text	Checks to see if the button is currently up.

	@in		MOAIButtonSensor self
	@out	boolean isUp
*/
int MOAIButtonSensor::_isUp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonIsUp ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	up
	@text	Checks to see if the button was released during the last iteration.

	@in		MOAIButtonSensor self
	@out	boolean wasReleased
*/
int MOAIButtonSensor::_up ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonUp ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued when button events occur.

	@in		MOAIButtonSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAIButtonSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIButtonSensor, "U" )
	
	self->mOnButton.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIButtonSensor
//================================================================//

//----------------------------------------------------------------//
bool MOAIButtonSensor::ButtonDown () {

	return (( mState & DOWN ) == DOWN );
}

//----------------------------------------------------------------//
bool MOAIButtonSensor::ButtonIsDown () {

	return (( mState & IS_DOWN ) == IS_DOWN );
}

//----------------------------------------------------------------//
bool MOAIButtonSensor::ButtonIsUp () {

	return (( mState & IS_DOWN ) == 0 );
}

//----------------------------------------------------------------//
bool MOAIButtonSensor::ButtonUp () {

	return (( mState & UP ) == UP );
}

//----------------------------------------------------------------//
void MOAIButtonSensor::EnqueueButtonEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, bool down ) {

	if ( queue.WriteEventHeader < MOAIButtonSensor >( deviceID, sensorID )) {
		queue.Write < bool >( down );
	}
}

//----------------------------------------------------------------//
MOAIButtonSensor::MOAIButtonSensor () :
	mState ( 0 ) {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAIButtonSensor::~MOAIButtonSensor () {
}

//----------------------------------------------------------------//
void MOAIButtonSensor::ParseEvent ( ZLStream& eventStream ) {

	bool down = eventStream.Read < bool >( false );
	
	if ( down ) {
		this->mState |= IS_DOWN | DOWN;
	}
	else {
		this->mState &= ~IS_DOWN;
		this->mState |= UP;
	}
	
	if ( this->mOnButton ) {
		MOAIScopedLuaState state = this->mOnButton.GetSelf ();
		lua_pushboolean ( state, down );
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIButtonSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIButtonSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "down",				_down },
		{ "isDown",				_isDown },
		{ "isUp",				_isUp },
		{ "up",					_up },
		{ "setCallback",		_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIButtonSensor::Reset () {

	// clear out the old events
	this->mState &= ~( DOWN | UP );	
}
