// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISim.h>
#include <moaicore/MOAITimer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getTimesExecuted
	@text	Gets the number of times the timer has completed a cycle.

	@in		MOAITimer self
	@out	number nTimes
*/
int MOAITimer::_getTimesExecuted ( lua_State* L ) {
	LUA_SETUP ( MOAITimer, "UN" )

	lua_pushnumber ( L, self->mTimesExecuted );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Set a function to be called every time the timer finishes a cycle.

	@in		MOAITimer self
	@in		function onCycle
	@out	nil
*/
int MOAITimer::_setCallback ( lua_State* L ) {
	LUA_SETUP ( MOAITimer, "UF" )

	self->mCallback.SetRef ( state, 2, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMode
	@text	Sets the playback mode of the timer.

	@in		MOAITimer self
	@in		number mode		One of: MOAITimer.NORMAL, MOAITimer.REVERSE, MOAITimer.LOOP, MOAITimer.LOOP_REVERSE, MOAITimer.PING_PONG
	@out	nil
*/
int MOAITimer::_setMode ( lua_State* L ) {
	LUA_SETUP ( MOAITimer, "UN" )

	self->mMode = state.GetValue < int >( 2, NORMAL );
	
	if( self->mMode == REVERSE ||
		self->mMode == LOOP_REVERSE ){
		self->mDirection = -1.0f;
	}
	else {
		self->mDirection = 1.0f;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSpan
	@text	Sets the playback mode of the timer.

	@overload	Span will be 0 to endTime

		@in		MOAITimer self
		@in		number endTime
		@out	nil
	
	@overload	Span will be startTime to endTime
	
		@in		MOAITimer self
		@in		number startTime
		@in		number endTime
		@out	nil
*/
int MOAITimer::_setSpan ( lua_State* L ) {
	LUA_SETUP ( MOAITimer, "UN" )

	if ( state.IsType ( 3, LUA_TNUMBER )) {
		self->mStartTime = state.GetValue < float >( 2, 0.0f );
		self->mEndTime = state.GetValue < float >( 3, 1.0f );
	}
	else {
		self->mStartTime = 0.0f;
		self->mEndTime = state.GetValue < float >( 2, 1.0f );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSpeed
	@text	Sets the playback speed. This affects only the timer, not
			its children in the action tree.

	@in		MOAITimer self
	@in		number speed
	@out	nil
*/
int MOAITimer::_setSpeed ( lua_State* L ) {
	LUA_SETUP ( MOAITimer, "UN" )

	self->mSpeed = state.GetValue < float >( 2, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTime
	@text	Manually set the current time. This will be wrapped
			into the current span.

	@in		MOAITimer self
	@opt	number time			Default value is 0.
	@out	nil
*/
int MOAITimer::_setTime ( lua_State* L ) {
	LUA_SETUP ( MOAITimer, "U" )
	
	float time = state.GetValue < float >( 2, 0.0f );
	self->SetTime ( time );
	
	return 0;
}

//================================================================//
// MOAITimer
//================================================================//

//----------------------------------------------------------------//
void MOAITimer::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch ( attrID ) {
		case ATTR_TIME:
			this->mTime = attrOp.Op ( this->mTime );
			return;
	};
}

//----------------------------------------------------------------//
void MOAITimer::Callback () {

	if ( this->mCallback ) {
		USLuaStateHandle state = this->mCallback.GetSelf ();
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
float MOAITimer::DoStep ( float step ) {

	float length = this->mEndTime - this->mStartTime;

	if ( length == 0.0f ) {
		this->Stop ();
		this->Callback ();
		this->ScheduleUpdate ();
		return 0.0f;
	}

	float t0 = this->mTime;
	this->mTime += step * this->mSpeed * this->mDirection;
	
	float t1 = this->mTime;
	float result = 0.0f;
	
	switch ( this->mMode ) {
	
		case NORMAL: {
			
			if ( this->mTime >= this->mEndTime ) {
				this->mTime = this->mEndTime;
				this->Stop ();
				this->Callback ();
			}
			result = this->mTime - t0;
			break;
		}
		
		case LOOP: {
		
			while ( this->mTime >= this->mEndTime ) {
				this->mTime -= length;
				this->mTimesExecuted++;
				this->Callback ();
			}
			result = t1 - t0;
			break;
		}
		case REVERSE: {
		
			if ( this->mTime < this->mStartTime ) {
				this->mTime = this->mStartTime ;
				this->Stop ();
				this->Callback ();
			}
			result = this->mTime - t0;
			break;
		}
		
		case LOOP_REVERSE: {
		
			while ( this->mTime < this->mStartTime ) {
				this->mTime += length;
				this->mTimesExecuted++;
				this->Callback ();
			}
			result = t1 - t0;
			break;
		}
		
		case PING_PONG: {
			
			while (( this->mTime < this->mStartTime ) || ( this->mTime >= this->mEndTime )) {
			
			
				if ( this->mTime < this->mStartTime ) {
					this->mTime = this->mStartTime + ( this->mStartTime - this->mTime );
				}
				else {
					this->mTime = this->mEndTime - ( this->mTime - this->mEndTime );
				}
				
				this->mSpeed *= -1.0f;
				this->mTimesExecuted++;
				this->Callback ();
			}
			result = this->mTime - t0;
			break;
		}
	}
	
	this->ScheduleUpdate ();
	return result;
}

//----------------------------------------------------------------//
bool MOAITimer::IsBusy () {

	if ( this->mMode == NORMAL ) {
		return (( this->mTime >= this->mStartTime ) && ( this->mTime < this->mEndTime ));
	}
	
	if ( this->mMode == REVERSE ) {
		return (( this->mTime > this->mStartTime ) && ( this->mTime <= this->mEndTime ));
	}
	
	return this->IsActive ();
}

//----------------------------------------------------------------//
MOAITimer::MOAITimer () :
	mStartTime ( 0.0f ),
	mEndTime ( 1.0f ),
	mTime ( 0.0f ),
	mSpeed ( 1.0f ),
	mDirection ( 1.0f ),
	mMode ( NORMAL ),
	mTimesExecuted ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITimer::~MOAITimer () {
}

//----------------------------------------------------------------//
void MOAITimer::OnDepNodeUpdate () {
}

//----------------------------------------------------------------//
void MOAITimer::OnStart () {

	if( this->mDirection > 0.0f ) {
		this->mTime = this->mStartTime;
	}
	else {
		this->mTime = this->mEndTime;
	}
}

//----------------------------------------------------------------//
void MOAITimer::OnUpdate ( float step ) {

	this->DoStep ( step );
}

//----------------------------------------------------------------//
void MOAITimer::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "ATTR_TIME", ( u32 )ATTR_TIME );
	
	state.SetField ( -1, "NORMAL", ( u32 )NORMAL );
	state.SetField ( -1, "REVERSE", ( u32 )REVERSE );
	state.SetField ( -1, "LOOP", ( u32 )LOOP );
	state.SetField ( -1, "LOOP_REVERSE", ( u32 )LOOP_REVERSE );
	state.SetField ( -1, "PING_PONG", ( u32 )PING_PONG );
}

//----------------------------------------------------------------//
void MOAITimer::RegisterLuaFuncs ( USLuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "getTimesExecuted",	_getTimesExecuted },
		{ "setCallback",		_setCallback },
		{ "setMode",			_setMode },
		{ "setSpan",			_setSpan },
		{ "setSpeed",			_setSpeed },
		{ "setTime",			_setTime },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITimer::SetTime ( float time ) {

	float length = USFloat::Abs ( this->mEndTime - this->mStartTime );
	while ( time >= this->mEndTime ) {
		time -= length;
	}
	this->mTime = time;
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
STLString MOAITimer::ToString () {

	const char *mode;

	switch ( mMode ) {
		case LOOP:
			mode = "loop";
			break;
		case LOOP_REVERSE:
			mode = "loop reverse";
			break;
		case NORMAL:
			mode = "normal";
			break;
		case PING_PONG:
			mode = "ping pong";
			break;
		case REVERSE:
			mode = "reverse";
			break;
		default:
			mode = "UNDEFINED";
	};

	const char *direction = mDirection > 1 ? "forward" : "backward";

	STLString repr;

	PRETTY_PRINT ( repr, mSpeed )
	PRETTY_PRINT ( repr, mStartTime )
	PRETTY_PRINT ( repr, mEndTime )
	PRETTY_PRINT ( repr, mTime )
	PrettyPrint ( repr, "mMode", mode );
	PrettyPrint ( repr, "mDirection", direction );
	PRETTY_PRINT ( repr, mTimesExecuted )

	return repr;
}
