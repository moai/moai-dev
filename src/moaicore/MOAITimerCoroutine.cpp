//
//  MOAITimerCoroutine.cpp
//  libmoai
//
//  Created by Alexey Chernikov on 4/18/13.
//
//

#include "pch.h"
#include <aku/AKU.h>
#include <moaicore/MOAIActionMgr.h>

#include "MOAITimerCoroutine.h"

//----------------------------------------------------------------//
MOAITimerCoroutine::MOAITimerCoroutine () :
	mState ( 0 ),
	mNarg ( 0 ),
	mIsFirstRun ( true ),
	mStartTime( 0.0f ),
	mEndTime( 0.0f )
{
	RTTI_BEGIN
//		RTTI_EXTEND ( MOAINode )
//		RTTI_EXTEND ( MOAIAction )
		RTTI_EXTEND( MOAITimer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITimerCoroutine::~MOAITimerCoroutine () {
}

//----------------------------------------------------------------//
void MOAITimerCoroutine::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "blockOnAction",		_blockOnAction },
		{ "currentThread",		_currentThread },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
/**	@name	blockOnAction
 @text	Skip updating current thread until the specified action is
 no longer busy. A little more efficient that spinlocking from
 Lua.
 
 @in		MOAIAction blocker
 @out	nil
 */
int MOAITimerCoroutine::_blockOnAction ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIAction* current = MOAIActionMgr::Get ().GetCurrentAction ();
	if ( !current ) return 0;
	
	MOAIAction* blocker = state.GetLuaObject < MOAIAction >( 1, true );
	if ( !blocker || !blocker->IsBusy ()) return 0;
	
	current->SetBlocker ( blocker );
	
	return lua_yield ( state, 0 );
}

//----------------------------------------------------------------//
/**	@name	currentThread
 @text	Returns the currently running thread (if any).
 
 @out	MOAICoroutine currentThread	Current thread or nil.
 */
int MOAITimerCoroutine::_currentThread ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAIAction* current = MOAIActionMgr::Get ().GetCurrentAction ();
	if ( !current ) return 0;
	
	current->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	run
 @text	Starts a thread with a function and passes parameters to it.
 
 @in		MOAICoroutine self
 @in		function threadFunc
 @in		...
 @out	nil
 */
int MOAITimerCoroutine::_run ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimerCoroutine, "UF" )
	
	if ( !MOAIActionMgr::IsValid ()) return 0;
	
	if ( MOAIActionMgr::Get ().GetThreadInfoEnabled ()) {
		
		// Get a copy of the function's debug info and store it so we can
		// refer to it in any debugging info regarding this thread.
		lua_Debug ar;
		lua_pushvalue ( state, 2 );
		lua_getinfo ( state, ">Snl", &ar );
		
		bool isC = strcmp ( ar.what, "C" ) == 0;
		
		if ( !ar.what ) {
			ar.what = "??";
		}
		
		if ( !ar.source ) {
			if ( isC ) {
				ar.source = "@?";
			}
			else {
				ar.source = "@<string>";
			}
		}
		
		self->mFuncName.clear ();
		if ( ar.name ) {
			self->mFuncName.write ( "%s:%s%s:%d", ar.what, ar.name, ar.source, ar.linedefined );
		}
		else {
			self->mFuncName.write ( "%s:%s:%d", ar.what, ar.source, ar.linedefined );
		}
	}
	
	self->mNarg = lua_gettop ( state ) - 2;
	self->mState = lua_newthread ( state );
	self->SetLocal ( state, -1, self->mRef );
	lua_pop ( state, 1 );
	
	lua_xmove ( state, self->mState, self->mNarg + 1 );
	
	self->Start ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCurve
 @text	Set or clear the curve to use for event generation.
 
 @in		MOAITimer self
 @opt	MOAIAnimCurve curve		Default value is nil.
 @out	nil
 */
int MOAITimerCoroutine::_setCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimerCoroutine, "U" );
	
	self->mCurve.Set ( *self, state.GetLuaObject < MOAIAnimCurve >( 2, true ));
	self->ScheduleUpdate ();
	
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
int MOAITimerCoroutine::_setSpan ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimerCoroutine, "UN" )
	
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
/**	@name	setTime
 @text	Manually set the current time. This will be wrapped
 into the current span.
 
 @in		MOAITimer self
 @opt	number time			Default value is 0.
 @out	nil
 */
int MOAITimerCoroutine::_setTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITimerCoroutine, "U" )
	
	float time = state.GetValue < float >( 2, 0.0f );
	self->SetTime ( time );
	
	return 0;
}
//----------------------------------------------------------------//
void MOAITimerCoroutine::SetSpan ( float span ) {
	
	this->mStartTime = 0.0f;
	this->mEndTime = span;
}

//----------------------------------------------------------------//
void MOAITimerCoroutine::SetSpan ( float startTime, float endTime ) {
	
	this->mStartTime = startTime;
	this->mEndTime = endTime;
}
////----------------------------------------------------------------//
void MOAITimerCoroutine::GenerateKeyframeCallbacks ( float t0, float t1, bool end ) {

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
void MOAITimerCoroutine::OnKeyframe ( u32 idx, float time, float value ) {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
}


//----------------------------------------------------------------//
void MOAITimerCoroutine::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "run",			_run},
		{ "setSpan",		_setSpan},
		{ "setTime",		_setTime},
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}




bool MOAITimerCoroutine::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	
	if ( MOAITimerAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );
		
		if ( attrID == ATTR_TIME ) {
			this->mTime = attrOp.Apply ( this->mTime, op, MOAIAttrOp::ATTR_READ_WRITE );

			//float newTime = attrOp.Apply ( this->GetTime (), op, MOAIAttrOp::ATTR_READ_WRITE ); // changed from ATTR_READ
			/*
			 if (op & MOAIAttrOp::ATTR_WRITE) {
			 // reverse changes maded with GetTime()
			 if ( ( this->mMode == CONTINUE ) || ( this->mMode == CONTINUE_REVERSE )) {
			 float length = this->mEndTime - this->mStartTime;
			 this->mTime = newTime - (length * this->mCycle);
			 }
			 else{
			 this->mTime = newTime;
			 }
			 }
			 */
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------//
void MOAITimerCoroutine::OnUpdate ( float step ) {
	//UNUSED ( step );
	float t0 = this->mTime;
	this->mTime += step * this->mSpeed * this->mDirection;
	this->GenerateKeyframeCallbacks ( t0, this->mTime, false );
	
	if ( this->mState ) {
		
		int result;
		
		if ( this->mIsFirstRun ) {
			result = lua_resume ( this->mState, this->mNarg );
			this->mNarg = 0;
			this->mIsFirstRun = false;
		}
		else {
			// Pass the step value as the return result from coroutine.yield ()
			lua_pushnumber ( this->mState, step );
			result = lua_resume ( this->mState, 1 );
		}
		
		if ( this->IsActive ()) {
			if (( result != LUA_YIELD )) {
				
				if ( result != 0 ) {
					
					cc8* msg = lua_tostring ( this->mState, -1 );
		            AKUErrorTracebackFunc errorTraceback = AKUGetFunc_ErrorTraceback ();
		            if ( errorTraceback ) {
			            errorTraceback ( msg, this->mState, 0 );
		            }
					
					if ( MOAILuaRuntime::Get ().GetCustomTraceback ()) {
						
						MOAILuaState state ( this->mState );
						state.Push ( MOAILuaRuntime::Get ().GetCustomTraceback ());
						state.Push ( msg );
						
						state.DebugCall ( 1, 0 );
					}
					
					lua_pop ( this->mState, 1 );
				}
				this->Stop ();
			}
		}
		else {
			this->ClearLocal ( this->mRef );
			this->mState = 0;
		}
	}
}

//----------------------------------------------------------------//
bool MOAITimerCoroutine::IsDone () {
	
	return ( this->mRef == false );
}

//----------------------------------------------------------------//
void MOAITimerCoroutine::SetTime ( float time ) {
	
	time = USFloat::Clamp ( time, this->mStartTime, this->mEndTime );
	this->mTime = time;

	
	if ( this->mTime + EPSILON > time && this->mTime - EPSILON < time ) {
		this->mTime = time;
	}
	
	this->ScheduleUpdate ();
}
