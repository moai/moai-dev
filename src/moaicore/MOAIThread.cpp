// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIThread.h>

//----------------------------------------------------------------//
/**	@name	blockOnAction
	@text	Skip updating current thread until the specified action is
			no longer busy. A little more efficient that spinlocking from
			Lua.

	@in		MOAIAction blocker
	@out	nil
*/
int MOAIThread::_blockOnAction ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;

	MOAIAction* current = MOAIActionMgr::Get ().GetCurrentAction ();
	if ( !current ) return 0;
	
	MOAIAction* blocker = state.GetLuaObject < MOAIAction >( 1 );
	if ( !blocker ) return 0;
	
	current->SetBlocker ( blocker );
	
	return lua_yield ( state, 0 );
}

//----------------------------------------------------------------//
/**	@name	currentThread
	@text	Returns the currently running thread (if any).
	
	@out	MOAIThread currentThread	Current thread or nil.
*/
int MOAIThread::_currentThread ( lua_State* L ) {
	USLuaState state ( L );

	MOAIAction* current = MOAIActionMgr::Get ().GetCurrentAction ();
	if ( !current ) return 0;
	
	current->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>run ( self, func )</tt>\n
\n
	Sets the thread's function and starts it.
	@param self (in)
	@param func (in) Function for this thread to run.
*/

/**	@name	run
	@text	Starts a thread with a function and passes parameters to it.
	
	@in		MOAIThread self
	@in		function threadFunc
	@in		...
	@out	nil
*/
int MOAIThread::_run ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIThread, "UF" )
	
	self->mNarg = lua_gettop ( state ) - 2;
	self->mState = lua_newthread ( state );
	self->mRef.SetRef ( state, -1, false );
	lua_pop ( state, 1 );
	
	lua_xmove ( state, self->mState, self->mNarg + 1 );
	
	self->Start ();

	return 0;
}

//================================================================//
// MOAIThread
//================================================================//

//----------------------------------------------------------------//
MOAIThread::MOAIThread () :
	mState ( 0 ),
	mNarg ( 0 ) {

	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIThread::~MOAIThread () {
}

//----------------------------------------------------------------//
bool MOAIThread::IsDone () {

	return this->mRef.IsNil ();
}

//----------------------------------------------------------------//
void MOAIThread::OnUpdate ( float step ) {
	UNUSED ( step );
	
	if ( this->mState ) {
		
		int result = lua_resume ( this->mState, this->mNarg );
		this->mNarg = 0;
		
		if ( result != LUA_YIELD ) {
		
			if ( result != 0 ) {
				
				cc8* msg = lua_tostring ( this->mState, -1 );
				USLog::Print ( "%s\n", msg );
				lua_pop ( this->mState, 1 );
				
				USLuaStateHandle state ( this->mState );
				state.PrintStackTrace ( 0 );
			}
		
			this->mRef.Clear ();
			this->mState = 0;
			this->Stop ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIThread::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "blockOnAction",		_blockOnAction },
		{ "currentThread",		_currentThread },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIThread::RegisterLuaFuncs ( USLuaState& state ) {

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

