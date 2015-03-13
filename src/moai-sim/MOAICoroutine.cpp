// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICoroutine.h>
#include <moai-sim/MOAISim.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	blockOnAction
	@text	Skip updating current thread until the specified action is
			no longer busy. A little more efficient than spinlocking from
			Lua.

	@in		MOAIAction blocker
	@out	nil
*/
int MOAICoroutine::_blockOnAction ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;

	MOAIAction* current = MOAIActionStackMgr::Get ().GetCurrent ();
	if ( !current ) return 0;
	
	MOAIAction* blocker = state.GetLuaObject < MOAIAction >( 1, true );
	if ( !blocker || !blocker->IsBusy ()) return 0;
	
	current->SetBlocker ( blocker );
	
	return lua_yield ( state, 0 );
}

//----------------------------------------------------------------//
/**	@lua	currentThread
	@text	Returns the currently running thread (if any).
	
	@out	MOAICoroutine currentThread	Current thread or nil.
*/
int MOAICoroutine::_currentThread ( lua_State* L ) {
	MOAILuaState state ( L );

	MOAIAction* current = MOAIActionStackMgr::Get ().GetCurrent ();
	if ( !current ) return 0;
	
	current->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICoroutine::_getHistogram ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" )
	
	MOAILuaRuntime::Get ().PushHistogram ( state, self->mTrackingGroup );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICoroutine::_getTrackingGroup ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" )
	
	state.Push ( self->mTrackingGroup );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICoroutine::_reportHistogram ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" )

	cc8* filename = state.GetValue < cc8* >( 1, 0 );
	MOAILuaRuntime::Get ().ReportHistogram ( filename, self->mTrackingGroup );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICoroutine::_reportLeaks ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" )
	
	cc8* filename = state.GetValue < cc8* >( 1, 0 );
	MOAILuaRuntime::Get ().ReportLeaksFormatted ( filename, self->mTrackingGroup );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	run
	@text	Starts a thread with a function and passes parameters to it.
	
	@in		MOAICoroutine self
	@in		function threadFunc
	@in		... parameters
	@out	nil
*/
int MOAICoroutine::_run ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" )

	if ( !MOAISim::IsValid ()) return 0;

	bool defer = false;
	int baseParam = 2;
	if ( state.IsType ( baseParam, LUA_TBOOLEAN )) {
		defer = state.GetValue < bool >( baseParam++, false );
	}

	if ( MOAISim::Get ().GetActionMgr ().GetThreadInfoEnabled ()) {

		// Get a copy of the function's debug info and store it so we can
		// refer to it in any debugging info regarding this thread.
		lua_Debug ar;
		lua_pushvalue ( state, baseParam );
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

	self->mNarg = lua_gettop ( state ) - baseParam;
	self->mState = lua_newthread ( state );
	self->mRef.SetRef ( *self, state, -1 );
	lua_pop ( state, 1 );
	
	lua_xmove ( state, self->mState, self->mNarg + 1 );
	
	self->Start ( MOAISim::Get ().GetActionMgr (), defer );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICoroutine::_setDefaultParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" );
	
	self->mIsDefaultParent = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICoroutine::_setTrackingGroup ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" );
	
	self->mTrackingGroup = state.GetValue < cc8* >( 2, "" );
	
	if ( MOAIActionStackMgr::Get ().GetCurrent () == self ) {
		MOAILuaRuntime::Get ().SetTrackingGroup ( self->mTrackingGroup );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICoroutine::_step ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICoroutine, "U" )

	self->mNarg = lua_gettop ( state ) - 1;
	if ( self->mNarg > 0 ) {
		lua_xmove ( state, self->mState, self->mNarg );
	}
	return self->Resume ( 0.0f );
}

//================================================================//
// MOAICoroutine
//================================================================//

//----------------------------------------------------------------//
STLString MOAICoroutine::GetDebugInfo () const {
	return mFuncName;
}

//----------------------------------------------------------------//
MOAIAction* MOAICoroutine::GetDefaultParent () {

	return this->mIsDefaultParent ? this : 0;
}

//----------------------------------------------------------------//
bool MOAICoroutine::IsDone () {

	return ( this->mRef == false );
}

//----------------------------------------------------------------//
MOAICoroutine::MOAICoroutine () :
	mState ( 0 ),
	mNarg ( 0 ),
	mIsUpdating ( false ),
	mIsActive ( false ),
	mIsDefaultParent ( true ) {

	RTTI_SINGLE ( MOAIAction )
	
	this->mTrackingGroup.write ( "MOAICoroutine: 0x%p", this );
}

//----------------------------------------------------------------//
MOAICoroutine::~MOAICoroutine () {
}

//----------------------------------------------------------------//
void MOAICoroutine::OnStart () {
	MOAIAction::OnStart ();
}

//----------------------------------------------------------------//
void MOAICoroutine::OnStop () {
	MOAIAction::OnStop ();
	
	// if we're stopping the thread from outside of its coroutine, clear out the ref
	if ( MOAIActionStackMgr::IsValid ()) {
		if ( MOAIActionStackMgr::Get ().GetCurrent () != this ) {
			this->mRef.Clear ();
			this->mState = 0;
		}
	}
}

//----------------------------------------------------------------//
void MOAICoroutine::OnUpdate ( double step ) {
	this->Resume ( step );
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
		{ "getHistogram",			_getHistogram },
		{ "getTrackingGroup",		_getTrackingGroup },
		{ "reportHistogram",		_reportHistogram },
		{ "reportLeaks",			_reportLeaks },
		{ "run",					_run },
		{ "setDefaultParent",		_setDefaultParent },
		{ "setTrackingGroup",		_setTrackingGroup },
		{ "step",					_step },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
	
	// ?
	
	lua_getglobal ( state, "coroutine" );
	
	lua_getfield ( state, -1, "create" );
	lua_setfield ( state, -3, "create" );
	
	lua_getfield ( state, -1, "resume" );
	lua_setfield ( state, -3, "resume" );
	
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
int MOAICoroutine::Resume ( float step ) {
	UNUSED ( step );
	
	int returnCount = 0;
	
	MOAILuaRuntime::Get ().SetTrackingGroup ( this->mTrackingGroup );
	
	if ( this->mState ) {
		
		int result;
		
		int narg = this->mNarg;
		this->mNarg = 0;
		
		if ( narg == 0 ) {
			lua_pushnumber ( this->mState, step );
			narg = 1;
		}
		
		MOAIActionStackMgr& coroutineMgr = MOAIActionStackMgr::Get ();
		coroutineMgr.Push ( *this );
		
		#if LUA_VERSION_NUM < 502
			result = lua_resume ( this->mState, narg );
		#else
			result = lua_resume ( this->mState, NULL, narg );
		#endif
		
		coroutineMgr.Pop ();
		
		if (( result != LUA_YIELD )) {
		
			if ( result != 0 ) {

				cc8* msg = lua_tostring ( this->mState, -1 );
				MOAILuaState state ( this->mState );

				#if (MOAI_WITH_LUAJIT)
					//luajit has assertions on lua_call if the thread has crashed due to runtime error
					//this means we can't run our custom stacktrace using this state. we will just bail instead
					if ( msg ) {
						ZLLog::LogF ( ZLLog::CONSOLE, "%s\n", msg );
					}
					state.PrintStackTrace ( ZLLog::CONSOLE, NULL, 0 );
				#else
					MOAILuaRuntime::Get ().PushTraceback ( state );
					state.Push ( msg );
					lua_call ( this->mState, 1, 0 );
					lua_pop ( this->mState, 1 );
				#endif
			}
			this->Stop ();
			this->mRef.Clear ();
			this->mState = 0;
		}
		else {
			returnCount = lua_gettop ( this->mState );
		}
	}
	
	MOAILuaRuntime::Get ().SetTrackingGroup ();
	
	return returnCount;
}
