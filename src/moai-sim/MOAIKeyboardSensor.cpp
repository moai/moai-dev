// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIKeyboardSensor.h>
#include <moai-sim/MOAIInputQueue.h>
#include <contrib/moai_utf8.h>

namespace KeyboardEventType {
	enum {
		KEY,
		CHAR,
		EDIT,
		INVALID
	};
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	keyDown
	@text	Checks to see if one or more keys were pressed during the last iteration.

	@in		MOAIKeyboardSensor self
	@in		... keys					Keys to be checked against the input table.
										Each key can be specified using a MOAIKeyboardSensor.* constant or the corresponding string.
										Multiple strings can be combined: 'wasd' is equivalent to 'w', 'a', 's', 'd'.
	@out	boolean... down				For every specified key, indicates whether this key was pressed during the last iteration.
*/
int MOAIKeyboardSensor::_keyDown ( lua_State* L ) {
	return CheckKeys ( L, &MOAIKeyboardSensor::KeyDown );
}

//----------------------------------------------------------------//
/**	@lua	keyIsDown
	@text	Checks to see if one or more keys are currently pressed.

	@in		MOAIKeyboardSensor self
	@in		... keys					Keys to be checked against the input table.
										Each key can be specified using a MOAIKeyboardSensor.* constant or the corresponding string.
										Multiple strings can be combined: 'wasd' is equivalent to 'w', 'a', 's', 'd'.
	@out	boolean... isDown			For every specified key, indicates whether this key is currently pressed.
*/
int MOAIKeyboardSensor::_keyIsDown ( lua_State* L ) {
	return CheckKeys ( L, &MOAIKeyboardSensor::KeyIsDown );
}

//----------------------------------------------------------------//
/**	@lua	keyIsUp
	@text	Checks to see if one or more keys are currently up.

	@in		MOAIKeyboardSensor self
	@in		... keys					Keys to be checked against the input table.
										Each key can be specified using a MOAIKeyboardSensor.* constant or the corresponding string.
										Multiple strings can be combined: 'wasd' is equivalent to 'w', 'a', 's', 'd'.
	@out	boolean... isUp				For every specified key, indicates whether this key is currently up.
*/
int MOAIKeyboardSensor::_keyIsUp ( lua_State* L ) {
	return CheckKeys ( L, &MOAIKeyboardSensor::KeyIsUp );
}

//----------------------------------------------------------------//
/**	@lua	keyUp
	@text	Checks to see if one or more keys were released during the last iteration.

	@in		MOAIKeyboardSensor self
	@in		... keys					Keys to be checked against the input table.
										Each key can be specified using a MOAIKeyboardSensor.* constant or the corresponding string.
										Multiple strings can be combined: 'wasd' is equivalent to 'w', 'a', 's', 'd'.
	@out	boolean... up				For every specified key, indicates whether this key was released during the last iteration.
*/
int MOAIKeyboardSensor::_keyUp ( lua_State* L ) {
	return CheckKeys ( L, &MOAIKeyboardSensor::KeyUp );
}

//----------------------------------------------------------------//
/**	@lua	setCallback
	@text	This method has been deprecated. Use setKeyCallback instead.

	@in		MOAIKeyboardSensor self
	@opt	function callback			A callback function with the signature "void callback(number keyCode, bool down)".
										Default value is nil.
	@out	nil
*/
int MOAIKeyboardSensor::_setCallback ( lua_State* L ) {
	return _setKeyCallback ( L );
}

//----------------------------------------------------------------//
/**	@lua	setCharCallback
	@text	Sets or clears the callback to be issued when a character is typed.

	@in		MOAIKeyboardSensor self
	@opt	function callback			A callback function with the signature "void callback(string character)".
										Note that for non-ASCII characters, the string argument will be a multibyte UTF-8 character.
										Default value is nil.
	@out	nil
*/
int MOAIKeyboardSensor::_setCharCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	self->mOnChar.SetRef ( state, 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setEditCallback
	@text	Sets or clears the callback to be issued when a character is editing.
 
	@in		MOAIKeyboardSensor self
	@opt	function callback			A callback function with the signature "void callback(string text, number start, number length)".
										Note that for non-ASCII characters, the string argument will be a multibyte UTF-8 character.
										Default value is nil.
	@out	nil
 */
int MOAIKeyboardSensor::_setEditCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	self->mOnEdit.SetRef ( state, 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setKeyCallback
	@text	Sets or clears the callback to be issued when a key is pressed or released.

	@in		MOAIKeyboardSensor self
	@opt	function callback			A callback function with the signature "void callback(number keyCode, bool down)".
										Default value is nil.
	@out	nil
*/
int MOAIKeyboardSensor::_setKeyCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )
	
	self->mOnKey.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIKeyboardSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIKeyboardSensor::EnqueueKeyboardCharEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, u32 unicodeChar ) {

	// Don't allow non-printable characters
	if ( unicodeChar < ' ' ) return;

	if ( queue.WriteEventHeader < MOAIKeyboardSensor >( deviceID, sensorID )) {
		queue.Write < u32 >( KeyboardEventType::CHAR );
		queue.Write < u32 >( unicodeChar );
	}
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::EnqueueKeyboardEditEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, char const* text, u32 start, u32 editLength, u32 maxLength ) {
	if ( queue.WriteEventHeader < MOAIKeyboardSensor >( deviceID, sensorID )) {
		queue.Write < u32 >( KeyboardEventType::EDIT );
		queue.Write < u32 >( start );
		queue.Write < u32 >( editLength );
		queue.Write < u32 >( maxLength );
		
		for ( u32 i = 0; i < maxLength; i++ ) {
			queue.Write < char >( text[i] );
		}
	}
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::EnqueueKeyboardKeyEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, u32 keyID, bool down ) {

	if ( keyID >= MOAI_KEY_TOTAL ) return;

	if ( queue.WriteEventHeader < MOAIKeyboardSensor >( deviceID, sensorID )) {
		queue.Write < u32 >( KeyboardEventType::KEY );
		queue.Write < u32 >( keyID );
		queue.Write < bool >( down );
	}
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::EnqueueKeyboardTextEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, cc8* text ) {
	
	int i = 0;
	while ( text [ i ]) {
		u_int32_t uc = moai_u8_nextchar ( text, &i );
		MOAIKeyboardSensor::EnqueueKeyboardCharEvent ( queue, deviceID, sensorID, uc );
	}
}

//----------------------------------------------------------------//
// For each key, returns whether the specified predicate returns true.
// Expects self and a number of keycodes or strings on the stack.
int MOAIKeyboardSensor::CheckKeys ( lua_State* L, bool ( MOAIKeyboardSensor::* predicate )( u32 keyCode )) {
	MOAI_LUA_SETUP ( MOAIKeyboardSensor, "U" )

	u32 argCount = lua_gettop ( state );
	int resultCount = 0;

	for ( u32 arg = 2; arg <= argCount; ++arg ) {
		if ( state.IsType ( arg, LUA_TNUMBER )) {
			// Argument is number. Treat as single key code.
			u32 keyCode = state.GetValue < u32 > ( arg, 0 );
			bool result = ( self->*predicate )( keyCode );
			lua_pushboolean ( state, result );
			++resultCount;
		}
		else if ( state.IsType ( arg, LUA_TSTRING ) ) {
			// Argument is string. Treat as list of key codes.
			cc8* str = lua_tostring ( state, arg );
			for ( u32 i = 0; str [ i ]; ++i ) {
				u32 keyCode = str [ i ];
				bool result = ( self->*predicate )( keyCode );
				lua_pushboolean ( state, result );
				++resultCount;
			}
		}
		else {
			// Argument is invalid. Push nil to return the expected number of results
			lua_pushnil ( state );
			++resultCount;
		}
	}

	return resultCount;
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
	
	memset ( this->mState, 0, MOAI_KEY_TOTAL * sizeof ( u32 ));
}

//----------------------------------------------------------------//
MOAIKeyboardSensor::~MOAIKeyboardSensor () {
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::ParseEvent ( ZLStream& eventStream ) {
	
	u32 eventType = eventStream.Read < u32 >( KeyboardEventType::INVALID );
	if ( eventType == KeyboardEventType::KEY ) {

		u32 keyCode = eventStream.Read < u32 >( 0 );
		bool down = eventStream.Read < bool >( false );
	
		// Ignore redundant events
		if ( down == KeyIsDown ( keyCode )) return;

		bool alreadyInClearQueue = ( this->mState [ keyCode ] & ( DOWN | UP )) != 0;
	
		if ( down ) {
			this->mState [ keyCode ] |= IS_DOWN | DOWN;
		}
		else {
			this->mState [ keyCode ] &= ~IS_DOWN;
			this->mState [ keyCode ] |= UP;
		}
	
		if ( this->mOnKey ) {
			MOAIScopedLuaState state = this->mOnKey.GetSelf ();
			lua_pushnumber ( state, keyCode );
			lua_pushboolean ( state, down );
			state.DebugCall ( 2, 0 );
		}
	
		if ( !alreadyInClearQueue ) {
			this->mClearQueue [ this->mClearCount ] = keyCode;
			this->mClearCount++;
		}
	} else if ( eventType == KeyboardEventType::CHAR ) {

		u32 unicodeChar = eventStream.Read < u32 >( -1 );
		
		if ( this->mOnChar ) {
			// Convert to UTF-8 and zero-terminate
			char utf8Sequence [ 5 ];
			int byteCount = moai_u8_wc_toutf8 ( utf8Sequence, unicodeChar );
			utf8Sequence [ byteCount ] = 0;

			if ( byteCount ) {
				MOAIScopedLuaState state = this->mOnChar.GetSelf ();
				lua_pushstring ( state, utf8Sequence );
				state.DebugCall ( 1, 0 );
			}
		}
	} else if ( eventType == KeyboardEventType::EDIT ) {
		
		u32 start = eventStream.Read < u32 >( 0 );
		u32 editLength = eventStream.Read < u32 >( 0 );
		u32 maxLength = eventStream.Read < u32 >( 0 );
		
		if ( this->mOnEdit ) {
			char *text = (char*)malloc(maxLength);
			eventStream.ReadBytes(text, ( size_t )maxLength);
			
			MOAIScopedLuaState state = this->mOnEdit.GetSelf ();
			lua_pushstring ( state, text );
			lua_pushnumber ( state, start );
			lua_pushnumber ( state, editLength );
			state.DebugCall ( 3, 0 );
			
			free(text);
		} else {
			eventStream.SetCursor(eventStream.GetCursor() + maxLength);
		}
	}
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "keyDown",				_keyDown },
		{ "keyIsDown",				_keyIsDown },
		{ "keyIsUp",				_keyIsUp },
		{ "keyUp",					_keyUp },
		{ "setCallback",			_setCallback },
		{ "setCharCallback",		_setCharCallback },
		{ "setEditCallback",		_setEditCallback },
		{ "setKeyCallback",			_setKeyCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIKeyboardSensor::Reset () {
	// Clear the DOWN and UP flags
	for ( u32 i = 0; i < this->mClearCount; ++i ) {
		u32 keyCode = this->mClearQueue [ i ];
		this->mState [ keyCode ] &= ~( DOWN | UP );	
	}
	this->mClearCount = 0;
}
