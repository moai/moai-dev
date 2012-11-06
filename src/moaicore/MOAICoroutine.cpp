// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <aku/AKU.h>
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAICoroutine.h>

//----------------------------------------------------------------//
/**	@name	blockOnAction
	@text	Skip updating current thread until the specified action is
			no longer busy. A little more efficient that spinlocking from
			Lua.

	@in		MOAIAction blocker
	@out	nil
*/
int MOAICoroutine::_blockOnAction ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;

	MOAIAction* current = MOAIActionMgr::Get ().GetCurrentAction ();
	if ( !current ) return 0;
	
	MOAIAction* blocker = state.GetLuaObject < MOAIAction >( 1, true );
	if ( !blocker ) return 0;
	
	current->SetBlocker ( blocker );
	
	return lua_yield ( state, 0 );
}

//----------------------------------------------------------------//
/**	@name	currentThread
	@text	Returns the currently running thread (if any).
	
	@out	MOAICoroutine currentThread	Current thread or nil.
*/
int MOAICoroutine::_currentThread ( lua_State* L ) {
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
int MOAICoroutine::_run ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "UF" )

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

//================================================================//
// MOAICoroutine
//================================================================//

//----------------------------------------------------------------//
bool MOAICoroutine::IsDone () {

	return ( this->mRef == false );
}

//----------------------------------------------------------------//
STLString MOAICoroutine::GetDebugInfo () const {
	return mFuncName;
}

//----------------------------------------------------------------//
MOAICoroutine::MOAICoroutine () :
	mState ( 0 ),
	mNarg ( 0 ),
	mIsFirstRun ( true ) {

	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAICoroutine::~MOAICoroutine () {
}

//----------------------------------------------------------------//
void MOAICoroutine::OnUpdate ( float step ) {
	UNUSED ( step );
	
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
void MOAICoroutine::OnStop () {
	MOAIAction::OnStop ();
	
	// if we're stopping the thread from outside of its coroutine, clear out the ref
	if ( !this->IsCurrent ()) {
		this->ClearLocal ( this->mRef );
		this->mState = 0;
	}
}

//----------------------------------------------------------------//
void MOAICoroutine::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "blockOnAction",		_blockOnAction },
		{ "currentThread",		_currentThread },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICoroutine::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "run",			_run },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
	
	lua_getglobal ( state, "coroutine" );
	
	lua_getfield ( state, -1, "create" );
	lua_setfield ( state, -3, "create" );
	
	lua_getfield ( state, -1, "resume" );
	lua_setfield ( state, -3, "resume" );
	
	lua_pop ( state, 1 );
}
