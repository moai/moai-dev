// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIRenderMgr.h>

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
// TODO: doxygen
int MOAIGfxResource::_getResourceState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	state.Push ( self->mState );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResource::_preload ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	return 0;
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
int MOAIGfxResource::_scheduleForGPUCreate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	u32 listID = state.GetValue < u32 >( 2, MOAIGfxPipelineClerk::DRAWING_PIPELINE );
	self->ScheduleForGPUCreate ( listID );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setReloader
	@text	The reloaded is called prior to recreating the resource. It should
			in turn call the resources regular load or init methods.
 
	@in		MOAIGfxResource self
	@opt	function reloader
	@out	nil
*/
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
void MOAIGfxResource::Affirm () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->DoGPUUpdate ();
	}
	else {
		this->InvokeLoader ();
		this->DoGPUCreate ();
	}
}

//----------------------------------------------------------------//
u32 MOAIGfxResource::Bind () {

//	if ( !MOAIGfxMgr::Get ().GetHasContext ()) {
//		MOAILog ( 0, MOAILogMessages::MOAIGfxResource_MissingDevice );
//		return false;
//	}

	// we're ready to bind, so do it
	if ( this->mState == STATE_READY_TO_BIND ) {
		this->mLastRenderCount = MOAIRenderMgr::Get ().GetRenderCounter ();
		this->OnGPUBind ();
	}
	return this->mState;
}

//----------------------------------------------------------------//
void MOAIGfxResource::Destroy () {

	this->OnGPUDeleteOrDiscard ( true );
	this->OnCPUDestroy ();
	this->mState = STATE_UNINITIALIZED;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::DoCPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// whether or not GPU is deferred, do the CPU part
	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}
	return this->mState != STATE_ERROR;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::DoGPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// if we get here, load both CPU and GPU parts

	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}

	if ( this->mState == STATE_READY_FOR_GPU_CREATE ) {
	
		// set this now; in immediate mode we'll go straight to STATE_READY_TO_BIND on success
		// otherwise we'll go there later when we get GFX_EVENT_CREATED
		// why can't we set STATE_READY_TO_BIND? becausein multi-threaded mode, it might be
		// a few more frames before we get GFX_EVENT_CREATED
		
		this->mState = STATE_PENDING;
	
		if ( this->OnGPUCreate ()) {
			MOAIGfxMgr::GetDrawingAPI ().Event ( this, GFX_EVENT_CREATED, 0 );
			this->OnCPUDestroy ();
		}
		else {
			this->mState = STATE_ERROR;
		}
	}
	return this->mState == STATE_READY_TO_BIND;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::DoGPUUpdate () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->mState = this->OnGPUUpdate () ? STATE_READY_TO_BIND : STATE_ERROR;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxResource::FinishInit () {

	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) {
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->ScheduleForGPUCreate ( MOAIGfxPipelineClerk::DRAWING_PIPELINE );
	}
	else {
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
bool MOAIGfxResource::HasReloader () {

	return ( bool )this->mReloader;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::InvokeLoader () {

	if ( this->mReloader ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mReloader.PushRef ( state )) {
			state.DebugCall ( 0, 0 );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
MOAIGfxResource::MOAIGfxResource () :
	mState ( STATE_UNINITIALIZED ),
	mLastRenderCount ( 0 ) {

	RTTI_SINGLE ( MOAIInstanceEventSource )

	this->mMasterLink.Data ( this );
	this->mPendingLink.Data ( this );
	
	MOAIGfxMgr::Get ().mResourceMgr.InsertGfxResource ( *this );
}

//----------------------------------------------------------------//
MOAIGfxResource::~MOAIGfxResource () {

	if ( MOAIGfxMgr::IsValid ()) {
		MOAIGfxMgr::Get ().mResourceMgr.RemoveGfxResource ( *this );
	}
	this->mReloader.Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxResource::OnClearDirty () {
}

//----------------------------------------------------------------//
void MOAIGfxResource::OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( userdata );

	if ( event == GFX_EVENT_CREATED ) {
	
		// we should only get this event if the creation sequence was successful
		this->mState = STATE_READY_TO_BIND;
		
		// let Lua know the resource is ready for use
		this->InvokeListener ( GFX_EVENT_CREATED );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxResource::Purge ( u32 age ) {

	if ( this->mState == STATE_ERROR ) return false;

	age = MOAIRenderMgr::Get ().GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		this->OnCPUDestroy ();
		this->OnGPUDeleteOrDiscard ( true );
		this->mState = STATE_READY_FOR_CPU_CREATE;
		
		this->ScheduleForGPUCreate ( MOAIGfxPipelineClerk::DRAWING_PIPELINE );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResource::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIInstanceEventSource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "STATE_UNINITIALIZED",					( u32 )STATE_UNINITIALIZED );
	state.SetField ( -1, "STATE_READY_FOR_CPU_CREATE",			( u32 )STATE_READY_FOR_CPU_CREATE );
	state.SetField ( -1, "STATE_READY_FOR_GPU_CREATE",			( u32 )STATE_READY_FOR_GPU_CREATE );
	state.SetField ( -1, "STATE_READY_TO_BIND",					( u32 )STATE_READY_TO_BIND );
	state.SetField ( -1, "STATE_ERROR",							( u32 )STATE_ERROR );
	
	state.SetField ( -1, "GFX_EVENT_CREATED",					( u32 )GFX_EVENT_CREATED );
	
	state.SetField ( -1, "DRAWING_PIPELINE",					( u32 )MOAIGfxPipelineClerk::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",					( u32 )MOAIGfxPipelineClerk::LOADING_PIPELINE );
}

//----------------------------------------------------------------//
void MOAIGfxResource::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getAge",					_getAge },
		{ "getResourceState",		_getResourceState },
		{ "preload",				_preload },
		{ "purge",					_purge },
		{ "softRelease",			_purge }, // back compat
		{ "scheduleForGPUCreate",	_scheduleForGPUCreate },
		{ "setReloader",			_setReloader },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxResource::Renew () {

	// any (valid) state other than error we go back to square zero
	if ( !(( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR ))) {
	
		this->OnGPUDeleteOrDiscard ( false ); // clear out the resource id (if any)
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->InvokeLoader ();
		this->DoGPUCreate ();
	}
}

//----------------------------------------------------------------//
bool MOAIGfxResource::ScheduleForGPUCreate ( u32 pipelineID ) {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) return false;
	
	if ( MOAIGfxMgr::IsValid ()) {
		MOAIGfxMgr::Get ().mResourceMgr.ScheduleGPUAffirm ( *this, pipelineID );
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::ScheduleForGPUUpdate () {

	if ( this->mState != STATE_READY_TO_BIND ) return false;

	this->mState = STATE_NEEDS_GPU_UPDATE;

	if ( MOAIGfxMgr::IsValid ()) {
		MOAIGfxMgr::Get ().mResourceMgr.ScheduleGPUAffirm ( *this, MOAIGfxPipelineClerk::DRAWING_PIPELINE ); // always update in the drawing pipeline
	}
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxResource::Unbind () {

	this->OnGPUUnbind ();
}
