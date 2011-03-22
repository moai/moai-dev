// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIKeyboardSensor.h>

//================================================================//
// MOAIKeyboardEvent
//================================================================//
class MOAIKeyboardEvent {
public:

	u32		mKey;
	bool	mDown;
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>keyDown ( str )</tt>\n
\n
	Determines if a key was JUST pressed.
	@param str Keycode value(s) to be checked against the input table
	@return True if the key was just pressed, false if not.
*/
int MOAIKeyboardSensor::_keyDown ( lua_State* L ) {
	LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	if ( state.IsType ( 2, LUA_TSTRING )) {
		
		cc8* str = lua_tostring ( state, 2 );
		
		int count;
		for ( count = 0; str [ count ]; ++count ) {
			lua_pushboolean ( state, self->KeyDown ( str [ count ]));
		}
		return count;
	}
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		u32 keyCode = state.GetValue < u32 >( 2, 0 );
		lua_pushboolean ( state, self->KeyDown ( keyCode ));
		return 1;
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>keyIsDown ( str )</tt>\n
\n
	Determines if a key is being held down.
	@param str Keycode value(s) to be checked against the input table
	@return True if the key is being depressed, false if not.
*/
int MOAIKeyboardSensor::_keyIsDown ( lua_State* L ) {
	LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	if ( state.IsType ( 2, LUA_TSTRING )) {
		
		cc8* str = lua_tostring ( state, 2 );
		
		int count;
		for ( count = 0; str [ count ]; ++count ) {
			lua_pushboolean ( state, self->KeyIsDown ( str [ count ]));
		}
		return count;
	}
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		u32 keyCode = state.GetValue < u32 >( 2, 0 );
		lua_pushboolean ( state, self->KeyIsDown ( keyCode ));
		return 1;
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>keyIsUp ( str )</tt>\n
\n
	Determines if a key is not being held.
	@param str Keycode value(s) to be checked against the input table
	@return True if key is not being pressed at all, false if not.
*/
int MOAIKeyboardSensor::_keyIsUp ( lua_State* L ) {
	LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	if ( state.IsType ( 2, LUA_TSTRING )) {
		
		cc8* str = lua_tostring ( state, 2 );
		
		int count;
		for ( count = 0; str [ count ]; ++count ) {
			lua_pushboolean ( state, self->KeyIsUp ( str [ count ]));
		}
		return count;
	}
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		u32 keyCode = state.GetValue < u32 >( 2, 0 );
		lua_pushboolean ( state, self->KeyIsUp ( keyCode ));
		return 1;
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>keyUp ( str )</tt>\n
\n
	Determines if a key was JUST released.
	@param str Keycode value(s) to be checked against the input table
	@return True if the key was just released, false if not.
*/
int MOAIKeyboardSensor::_keyUp ( lua_State* L ) {
	LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	if ( state.IsType ( 2, LUA_TSTRING )) {
		
		cc8* str = lua_tostring ( state, 2 );
		
		int count;
		for ( count = 0; str [ count ]; ++count ) {
			lua_pushboolean ( state, self->KeyUp ( str [ count ]));
		}
		return count;
	}
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		u32 keyCode = state.GetValue < u32 >( 2, 0 );
		lua_pushboolean ( state, self->KeyUp ( keyCode ));
		return 1;
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setCallback ( ref )</tt>\n
\n
	Sets the callback function for when a key is pressed.
	@param ref Function to call.
*/
int MOAIKeyboardSensor::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAIKeyboardSensor, "UF" )
	
	self->mOnKey.SetRef ( state, 2, false );
	
	return 0;
}

//================================================================//
// MOAIKeyboardSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyboardSensor::HandleEvent ( USStream& eventStream ) {
	
	u32 keyCode = eventStream.Read < u32 >();
	bool down = eventStream.Read < bool >();
	
	bool inQueue = (( this->mState [ keyCode ] & ( DOWN | UP )) != 0 );
	
	if ( down ) {
		this->mState [ keyCode ] |= IS_DOWN | DOWN;
	}
	else {
		this->mState [ keyCode ] &= ~IS_DOWN;
		this->mState [ keyCode ] |= UP;
	}
	
	if ( this->mOnKey ) {
		USLuaStateHandle state = this->mOnKey.GetSelf ();
		lua_pushnumber ( state, keyCode );
		lua_pushboolean ( state, down );
		state.DebugCall ( 2, 0 );
	}
	
	if ( !inQueue ) {
		this->mClearQueue [ this->mClearCount ] = keyCode;
		this->mClearCount++;
	}
}

//----------------------------------------------------------------//
bool MOAIKeyboardSensor::KeyDown ( u32 keyID ) {

	return (( this->mState [ keyID ] & DOWN ) == DOWN );
}

//----------------------------------------------------------------//
bool MOAIKeyboardSensor::KeyIsDown ( u32 keyID ) {

	return (( this->mState [ keyID ] & IS_DOWN ) == IS_DOWN );
}

//----------------------------------------------------------------//
bool MOAIKeyboardSensor::KeyIsUp ( u32 keyID ) {

	return (( this->mState [ keyID ] & IS_DOWN ) == 0 );
}

//----------------------------------------------------------------//
bool MOAIKeyboardSensor::KeyUp ( u32 keyID ) {

	return (( this->mState [ keyID ] & UP ) == UP );
}

//----------------------------------------------------------------//
MOAIKeyboardSensor::MOAIKeyboardSensor () :
	mClearCount ( 0 ) {
	
	RTTI_SINGLE ( MOAISensor )
	
	memset ( this->mState, 0, MOAIKeyCodes::TOTAL * sizeof ( u32 ));
}

//----------------------------------------------------------------//
MOAIKeyboardSensor::~MOAIKeyboardSensor () {
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "SHIFT", ( u32 )MOAIKeyCodes::SHIFT );
	state.SetField ( -1, "CONTROL", ( u32 )MOAIKeyCodes::CONTROL );
	state.SetField ( -1, "ALT", ( u32 )MOAIKeyCodes::ALT );
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "keyDown",				_keyDown },
		{ "keyIsDown",				_keyIsDown },
		{ "keyIsUp",				_keyIsUp },
		{ "keyUp",					_keyUp },
		{ "setCallback",			_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::Reset () {

	// clear out the old events
	for ( u32 i = 0; i < this->mClearCount; ++i ) {
		u32 keyCode = this->mClearQueue [ i ];
		this->mState [ keyCode ] &= ~( DOWN | UP );	
	}
	this->mClearCount = 0;
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::WriteEvent ( USStream& eventStream, u32 key, bool down ) {

	eventStream.Write < u32 >( key );
	eventStream.Write < bool >( down );
}
