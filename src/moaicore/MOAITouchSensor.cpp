// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITouchSensor.h>

const float MOAITouchSensor::DEFAULT_TAPTIME = 0.6f;
const float MOAITouchSensor::DEFAULT_TAPMARGIN = 50.0f;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	down
	@text	Checks to see if the screen was touched during the last iteration.

	@in		MOAITouchSensor self
	@opt	number idx				Index of touch to check.
	@out	boolean wasPressed
*/
int MOAITouchSensor::_down ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, self->mActiveStack [ 0 ] );
	
	if ( idx < MAX_TOUCHES ) {
		lua_pushboolean ( state, ( self->mTouches [ idx ].mState & DOWN ) == DOWN );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getActiveTouches
	@text	Returns the IDs of all of the touches currently occurring (for use with getTouch).

	@in		MOAITouchSensor self
	@out	number idx1
	@out	...
	@out	number idxN
*/
int MOAITouchSensor::_getActiveTouches ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	for ( u32 i = 0; i < self->mTop; ++i ) {
		lua_pushnumber ( state, self->mActiveStack [ i ]);
	}
	return self->mTop;
}

//----------------------------------------------------------------//
/**	@name	getTouch
	@text	Checks to see if there are currently touches being made on the screen.

	@in		MOAITouchSensor self
	@in		number id				The ID of the touch.
	@out	number x
	@out	number y
	@out	number tapCount
*/
int MOAITouchSensor::_getTouch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
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
/**	@name	hasTouches
	@text	Checks to see if there are currently touches being made on the screen.

	@in		MOAITouchSensor self
	@out	boolean hasTouches
*/
int MOAITouchSensor::_hasTouches ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )

	lua_pushboolean ( state, ( self->mTop > 0 ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isDown
	@text	Checks to see if the touch status is currently down.

	@in		MOAITouchSensor self
	@opt	number idx				Index of touch to check.
	@out	boolean isDown
*/
int MOAITouchSensor::_isDown ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, self->mActiveStack [ 0 ] );
	
	if ( idx < MAX_TOUCHES ) {
		lua_pushboolean ( state, ( self->mTouches [ idx ].mState & IS_DOWN ) == IS_DOWN );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAcceptCancel
	@text	Sets whether or not to accept cancel events ( these happen on iOS backgrounding ), default value is false
 
	@in		MOAITouchSensor self
	@in		boolean accept	true then touch cancel events will be sent 
	@out	nil
*/
int MOAITouchSensor::_setAcceptCancel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "UB" )
	
	self->mAcceptCancel = state.GetValue < bool >( 2, self->mAcceptCancel );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets or clears the callback to be issued when the pointer location changes.

	@in		MOAITouchSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAITouchSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	self->mCallback.SetStrongRef ( state, 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTapMargin
	@text	Sets maximum distance between two touches for them to be considered a tap
	 
	@in		MOAITouchSensor self
	@in		number margin			Max difference on x and y between taps
	@out	nil
 */
int MOAITouchSensor::_setTapMargin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "UN" )
	
	float tapMargin = state.GetValue < float >( 2, DEFAULT_TAPMARGIN );
	self->mTapMargin = tapMargin;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTapTime
	@text	Sets the time between each touch for it to be counted as a tap
	 
	@in		MOAITouchSensor self
	@in		number time				New time between taps
	@out	nil
*/
int MOAITouchSensor::_setTapTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "UN" )
	
	float tapTime = state.GetValue < float >( 2, DEFAULT_TAPTIME );
	self->mTapTime = tapTime;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	up
	@text	Checks to see if the screen was untouched (is no longer being touched) during the last iteration.

	@in		MOAITouchSensor self
	@opt	number idx				Index of touch to check.
	@out	boolean wasPressed
*/
int MOAITouchSensor::_up ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, self->mActiveStack [ 0 ] );
	
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
void MOAITouchSensor::AddLingerTouch ( MOAITouchLinger& touch ) {

	if ( this->mLingerTop < MAX_TOUCHES ) {
		this->mLingerTouches [ this->mLingerTop ] = touch;
		++this->mLingerTop;
	}
}

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
s32 MOAITouchSensor::CheckLingerList ( float x, float y, float time ) {

	u32 top = this->mLingerTop;
	float margin = this->mTapMargin;
	float testTime = time - mTapTime;
	
	s32 maxTapCount = 0;
	for ( u32 i = 0; i < top; ++i ) {
		if ( this->mLingerTouches [ i ].mX > ( x - margin ) &&
			 this->mLingerTouches [ i ].mX < ( x + margin ) &&
			 this->mLingerTouches [ i ].mY > ( y - margin ) &&
			 this->mLingerTouches [ i ].mY < ( y + margin ) &&
			 this->mLingerTouches [ i ].mTime > testTime &&
			 maxTapCount < this->mLingerTouches [ i ].mTapCount ) {

			maxTapCount = this->mLingerTouches [ i ].mTapCount;
		}
	}
	
	return maxTapCount;
}

//----------------------------------------------------------------//
void MOAITouchSensor::Clear () {

	this->mTop = 0;
	this->mLingerTop = 0;

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

	u32 eventType = eventStream.Read < u32 >( 0 );

	if ( eventType == TOUCH_CANCEL ) {
		
		this->Clear ();
		
		if ( this->mCallback && this->mAcceptCancel ) {
			MOAILuaStateHandle state = this->mCallback.GetSelf ();
			lua_pushnumber ( state, eventType );
			state.DebugCall ( 1, 0 );
		}
	}
	else {
		
		MOAITouch touch;
		
		touch.mState		= 0;
		touch.mTouchID		= eventStream.Read < u32 >( 0 );
		touch.mX			= eventStream.Read < float >( 0.0f );
		touch.mY			= eventStream.Read < float >( 0.0f );
		touch.mTime			= eventStream.Read < float >( 0.0f );
		touch.mTapCount     = 0;
		
		u32 idx = this->FindTouch ( touch.mTouchID );
		
		if ( eventType == TOUCH_DOWN ) {

			if ( idx == UNKNOWN_TOUCH ) {
				
				idx = this->AddTouch ();
				if ( idx == UNKNOWN_TOUCH ) return;
				
				touch.mTapCount = CheckLingerList ( touch.mX, touch.mY, touch.mTime ) + 1;

				touch.mState = IS_DOWN | DOWN;
			}
			else {
			
				if ( idx == UNKNOWN_TOUCH ) return;
				touch.mState = this->mTouches [ idx ].mState | IS_DOWN;
				touch.mTapCount = this->mTouches [ idx ].mTapCount;
				eventType = TOUCH_MOVE;
			}
		}
		else {
			
			MOAITouchLinger linger;
			linger.mX = this->mTouches [ idx ].mX;
			linger.mY = this->mTouches [ idx ].mY;
			linger.mTapCount = this->mTouches [ idx ].mTapCount;
			linger.mTime = this->mTouches [ idx ].mTime;

			this->AddLingerTouch ( linger );

			touch.mState &= ~IS_DOWN;
			touch.mState |= UP;
			touch.mTouchID = 0;
			touch.mTapCount = CheckLingerList ( touch.mX, touch.mY, touch.mTime );
		}
		
		if ( idx != UNKNOWN_TOUCH ) {
			
			this->mTouches [ idx ] = touch;
			
			if (( idx != UNKNOWN_TOUCH ) && ( this->mCallback )) {
				
				MOAILuaStateHandle state = this->mCallback.GetSelf ();
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
	
	mTapTime = DEFAULT_TAPTIME;
	mTapMargin = DEFAULT_TAPMARGIN;
	
	mAcceptCancel = false;
	
	this->Clear ();
}

//----------------------------------------------------------------//
MOAITouchSensor::~MOAITouchSensor () {
}

//----------------------------------------------------------------//
void MOAITouchSensor::PrintStacks () {

	MOAIPrint ( "[" );

	for ( u32 i = 0; i < MAX_TOUCHES; ++i ) {
	
		if ( i == this->mTop ) {
			MOAIPrint ( "|" );
		}
		else {
			MOAIPrint ( " " );
		}
	
		MOAIPrint ( "%d", ( int )this->mAllocStack [ i ]);
	}
	
	MOAIPrint ( " ] [" );
	
	for ( u32 i = 0; i < MAX_TOUCHES; ++i ) {
	
		if ( i == this->mTop ) {
			MOAIPrint ( "|" );
		}
		else {
			MOAIPrint ( " " );
		}
		
		if ( this->mActiveStack [ i ] < MAX_TOUCHES ) {
			MOAIPrint ( "%d", ( int )this->mActiveStack [ i ]);
		}
		else {
			MOAIPrint ( "-" );
		}
	}
	
	MOAIPrint ( " ]\n" );
}

//----------------------------------------------------------------//
void MOAITouchSensor::RegisterLuaClass ( MOAILuaState& state ) {

	MOAISensor::RegisterLuaClass ( state );

	state.SetField ( -1, "TOUCH_DOWN", ( u32 )TOUCH_DOWN );
	state.SetField ( -1, "TOUCH_MOVE", ( u32 )TOUCH_MOVE );
	state.SetField ( -1, "TOUCH_UP", ( u32 )TOUCH_UP );
	state.SetField ( -1, "TOUCH_CANCEL", ( u32 )TOUCH_CANCEL );
}

//----------------------------------------------------------------//
void MOAITouchSensor::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "down",				_down },
		{ "getActiveTouches",	_getActiveTouches },
		{ "getTouch",			_getTouch },
		{ "hasTouches",			_hasTouches },
		{ "isDown",				_isDown },
		{ "setAcceptCancel",	_setAcceptCancel },
		{ "setCallback",		_setCallback },
		{ "setTapMargin",		_setTapMargin },
		{ "setTapTime",			_setTapTime },
		{ "up",					_up },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITouchSensor::Reset () {

	u32 top = this->mTop;
	u32 j = 0;
	for ( u32 i = 0; i < top; ++i ) {
		
		u32 idx = this->mActiveStack [ i ];
		MOAITouch& touch = this->mTouches [ idx ];
		
		if (( touch.mState & IS_DOWN ) == 0 ) {

			touch.mState = 0;
			--this->mTop;
			this->mAllocStack [ this->mTop ] = idx;
		}
		else {
			touch.mState &= ~( DOWN | UP );	
			this->mActiveStack [ j++ ] = this->mActiveStack [ i ];
		}
	}
	
	bool changed = true;
	float time = ( float ) USDeviceTime::GetTimeInSeconds () - mTapTime;

	while ( changed ) {
		changed = false;
		top = this->mLingerTop;

		for ( u32 i = 0; i < top; ++i ) {
			if ( this->mLingerTouches [ i ].mTime < time ) {

				this->mLingerTouches [ i ] = this->mLingerTouches [ top ];
				this->mLingerTop--;

				changed = true;
				break;
			}
		}
	}

	if ( this->mTop == 0 && this->mLingerTop == 0 ) {
		this->Clear ();
	}
}

//----------------------------------------------------------------//
void MOAITouchSensor::WriteEvent ( USStream& eventStream, u32 touchID, bool down, float x, float y, float time ) {

	u32 eventType = down ? TOUCH_DOWN : TOUCH_UP;

	eventStream.Write < u32 >( eventType );
	eventStream.Write < u32 >( touchID );
	eventStream.Write < float >( x );
	eventStream.Write < float >( y );
	eventStream.Write < float >( time );
}

//----------------------------------------------------------------//
void MOAITouchSensor::WriteEventCancel ( USStream& eventStream ) {

	eventStream.Write < u32 >( TOUCH_CANCEL );
}
