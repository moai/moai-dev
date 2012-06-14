// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAITimer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getTime
	@text	Return the current time.

	@in		MOAITimer self
	@out	number time
*/
int MOAITimer::_getTime( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimer, "U" )

	lua_pushnumber ( L, self->GetTime ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTimesExecuted
	@text	Gets the number of times the timer has completed a cycle.

	@in		MOAITimer self
	@out	number nTimes
*/
int MOAITimer::_getTimesExecuted ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimer, "U" )

	lua_pushnumber ( L, self->mTimesExecuted );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setCurve
	@text	Set or clear the curve to use for event generation.
	
	@in		MOAITimer self
	@opt	MOAIAnimCurve curve		Default value is nil.
	@out	nil
*/
int MOAITimer::_setCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimer, "U" );

	self->mCurve.Set ( *self, state.GetLuaObject < MOAIAnimCurve >( 2, true ));
	self->ScheduleUpdate ();

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
	MOAI_LUA_SETUP ( MOAITimer, "UN" )

	self->mMode = state.GetValue < int >( 2, NORMAL );
	
	if( self->mMode == REVERSE || self->mMode == LOOP_REVERSE || self->mMode == CONTINUE_REVERSE ){
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

	@overload	Span will be 0 to endTime.

		@in		MOAITimer self
		@in		number endTime
		@out	nil
	
	@overload	Span will be startTime to endTime.
	
		@in		MOAITimer self
		@in		number startTime
		@in		number endTime
		@out	nil
*/
int MOAITimer::_setSpan ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimer, "UN" )

	if ( state.IsType ( 3, LUA_TNUMBER )) {
		
		float startTime		= state.GetValue < float >( 2, 0.0f );
		float endTime		= state.GetValue < float >( 3, 1.0f );
		
		self->SetSpan ( startTime, endTime );
	}
	else {
		float span			= state.GetValue < float >( 2, 1.0f );
		
		self->SetSpan ( span );
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
	MOAI_LUA_SETUP ( MOAITimer, "UN" )

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
	MOAI_LUA_SETUP ( MOAITimer, "U" )
	
	float time = state.GetValue < float >( 2, 0.0f );
	self->SetTime ( time );
	
	return 0;
}

//================================================================//
// MOAITimer
//================================================================//

//----------------------------------------------------------------//
bool MOAITimer::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAITimerAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );
		
		if ( attrID == ATTR_TIME ) {
			attrOp.Apply ( this->GetTime (), op, MOAIAttrOp::ATTR_READ );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAITimer::DoStep ( float step ) {

	if ( step == 0.0f ) return;

	float length = this->mEndTime - this->mStartTime;

	if ( length == 0.0f ) {
		this->Stop ();
		this->ScheduleUpdate ();
		return;
	}

	float t0 = this->mTime;
	this->mTime += step * this->mSpeed * this->mDirection;
	
	float t1 = this->mTime;
	float result = 0.0f;
	
	switch ( this->mMode ) {
	
		case NORMAL: {
			
			if ( this->mTime >= this->mEndTime ) {
			
				this->OnEndSpan ();
				this->mTime = this->mEndTime;
				this->GenerateKeyframeCallbacks ( t0, this->mTime, true );
				this->mCycle = 1.0f;
				this->Stop ();
			}
			else {
				this->GenerateKeyframeCallbacks ( t0, this->mTime, false );
			}
			result = this->mTime - t0;
			break;
		}
		
		case REVERSE: {
		
			if ( this->mTime < this->mStartTime ) {
			
				this->OnEndSpan ();
				this->mTime = this->mStartTime ;
				this->GenerateKeyframeCallbacks ( t0, this->mTime, true );
				this->mCycle = -1.0f;
				this->Stop ();
			}
			else {
				this->GenerateKeyframeCallbacks ( t0, this->mTime, false );
			}
			result = this->mTime - t0;
			break;
		}
		
		case CONTINUE:
		case LOOP: {
			
			if ( this->mTime >= this->mEndTime ) {
				
				this->GenerateKeyframeCallbacks ( t0, this->mEndTime, true );
				
				while ( this->mTime >= this->mEndTime ) {
				
					this->OnEndSpan ();
					this->mTime -= length;

					if ( this->mMode == CONTINUE ) {
						this->mCycle += 1.0f;
					}

					this->OnLoop ();
					this->OnBeginSpan ();
					
					float end = this->mTime < this->mEndTime ? this->mTime : this->mEndTime;
					this->GenerateKeyframeCallbacks ( this->mStartTime, end, end == this->mEndTime );
				}
			}
			else {
				this->GenerateKeyframeCallbacks ( t0, this->mTime, false );
			}
			result = t1 - t0;
			break;
		}
		
		case CONTINUE_REVERSE:
		case LOOP_REVERSE: {
		
			if ( this->mTime <= this->mStartTime ) {
				
				this->GenerateKeyframeCallbacks ( t0, this->mStartTime, true );
				
				while ( this->mTime <= this->mStartTime ) {
				
					this->OnEndSpan ();
					this->mTime += length;

					if ( this->mMode == CONTINUE_REVERSE ) {
						this->mCycle -= 1.0f;
					}

					this->OnLoop ();
					this->OnBeginSpan ();
					
					float end = this->mTime > this->mStartTime ? this->mTime : this->mStartTime;
					this->GenerateKeyframeCallbacks ( this->mEndTime, end, end == this->mStartTime );
				}
			}
			else {
				this->GenerateKeyframeCallbacks ( t0, this->mTime, false );
			}
			result = t1 - t0;
			break;
		}
		
		case PING_PONG: {
			
			if (( this->mTime < this->mStartTime ) || ( this->mTime >= this->mEndTime )) {
			
				while (( this->mTime < this->mStartTime ) || ( this->mTime >= this->mEndTime )) {
					
					this->OnEndSpan ();
					
					if ( this->mTime < this->mStartTime ) {
						this->mTime = this->mStartTime + ( this->mStartTime - this->mTime );
						
						float end = this->mTime < this->mEndTime ? this->mTime : this->mEndTime;
						this->GenerateKeyframeCallbacks ( this->mStartTime, end, end == this->mEndTime );
					}
					else {
						this->mTime = this->mEndTime - ( this->mTime - this->mEndTime );
						
						float end = this->mTime > this->mStartTime ? this->mTime : this->mStartTime;
						this->GenerateKeyframeCallbacks ( this->mEndTime, end, end == this->mStartTime );
					}
					
					this->mDirection *= -1.0f;
					this->OnLoop ();
					this->OnBeginSpan ();
				}
			}
			else {
				this->GenerateKeyframeCallbacks ( t0, this->mTime, false );
			}
			result = this->mTime - t0;
			break;
		}
	}
	
	this->ScheduleUpdate ();
	return;
}

//----------------------------------------------------------------//
void MOAITimer::GenerateKeyframeCallbacks ( float t0, float t1, bool end ) {

	if ( t0 == t1 ) return;
	u32 size = this->mCurve ? this->mCurve->Size () : 0;
	if ( !size ) return;
		
	u32 keyID = ( int )this->mCurve->FindKeyID ( t0 );
	if ( size <= keyID ) {
		keyID = 0;
	}
	
	if ( t0 < t1 ) {
	
		for ( ; keyID < size; ++keyID ) {
			const MOAIAnimKey& key = this->mCurve->GetKey ( keyID );
			
			if (( end && ( key.mTime >= t1 )) || (( key.mTime >= t0 ) && ( key.mTime < t1 ))) {
				this->OnKeyframe ( keyID, key.mTime, this->mCurve->GetSample ( keyID ));
			}
			
			if ( key.mTime >= t1 ) break;
		}
	}
	else {
		
		for ( ; ( int )keyID > -1; --keyID ) {
			const MOAIAnimKey& key = this->mCurve->GetKey ( keyID );
		
			if (( end && ( key.mTime <= t1 )) || (( key.mTime <= t0 ) && ( key.mTime > t1 ))) {
				this->OnKeyframe ( keyID, key.mTime, this->mCurve->GetSample ( keyID ));
			}
			
			if ( key.mTime <= t1 ) break;
		}
	}
}

//----------------------------------------------------------------//
float MOAITimer::GetCycle () {

	return this->mCycle;
}

//----------------------------------------------------------------//
float MOAITimer::GetLength () {

	return this->mEndTime - this->mStartTime;
}

//----------------------------------------------------------------//
float MOAITimer::GetNormalizedTime () {

	float length = this->mEndTime - this->mStartTime;
	return this->GetTime () / length;
}

//----------------------------------------------------------------//
float MOAITimer::GetTime () {

	if (( this->mMode == CONTINUE ) || ( this->mMode == CONTINUE_REVERSE )) {
		float length = this->mEndTime - this->mStartTime;
		return this->mTime + ( length * this->mCycle );
	}
	return this->mTime;
}

//----------------------------------------------------------------//
bool MOAITimer::IsDone () {

	if ( this->mMode == NORMAL ) {
		return (( this->mTime < this->mStartTime ) || ( this->mTime >= this->mEndTime ));
	}
	
	if ( this->mMode == REVERSE ) {
		return (( this->mTime <= this->mStartTime ) || ( this->mTime > this->mEndTime ));
	}
	return false;
}

//----------------------------------------------------------------//
MOAITimer::MOAITimer () :
	mTime ( 0.0f ),
	mCycle ( 0.0f ),
	mSpeed ( 1.0f ),
	mDirection ( 1.0f ),
	mMode ( NORMAL ),
	mTimesExecuted ( 0.0f ),
	mStartTime ( 0.0f ),
	mEndTime ( 1.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITimer::~MOAITimer () {

	this->mCurve.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAITimer::OnBeginSpan () {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( EVENT_TIMER_BEGIN_SPAN, state )) {
		state.Push ( this->mTimesExecuted );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITimer::OnDepNodeUpdate () {
}

//----------------------------------------------------------------//
void MOAITimer::OnEndSpan () {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( EVENT_TIMER_END_SPAN, state )) {
		state.Push ( this->mTimesExecuted );
		state.DebugCall ( 2, 0 );
	}
	
	this->mTimesExecuted += 1.0f;
}

//----------------------------------------------------------------//
void MOAITimer::OnKeyframe ( u32 idx, float time, float value ) {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( EVENT_TIMER_KEYFRAME, state )) {
		state.Push ( idx + 1 );
		state.Push ( mTimesExecuted );
		state.Push ( time );
		state.Push ( value );
		state.DebugCall ( 5, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITimer::OnLoop () {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( EVENT_TIMER_LOOP, state )) {
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITimer::OnStart () {

	if( this->mDirection > 0.0f ) {
		this->mTime = this->mStartTime;
	}
	else {
		this->mTime = this->mEndTime;
	}
	this->mCycle = 0.0f;
	this->mTimesExecuted = 0.0f;
	
	this->OnBeginSpan ();
}

//----------------------------------------------------------------//
void MOAITimer::OnUpdate ( float step ) {

	this->DoStep ( step );
}

//----------------------------------------------------------------//
void MOAITimer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
	MOAIAction::RegisterLuaClass ( state );

	state.SetField ( -1, "ATTR_TIME", MOAITimerAttr::Pack ( ATTR_TIME ));
	
	state.SetField ( -1, "EVENT_TIMER_KEYFRAME",	( u32 )EVENT_TIMER_KEYFRAME );
	state.SetField ( -1, "EVENT_TIMER_LOOP",		( u32 )EVENT_TIMER_LOOP );
	state.SetField ( -1, "EVENT_TIMER_BEGIN_SPAN",	( u32 )EVENT_TIMER_BEGIN_SPAN );
	state.SetField ( -1, "EVENT_TIMER_END_SPAN",	( u32 )EVENT_TIMER_END_SPAN );
	
	state.SetField ( -1, "NORMAL",					( u32 )NORMAL );
	state.SetField ( -1, "REVERSE",					( u32 )REVERSE );
	state.SetField ( -1, "CONTINUE",				( u32 )CONTINUE );
	state.SetField ( -1, "CONTINUE_REVERSE",		( u32 )CONTINUE_REVERSE );
	state.SetField ( -1, "LOOP",					( u32 )LOOP );
	state.SetField ( -1, "LOOP_REVERSE",			( u32 )LOOP_REVERSE );
	state.SetField ( -1, "PING_PONG",				( u32 )PING_PONG );
}

//----------------------------------------------------------------//
void MOAITimer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getTime",			_getTime },
		{ "getTimesExecuted",	_getTimesExecuted },
		{ "setCurve",			_setCurve },
		{ "setMode",			_setMode },
		{ "setSpan",			_setSpan },
		{ "setSpeed",			_setSpeed },
		{ "setTime",			_setTime },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITimer::SetSpan ( float span ) {

	this->mStartTime = 0.0f;
	this->mEndTime = span;
}

//----------------------------------------------------------------//
void MOAITimer::SetSpan ( float startTime, float endTime ) {

	this->mStartTime = startTime;
	this->mEndTime = endTime;
}

//----------------------------------------------------------------//
void MOAITimer::SetTime ( float time ) {

	float length = USFloat::Abs ( this->mEndTime - this->mStartTime );

	float transformedTime = ( time - this->mStartTime ) / length;

	switch ( this->mMode ) {
		case NORMAL: 
		case REVERSE: {
			time = USFloat::Clamp ( time, this->mStartTime, this->mEndTime );
			this->mTime = time;
			break;
		}
		case CONTINUE:
		case CONTINUE_REVERSE: {
			float wrappedT = transformedTime - USFloat::Floor ( transformedTime );
			this->mTime = wrappedT * length + this->mStartTime;
			this->mCycle = USFloat::Floor ( transformedTime );
			break;
		}
		case LOOP:
		case LOOP_REVERSE: {
			float wrappedT = transformedTime - USFloat::Floor ( transformedTime );
			this->mTime = wrappedT * length + this->mStartTime;
			this->mCycle = 0.0f;
			break;
		}
		case PING_PONG: {
			float wrappedT = transformedTime - USFloat::Floor ( transformedTime );
			this->mTime = wrappedT * length + this->mStartTime;
			if ( ( u32 ) USFloat::Floor ( transformedTime ) % 2 ) { //switch direction if odd
				this->mDirection = -1.0f;
			}
			else {
				this->mDirection = 1.0f;
			}
			this->mCycle = 0.0f;
			break;
		}
	}

	if ( this->mTime + EPSILON > time && this->mTime - EPSILON < time ) { 
		this->mTime = time; 
	}

	this->ScheduleUpdate ();
}
