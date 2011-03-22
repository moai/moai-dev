// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITouchSensor.h>

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
int MOAITouchSensor::_down ( lua_State* L ) {
	LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, 0 );
	
	if ( idx < MAX_TOUCHES ) {
		lua_pushboolean ( state, ( self->mTouches [ idx ].mState & DOWN ) == DOWN );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITouchSensor::_getActiveTouches ( lua_State* L ) {
	LUA_SETUP ( MOAITouchSensor, "U" )
	
	for ( u32 i = 0; i < self->mTop; ++i ) {
		lua_pushnumber ( state, self->mActiveStack [ i ]);
	}
	return self->mTop;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITouchSensor::_getTouch ( lua_State* L ) {
	LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, 0 );
	
	if ( idx < MAX_TOUCHES ) {
		
		MOAITouch& touch = self->mTouches [ idx ];
		
		lua_pushnumber ( state, touch.mX );
		lua_pushnumber ( state, touch.mY );
		lua_pushnumber ( state, touch.mTapCount );
			
		return 3;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITouchSensor::_hasTouches ( lua_State* L ) {
	LUA_SETUP ( MOAITouchSensor, "U" )

	lua_pushboolean ( state, ( self->mTop > 0 ));
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITouchSensor::_isDown ( lua_State* L ) {
	LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, 0 );
	
	if ( idx < MAX_TOUCHES ) {
		lua_pushboolean ( state, ( self->mTouches [ idx ].mState & IS_DOWN ) == IS_DOWN );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setCallback ( ref )</tt>\n
\n
	Sets the callback function for when the pointer moves.
	@param ref Function to call.
*/
int MOAITouchSensor::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAITouchSensor, "UF" )
	
	self->mCallback.SetRef ( state, 2, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAITouchSensor::_up ( lua_State* L ) {
	LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, 0 );
	
	if ( idx < MAX_TOUCHES ) {
		lua_pushboolean ( state, ( self->mTouches [ idx ].mState & UP ) == UP );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAITouchSensor
//================================================================//

//----------------------------------------------------------------//
u32 MOAITouchSensor::AddTouch () {

	u32 idx = UNKNOWN_TOUCH;

	if ( this->mTop < MAX_TOUCHES ) {
		idx = this->mAllocStack [ this->mTop ];
		this->mActiveStack [ this->mTop ] = idx;
		this->mTop++;
		
		//this->PrintStacks ();
	}
	return idx;
}

//----------------------------------------------------------------//
void MOAITouchSensor::Clear () {

	this->mTop = 0;
	for ( u32 i = 0; i < MAX_TOUCHES; ++i ) {
		this->mTouches [ i ].mState = 0;
		this->mAllocStack [ i ] = i;
		this->mActiveStack [ i ] = MAX_TOUCHES;
	}
}

//----------------------------------------------------------------//
u32 MOAITouchSensor::FindTouch ( u32 touchID ) {

	for ( u32 i = 0; i < this->mTop; ++i ) {
		u32 idx = this->mActiveStack [ i ];
		if ( this->mTouches [ idx ].mTouchID == touchID ) {
			return idx;
		}
	}
	return UNKNOWN_TOUCH;
}

//----------------------------------------------------------------//
void MOAITouchSensor::HandleEvent ( USStream& eventStream ) {

	u32 eventType = eventStream.Read < u32 >();

	if ( eventType == TOUCH_CANCEL ) {
		
		this->Clear ();
		
		if ( this->mCallback ) {
			USLuaStateHandle state = this->mCallback.GetSelf ();
			lua_pushnumber ( state, eventType );
			state.DebugCall ( 1, 0 );
		}
	}
	else {
		
		MOAITouch touch;
		
		touch.mState		= 0;
		touch.mTouchID		= eventStream.Read < u32 >();
		touch.mX			= eventStream.Read < float >();
		touch.mY			= eventStream.Read < float >();
		touch.mTapCount		= eventStream.Read < u32 >();
		
		u32 idx = this->FindTouch ( touch.mTouchID );
		
		if ( eventType == TOUCH_DOWN ) {
			
			if ( idx == UNKNOWN_TOUCH ) {
				
				idx = this->AddTouch ();
				if ( idx == UNKNOWN_TOUCH ) return;
				
				touch.mState = IS_DOWN | DOWN;
			}
			else {
			
				if ( idx == UNKNOWN_TOUCH ) return;
				touch.mState = this->mTouches [ idx ].mState | IS_DOWN;
				eventType = TOUCH_MOVE;
			}
		}
		else {
			
			touch.mState &= ~IS_DOWN;
			touch.mState |= UP;
			touch.mTouchID = 0;
		}
		
		if ( idx != UNKNOWN_TOUCH ) {
			
			this->mTouches [ idx ] = touch;
			
			if (( idx != UNKNOWN_TOUCH ) && ( this->mCallback )) {
				
				USLuaStateHandle state = this->mCallback.GetSelf ();
				lua_pushnumber ( state, eventType );
				lua_pushnumber ( state, idx );
				lua_pushnumber ( state, touch.mX );
				lua_pushnumber ( state, touch.mY );
				lua_pushnumber ( state, touch.mTapCount );
				state.DebugCall ( 5, 0 );
			}
		}
	}
}

//----------------------------------------------------------------//
MOAITouchSensor::MOAITouchSensor () {

	RTTI_SINGLE ( MOAISensor )
	
	this->Clear ();
}

//----------------------------------------------------------------//
MOAITouchSensor::~MOAITouchSensor () {
}

//----------------------------------------------------------------//
void MOAITouchSensor::PrintStacks () {

	printf ( "[" );

	for ( u32 i = 0; i < MAX_TOUCHES; ++i ) {
	
		if ( i == this->mTop ) {
			printf ( "|" );
		}
		else {
			printf ( " " );
		}
	
		printf ( "%d", ( int )this->mAllocStack [ i ]);
	}
	
	printf ( " ] [" );
	
	for ( u32 i = 0; i < MAX_TOUCHES; ++i ) {
	
		if ( i == this->mTop ) {
			printf ( "|" );
		}
		else {
			printf ( " " );
		}
		
		if ( this->mActiveStack [ i ] < MAX_TOUCHES ) {
			printf ( "%d", ( int )this->mActiveStack [ i ]);
		}
		else {
			printf ( "-" );
		}
	}
	
	printf ( " ]\n" );
}

//----------------------------------------------------------------//
void MOAITouchSensor::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "TOUCH_DOWN", ( u32 )TOUCH_DOWN );
	state.SetField ( -1, "TOUCH_MOVE", ( u32 )TOUCH_MOVE );
	state.SetField ( -1, "TOUCH_UP", ( u32 )TOUCH_UP );
	state.SetField ( -1, "TOUCH_CANCEL", ( u32 )TOUCH_CANCEL );
}

//----------------------------------------------------------------//
void MOAITouchSensor::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "down",				_down },
		{ "getActiveTouches",	_getActiveTouches },
		{ "getTouch",			_getTouch },
		{ "hasTouches",			_hasTouches },
		{ "isDown",				_isDown },
		{ "setCallback",		_setCallback },
		{ "up",					_up },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITouchSensor::Reset () {

	//bool remove = false;

	u32 top = this->mTop;
	u32 j = 0;
	for ( u32 i = 0; i < top; ++i ) {
		
		u32 idx = this->mActiveStack [ i ];
		MOAITouch& touch = this->mTouches [ idx ];
		
		if (( touch.mState & IS_DOWN ) == 0 ) {
			touch.mState = 0;
			--this->mTop;
			this->mAllocStack [ this->mTop ] = idx;
			//remove = true;
		}
		else {
			touch.mState &= ~( DOWN | UP );	
			this->mActiveStack [ j++ ] = this->mActiveStack [ i ];
		}
	}
	
	if ( this->mTop == 0 ) {
		this->Clear ();
	}
	
	//if ( remove ) {
	//	this->PrintStacks ();
	//}
}

//----------------------------------------------------------------//
STLString MOAITouchSensor::ToString () {

	STLString repr;

	//PRETTY_PRINT ( repr, mX )
	//PRETTY_PRINT ( repr, mY )

	return repr;
}

//----------------------------------------------------------------//
void MOAITouchSensor::WriteEvent ( USStream& eventStream, u32 touchID, bool down, float x, float y, u32 tapCount ) {

	u32 eventType = down ? TOUCH_DOWN : TOUCH_UP;

	eventStream.Write < u32 >( eventType );
	eventStream.Write < u32 >( touchID );
	eventStream.Write < float >( x );
	eventStream.Write < float >( y );
	eventStream.Write < u32 >( tapCount );
}

//----------------------------------------------------------------//
void MOAITouchSensor::WriteEventCancel ( USStream& eventStream ) {

	eventStream.Write < u32 >( TOUCH_CANCEL );
}
