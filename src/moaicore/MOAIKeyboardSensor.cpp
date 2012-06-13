// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIKeyboardSensor.h>
#include <moaicore/MOAILogMessages.h>

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
/**	@name	keyDown
	@text	Checks to see if the button was pressed during the last iteration.

	@in		MOAIKeyboardSensor self
	@in		string key				Keycode value(s) to be checked against the input table.
	@out	boolean wasPressed
*/
int MOAIKeyboardSensor::_keyDown ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
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
/**	@name	keyIsDown
	@text	Checks to see if the button is currently down.

	@in		MOAIKeyboardSensor self
	@in		string key				Keycode value(s) to be checked against the input table.
	@out	boolean isDown
*/
int MOAIKeyboardSensor::_keyIsDown ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
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
/**	@name	keyIsUp
	@text	Checks to see if the specified key is currently up.

	@in		MOAIKeyboardSensor self
	@in		string key				Keycode value(s) to be checked against the input table.
	@out	boolean wasReleased
*/
int MOAIKeyboardSensor::_keyIsUp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
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
/**	@name	keyUp
	@text	Checks to see if the specified key was released during the last iteration.

	@in		MOAIKeyboardSensor self
	@in		string key				Keycode value(s) to be checked against the input table.
	@out	boolean wasReleased
*/
int MOAIKeyboardSensor::_keyUp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
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
/**	@name	setCallback
	@text	Sets or clears the callback to be issued when a key is pressed.

	@in		MOAIKeyboardSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAIKeyboardSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	self->mOnKey.SetStrongRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIKeyboardSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyboardSensor::HandleEvent ( USStream& eventStream ) {
	
	u32 keyCode = eventStream.Read < u32 >( 0 );
	bool down = eventStream.Read < bool >( false );
	
	bool inQueue = (( this->mState [ keyCode ] & ( DOWN | UP )) != 0 );
	
	if ( down ) {
		this->mState [ keyCode ] |= IS_DOWN | DOWN;
	}
	else {
		this->mState [ keyCode ] &= ~IS_DOWN;
		this->mState [ keyCode ] |= UP;
	}
	
	if ( this->mOnKey ) {
		MOAILuaStateHandle state = this->mOnKey.GetSelf ();
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
void MOAIKeyboardSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );

	state.SetField ( -1, "SHIFT", ( u32 )MOAIKeyCodes::SHIFT );
	state.SetField ( -1, "CONTROL", ( u32 )MOAIKeyCodes::CONTROL );
	state.SetField ( -1, "ALT", ( u32 )MOAIKeyCodes::ALT );
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
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
