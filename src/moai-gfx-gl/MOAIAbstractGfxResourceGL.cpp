// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIAbstractGfxResourceGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_PipelineClerkGL.h>
#include <moai-gfx-gl/MOAIRenderMgrGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getAge
	@text	Returns the 'age' of the graphics resource. The age
			is the number of times MOAIRenderMgrGL has rendered a scene
			since the resource was last bound. It is part of the
			render count, not a timestamp. This may change to be
			time-based in future releases.
 
	@in		MOAIAbstractGfxResourceGL self
	@out	number age
*/
int MOAIAbstractGfxResourceGL::_getAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	u32 age = MOAIRenderMgrGL::Get ().GetRenderCounter () - self->GetLastRenderCount ();
	lua_pushnumber ( state, age );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxResourceGL::_getResourceState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	state.Push ( self->GetState ());
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
 
	@in		MOAIAbstractGfxResourceGL self
	@opt	number age				Release only if the texture hasn't been used in X frames.
	@out	boolean released		True if the texture was actually released.
*/
int MOAIAbstractGfxResourceGL::_purge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean ( L, self->Purge ( age ));

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxResourceGL::_scheduleForGPUCreate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	u32 listID = state.GetValue < u32 >( 2, MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE );
	self->ScheduleForGPUCreate ( listID );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setReloader
	@text	The reloaded is called prior to recreating the resource. It should
			in turn call the resources regular load or init methods.
 
	@in		MOAIAbstractGfxResourceGL self
	@opt	function reloader
	@out	nil
*/
int MOAIAbstractGfxResourceGL::_setReloader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	self->mReloader.SetRef ( *self, state, 2 );
	self->InvokeLoader ();
	return 0;
}

//================================================================//
// MOAIAbstractGfxResourceGL
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::Affirm () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->DoGPUUpdate ();
	}
	else {
		this->InvokeLoader ();
		this->DoGPUCreate ();
	}
	return this->IsReady ();
}

//----------------------------------------------------------------//
u32 MOAIAbstractGfxResourceGL::Bind () {

	// TODO: ZLGfx
//	if ( !MOAIGfxMgr::Get ().GetHasContext ()) {
//		MOAILog ( 0, MOAISTRING_MOAIGfxResource_MissingDevice );
//		return false;
//	}

	if ( this->mState != STATE_READY_TO_BIND ) {
		this->Affirm ();
	}

	// we're ready to bind, so do it
	if ( this->mState == STATE_READY_TO_BIND ) {
		// TODO: ZLGfx
		this->mLastRenderCount = this->mGfxMgr->GetRenderCounter ();
		this->ZLAbstractGfxResource_OnGPUBind ();
	}
	return this->mState;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::Destroy () {

	this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
	this->ZLAbstractGfxResource_OnCPUDestroy ();
	this->mState = STATE_UNINITIALIZED;
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::DoCPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// whether or not GPU is deferred, do the CPU part
	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->ZLAbstractGfxResource_OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}
	return this->mState != STATE_ERROR;
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::DoGPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// if we get here, load both CPU and GPU parts

	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->ZLAbstractGfxResource_OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}

	if ( this->mState == STATE_READY_FOR_GPU_CREATE ) {
	
		// set this now; in immediate mode we'll go straight to STATE_READY_TO_BIND on success
		// otherwise we'll go there later when we get GFX_EVENT_CREATED
		// why can't we set STATE_READY_TO_BIND? becausein multi-threaded mode, it might be
		// a few more frames before we get GFX_EVENT_CREATED
		
		this->mState = STATE_PENDING;
		
		if ( this->ZLAbstractGfxResource_OnGPUCreate ()) {
			this->mGfxMgr->GetDrawingAPI ().Event ( this, GFX_EVENT_CREATED, 0 );
			this->ZLAbstractGfxResource_OnCPUDestroy ();
		}
		else {
			this->mState = STATE_ERROR;
		}
	}
	return this->mState == STATE_READY_TO_BIND;
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::DoGPUUpdate () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->mState = this->ZLAbstractGfxResource_OnGPUUpdate () ? STATE_READY_TO_BIND : STATE_ERROR;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::FinishInit () {

	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) {
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->ScheduleForGPUCreate ( MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE );
	}
	else {
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::HasLoader () {

	return this->ZLAbstractGfxResource_HasLoader ();
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::InvokeLoader () {

	return this->ZLAbstractGfxResource_InvokeLoader ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxResourceGL::MOAIAbstractGfxResourceGL () :
	mGfxMgr ( NULL ),
	mState ( STATE_UNINITIALIZED ),
	mLastRenderCount ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIInstanceEventSource )
	RTTI_END

	this->mMasterLink.Data ( this );
	this->mPendingLink.Data ( this );
}

//----------------------------------------------------------------//
MOAIAbstractGfxResourceGL::~MOAIAbstractGfxResourceGL () {

	this->mGfxMgr->GetResourceClerkGL ().RemoveGfxResource ( *this );
	this->mReloader.Clear ();
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::Purge ( u32 age ) {

	if ( this->mState == STATE_ERROR ) return false;

	age = this->mGfxMgr->GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		this->ZLAbstractGfxResource_OnCPUDestroy ();
		this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
		this->mState = STATE_READY_FOR_CPU_CREATE;
		
		this->ScheduleForGPUCreate ( MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIInstanceEventSource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "STATE_UNINITIALIZED",					( u32 )MOAIAbstractGfxResourceGL::STATE_UNINITIALIZED );
	state.SetField ( -1, "STATE_READY_FOR_CPU_CREATE",			( u32 )MOAIAbstractGfxResourceGL::STATE_READY_FOR_CPU_CREATE );
	state.SetField ( -1, "STATE_READY_FOR_GPU_CREATE",			( u32 )MOAIAbstractGfxResourceGL::STATE_READY_FOR_GPU_CREATE );
	state.SetField ( -1, "STATE_READY_TO_BIND",					( u32 )MOAIAbstractGfxResourceGL::STATE_READY_TO_BIND );
	state.SetField ( -1, "STATE_ERROR",							( u32 )MOAIAbstractGfxResourceGL::STATE_ERROR );
	
	state.SetField ( -1, "GFX_EVENT_CREATED",					( u32 )MOAIAbstractGfxResourceGL::GFX_EVENT_CREATED );
	
	state.SetField ( -1, "DRAWING_PIPELINE",					( u32 )MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",					( u32 )MOAIGfxMgrGL_PipelineClerkGL::LOADING_PIPELINE );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getAge",					_getAge },
		{ "getResourceState",		_getResourceState },
		{ "purge",					_purge },
		{ "softRelease",			_purge }, // back compat
		{ "scheduleForGPUCreate",	_scheduleForGPUCreate },
		{ "setReloader",			_setReloader },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::Renew () {

	// any (valid) state other than error we go back to square zero
	if ( !(( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR ))) {
	
		this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( false ); // clear out the resource id (if any)
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->InvokeLoader ();
		this->DoGPUCreate ();
	}
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::ScheduleForGPUCreate ( u32 pipelineID ) {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) return false;
	
	this->mGfxMgr->ScheduleGPUAffirm ( *this, pipelineID );
	return true;
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::ScheduleForGPUUpdate () {

	if ( this->mState != STATE_READY_TO_BIND ) return false;

	this->mState = STATE_NEEDS_GPU_UPDATE;

	this->mGfxMgr->ScheduleGPUAffirm ( *this, MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE ); // always update in the drawing pipeline
	return true;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::SetGfxMgr ( MOAIGfxMgrGL& gfxMgr ) {

	this->mGfxMgr = &gfxMgr;
	this->mGfxMgr->InsertGfxResource ( *this );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::Unbind () {

	this->ZLAbstractGfxResource_OnGPUUnbind ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::ZLAbstractGfxResource_HasLoader () {

	return ( bool )this->mReloader;
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxResourceGL::ZLAbstractGfxResource_InvokeLoader () {

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
void MOAIAbstractGfxResourceGL::ZLAbstractGfxResource_OnClearDirty () {
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( userdata );

	if ( event == GFX_EVENT_CREATED ) {
	
		// let Lua know the resource is ready for use
		this->InvokeListener ( MOAIAbstractGfxResourceGL::GFX_EVENT_CREATED );
	
		// we should only get this event if the creation sequence was successful
		this->mState = STATE_READY_TO_BIND;
	}
}
