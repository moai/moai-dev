// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIButtonSensor.h>

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
/**	@brief <tt>( wasPressed ) down ( self )</tt>\n
\n
	Checks to see if the button was pressed during the last iteration.
	@param self (in)
	@param down (out)
*/
int MOAIButtonSensor::_down ( lua_State* L ) {
	LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonDown ());
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( isDown ) isDown ( self )</tt>\n
\n
	Checks to see if the button is currently down.
	@param self (in)
	@param isDown (out)
*/
int MOAIButtonSensor::_isDown ( lua_State* L ) {
	LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonIsDown ());
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( isUp ) isUp ( self )</tt>\n
\n
	Checks to see if the button is currently up.
	@param self (in)
	@param isUp (out)
*/
int MOAIButtonSensor::_isUp ( lua_State* L ) {
	LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonIsUp ());
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( wasReleased ) up ( params )</tt>\n
\n
	Checks to see if the button was released during the last iteration.
	@param self (in)
	@param up (out)
*/
int MOAIButtonSensor::_up ( lua_State* L ) {
	LUA_SETUP ( MOAIButtonSensor, "U" )
	
	lua_pushboolean ( state, self->ButtonUp ());
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) setCallback ( self, callback )</tt>\n
\n
	Sets the function to be called on button events.
	@param self (in)
	@param callback (in)
*/
int MOAIButtonSensor::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAIButtonSensor, "UF" )
	
	self->mOnButton.SetRef ( state, 2, false );
	
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
void MOAIButtonSensor::HandleEvent ( USStream& eventStream ) {

	bool down = eventStream.Read < bool >();
	
	if ( down ) {
		this->mState |= IS_DOWN | DOWN;
	}
	else {
		this->mState &= ~IS_DOWN;
		this->mState |= UP;
	}
	
	if ( this->mOnButton ) {
		USLuaStateHandle state = this->mOnButton.GetSelf ();
		lua_pushboolean ( state, down );
		state.DebugCall ( 1, 0 );
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
void MOAIButtonSensor::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIButtonSensor::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
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

//----------------------------------------------------------------//
void MOAIButtonSensor::WriteEvent ( USStream& eventStream, bool down ) {

	eventStream.Write < bool >( down );
}
