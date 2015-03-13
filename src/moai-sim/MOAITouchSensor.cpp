// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAITouchSensor.h>
#include <moai-sim/MOAIInputQueue.h>

const float MOAITouchSensor::DEFAULT_TAPTIME = 0.6f;
const float MOAITouchSensor::DEFAULT_TAPMARGIN = 50.0f;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITouchSensor::_countTouches ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	state.Push ( self->mTop );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	down
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
/**	@lua	getActiveTouches
	@text	Returns the IDs of all of the touches currently occurring (for use with getTouch).

	@in		MOAITouchSensor self
	@out	number idx1
	@out	...
	@out	number idxN
*/
int MOAITouchSensor::_getActiveTouches ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 count = self->mTop;
	lua_checkstack( L, count );
	
	for ( u32 i = 0; i < count; ++i ) {
		lua_pushnumber ( state, self->mActiveStack [ i ]);
	}
	return count;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITouchSensor::_getCenterLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	u32 count = self->mTop;
	if ( count == 0 ) return 0;
	
	ZLVec2D loc;
	
	for ( u32 i = 0; i < count; ++i ) {
		u32 touchID = self->mActiveStack [ i ];
		const MOAITouch& touch = self->mTouches [ touchID ];
		
		if ( i == 0 ) {
			loc.mX = touch.mX;
			loc.mY = touch.mY;
		}
		else {
			loc.mX += touch.mX;
			loc.mY += touch.mY;
		}
	}
	
	loc.Scale ( 1.0f / ( float )count );
	
	state.Push ( loc.mX );
	state.Push ( loc.mY );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getTouch
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
/**	@lua	hasTouches
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
/**	@lua	isDown
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
/**	@lua	setAcceptCancel
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
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued when the pointer location changes.

	@in		MOAITouchSensor self
	@opt	function callback		Default value is nil.
	@out	nil
*/
int MOAITouchSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITouchSensor, "U" )
	
	self->mCallback.SetRef ( state, 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTapMargin
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
/**	@lua	setTapTime
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
/**	@lua	up
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
void MOAITouchSensor::EnqueueTouchEvent ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, u32 touchID, bool down, float x, float y ) {

	if ( queue.WriteEventHeader < MOAITouchSensor >( deviceID, sensorID )) {
	
		float time = ( float )ZLDeviceTime::GetTimeInSeconds ();
		
		u32 eventType = down ? TOUCH_DOWN : TOUCH_UP;

		queue.Write < u32 >( eventType );
		queue.Write < u32 >( touchID );
		queue.Write < float >( x );
		queue.Write < float >( y );
		queue.Write < float >( time );
	}
}

//----------------------------------------------------------------//
void MOAITouchSensor::EnqueueTouchEventCancel ( MOAIInputQueue& queue, u8 deviceID, u8 sensorID ) {

	if ( queue.WriteEventHeader < MOAITouchSensor >( deviceID, sensorID )) {
		queue.Write < u32 >( TOUCH_CANCEL );
	}
}

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
	float testTime = time - this->mTapTime;
	
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
void MOAITouchSensor::ParseEvent ( ZLStream& eventStream ) {

	u32 eventType = eventStream.Read < u32 >( 0 );
	
	// we only get TOUCH_DOWN, TOUCH_UP and TOUCH_CANCEL events
	// so we have to infer TOUCH_MOVE

	if ( eventType == TOUCH_CANCEL ) {
		
		// for now, TOUCH_CANCEL clobbers all touches
		this->Clear ();
		
		if ( this->mCallback && this->mAcceptCancel ) {
			MOAIScopedLuaState state = this->mCallback.GetSelf ();
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
		
		// see if there's already a record for this touch event
		u32 idx = this->FindTouch ( touch.mTouchID );
		
		if ( eventType == TOUCH_DOWN ) { // TOUCH_DOWN or TOUCH_MOVE
			
			// if it's a new touch, this is really a TOUCH_DOWN
			if ( idx == UNKNOWN_TOUCH ) {
				
				// if we're maxed out on touches, this touch will be ignored until something
				// frees up. after that it will be counted as a TOUCH_DOWN
				idx = this->AddTouch ();
				if ( idx == UNKNOWN_TOUCH ) return;
				
				// see if we touched down in a linger
				touch.mTapCount = CheckLingerList ( touch.mX, touch.mY, touch.mTime ) + 1;

				touch.mState = IS_DOWN | DOWN;
			}
			else {
			
				// we already knew about this touch, so it's a move
				touch.mState = this->mTouches [ idx ].mState | IS_DOWN;
				touch.mTapCount = this->mTouches [ idx ].mTapCount;
				eventType = TOUCH_MOVE;
			}
		}
		else if ( idx != UNKNOWN_TOUCH ) {
			
			// we know about the touch and it's not a TOUCH_DOWN, so it must be a TOUCH_UP
			
			// create a little cloud of linger to remember where the touch lifted up
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
			
			// if we have a valid touch, invoke the callback
			
			this->mTouches [ idx ] = touch;
			
			if ( this->mCallback ) {
				
				MOAIScopedLuaState state = this->mCallback.GetSelf ();
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
void MOAITouchSensor::PrintStacks () {

	STLString stacks = "";

	stacks.write ( "[" );

	for ( u32 i = 0; i < MAX_TOUCHES; ++i ) {
	
		if ( i == this->mTop ) {
			stacks.write ( "|" );
		}
		else {
			stacks.write ( " " );
		}
	
		stacks.write ( "%d", ( int )this->mAllocStack [ i ]);
	}
	
	stacks.write ( " ] [" );
	
	for ( u32 i = 0; i < MAX_TOUCHES; ++i ) {
	
		if ( i == this->mTop ) {
			stacks.write ( "|" );
		}
		else {
			stacks.write ( " " );
		}
		
		if ( this->mActiveStack [ i ] < MAX_TOUCHES ) {
			stacks.write ( "%d", ( int )this->mActiveStack [ i ]);
		}
		else {
			stacks.write ( "-" );
		}
	}
	
	stacks.write ( " ]\n" );
	
	MOAILog ( 0, 0, stacks.c_str ());
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

	MOAISensor::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "countTouches",		_countTouches },
		{ "down",				_down },
		{ "getActiveTouches",	_getActiveTouches },
		{ "getCenterLoc",		_getCenterLoc },
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
	float time = ( float ) ZLDeviceTime::GetTimeInSeconds () - mTapTime;

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
