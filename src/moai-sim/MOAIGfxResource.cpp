// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// MOAIGfxState
//================================================================//

//----------------------------------------------------------------//
MOAIGfxState::MOAIGfxState () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGfxState::~MOAIGfxState () {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getAge
	@text	Returns the 'age' of the graphics resource. The age
			is the number of times MOAIRenderMgr has rendered a scene
			since the resource was last bound. It is part of the
			render count, not a timestamp. This may change to be
			time-based in future releases.
 
	@in		MOAIGfxResource self
	@out	number age
*/
int MOAIGfxResource::_getAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	u32 age = MOAIRenderMgr::Get ().GetRenderCounter () - self->mLastRenderCount;
	lua_pushnumber ( state, age );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	purge
	@text	Attempt to release the resource. Generally this is used when
			responding to a memory warning from the system. A resource
			will only be released if it is renewable (i.e. has a renew
			callback or contains all information needed to reload the
			resources on demand).
			
			Using soft release can save an app in extreme memory
			circumstances, but may trigger reloads of resources during
			runtime which can significantly degrade performance.
 
	@in		MOAIGfxResource self
	@opt	number age				Release only if the texture hasn't been used in X frames.
	@out	boolean released		True if the texture was actually released.
*/
int MOAIGfxResource::_purge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean ( L, self->Purge ( age ));

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResource::_setLoadingPolicy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	self->mLoadingPolicy = state.GetValue < u32 >( 1, LOADING_POLICY_NONE );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResource::_setReloader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	self->mReloader.SetRef ( *self, state, 2 );
	self->InvokeLoader ();
	return 0;
}

//================================================================//
// MOAIGfxResource
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResource::Bind () {

	if ( this->PrepareForBind ()) {
		this->OnGPUBind ();
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResource::Destroy () {

	if ( MOAIGfxResourceMgr::IsValid ()) {
		this->OnGPUDestroy ();
	}
	this->OnGPULost ();
	this->OnCPUDestroy ();
	this->mState = STATE_NEW;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::DoCPUAffirm () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_NEW )) return false;

	u32 loadingPolicy = this->GetLoadingPolicy ();
	
	// if we're deferring both CPU and GPU, bail (unless we're being forced to load the CPU)
	if ( loadingPolicy == LOADING_POLICY_CPU_GPU_BIND ) return true;

	// whether or not GPU is deferred, do the CPU part
	if ( this->mState == STATE_NEEDS_CPU_CREATE ) {
		this->mState = this->OnCPUCreate () ? STATE_NEEDS_GPU_CREATE : STATE_ERROR;
	}
	
	// turns out we want to do the GPU piece ASAP as well
	if ( this->mState == STATE_NEEDS_GPU_CREATE ) {
	
		if ( loadingPolicy == LOADING_POLICY_CPU_ASAP_GPU_NEXT ) {
	
			MOAIGfxResourceMgr::Get ().ScheduleGPUAffirm ( *this );
		}
		else if ( loadingPolicy == LOADING_POLICY_CPU_GPU_ASAP ) {
		
			#if MOAI_USE_GFX_THREAD
			
				MOAIGfxResourceMgr::Get ().ScheduleGPUAffirm ( *this );
			
			#else
			
				zglBegin ();
				this->mState = this->OnGPUCreate () ? STATE_READY_TO_BIND : STATE_ERROR;
				if ( this->mState == STATE_READY_TO_BIND ) {
					this->OnCPUDestroy ();
				}
				zglEnd ();
			
			#endif
		
		}
	}
	
	return this->mState != STATE_ERROR;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::DoGPUAffirm () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_NEW )) return false;

	// if we get here, load both CPU and GPU parts

	if ( this->mState == STATE_NEEDS_CPU_CREATE ) {
		this->mState = this->OnCPUCreate () ? STATE_NEEDS_GPU_CREATE : STATE_ERROR;
	}

	if ( this->mState == STATE_NEEDS_GPU_CREATE ) {
		this->mState = this->OnGPUCreate () ? STATE_READY_TO_BIND : STATE_ERROR;
		if ( this->mState == STATE_READY_TO_BIND ) {
			this->OnCPUDestroy ();
		}
	}
	return this->mState == STATE_READY_TO_BIND;
}

//----------------------------------------------------------------//
void MOAIGfxResource::ForceCPUCreate () {

	if ( this->mState == STATE_NEEDS_CPU_CREATE ) {
		this->mState = this->OnCPUCreate () ? STATE_NEEDS_GPU_CREATE : STATE_ERROR;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResource::FinishInit () {

	if (( this->mState == STATE_NEW ) || ( this->mState == STATE_ERROR )) {
		this->mState = STATE_NEEDS_CPU_CREATE;
	}
}

//----------------------------------------------------------------//
u32 MOAIGfxResource::GetLoadingPolicy () {

	if ( this->mLoadingPolicy == LOADING_POLICY_NONE ) {
		u32 globalLoadingPolicy = MOAIGfxResourceMgr::Get ().mResourceLoadingPolicy;
		return globalLoadingPolicy == LOADING_POLICY_NONE ? DEFAULT_LOADING_POLICY : globalLoadingPolicy;
	}
	return this->mLoadingPolicy;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::HasReloader () {

	return ( bool )this->mReloader;
}

//----------------------------------------------------------------//
void MOAIGfxResource::InvokeLoader () {

	if ( this->mReloader ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mReloader.PushRef ( state )) {
			state.DebugCall ( 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
MOAIGfxResource::MOAIGfxResource () :
	mState ( STATE_NEW ),
	mLastRenderCount ( 0 ),
	mLoadingPolicy ( LOADING_POLICY_NONE ) {

	RTTI_SINGLE ( MOAIGfxState )

	this->mLink.Data ( this );
	
	MOAIGfxResourceMgr::Get ().InsertGfxResource ( *this );
}

//----------------------------------------------------------------//
MOAIGfxResource::~MOAIGfxResource () {

	if ( MOAIGfxResourceMgr::IsValid ()) {
		MOAIGfxResourceMgr::Get ().RemoveGfxResource ( *this );
	}
	this->mReloader.Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxResource::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
	
	state.SetField ( -1, "LOADING_POLICY_NONE",					( u32 )LOADING_POLICY_NONE );
	state.SetField ( -1, "LOADING_POLICY_CPU_GPU_ASAP",			( u32 )LOADING_POLICY_CPU_GPU_ASAP );
	state.SetField ( -1, "LOADING_POLICY_CPU_ASAP_GPU_NEXT",	( u32 )LOADING_POLICY_CPU_ASAP_GPU_NEXT );
	state.SetField ( -1, "LOADING_POLICY_CPU_ASAP_GPU_BIND",	( u32 )LOADING_POLICY_CPU_ASAP_GPU_BIND );
	state.SetField ( -1, "LOADING_POLICY_CPU_GPU_BIND",			( u32 )LOADING_POLICY_CPU_GPU_BIND );
}

//----------------------------------------------------------------//
void MOAIGfxResource::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getAge",					_getAge },
		{ "purge",					_purge },
		{ "softRelease",			_purge }, // back compat
		{ "setLoadingPolicy",		_setLoadingPolicy },
		{ "setReloader",			_setReloader },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIGfxResource::PrepareForBind () {

	if (( this->mState == STATE_NEW ) || ( this->mState == STATE_ERROR )) return false;

	if ( !MOAIGfxDevice::Get ().GetHasContext ()) {
		MOAILog ( 0, MOAILogMessages::MOAIGfxResource_MissingDevice );
		return false;
	}

	if ( this->mState == STATE_NEEDS_CPU_CREATE ) {
		this->InvokeLoader ();
	}

	if ( this->DoGPUAffirm ()) {
		this->mLastRenderCount = MOAIRenderMgr::Get ().GetRenderCounter ();
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::Purge ( u32 age ) {

	if ( this->mState == STATE_ERROR ) return false;

	age = MOAIRenderMgr::Get ().GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		this->OnCPUDestroy ();
		this->OnGPUDestroy ();
		this->mState = STATE_NEEDS_CPU_CREATE;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResource::Renew () {

	// any state other than error we go back to square zero
	if ( this->mState != STATE_ERROR ) {
		this->OnGPULost (); // clear out the resource id (if any)
		this->mState = STATE_NEEDS_CPU_CREATE;
		this->InvokeLoader ();
		this->DoGPUAffirm ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxResource::Unbind () {

	this->OnGPUUnbind ();
}
