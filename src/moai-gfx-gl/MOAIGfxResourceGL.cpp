// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxResourceGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_DisplayListClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_RenderTreeGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getAge
	@text	Returns the 'age' of the graphics resource. The age
			is the number of times MOAIGfxMgrGL_RenderTreeGL has rendered a scene
			since the resource was last bound. It is part of the
			render count, not a timestamp. This may change to be
			time-based in future releases.
 
	@in		MOAIGfxResourceGL self
	@out	number age
*/
int MOAIGfxResourceGL::_getAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResourceGL, "U" )

	u32 age = MOAIGfxMgrGL_RenderTreeGL::Get ().GetRenderCounter () - self->GetLastRenderCount ();
	lua_pushnumber ( state, age );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResourceGL::_getResourceState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResourceGL, "U" )

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
 
	@in		MOAIGfxResourceGL self
	@opt	number age				Release only if the texture hasn't been used in X frames.
	@out	boolean released		True if the texture was actually released.
*/
int MOAIGfxResourceGL::_purge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResourceGL, "U" )

	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean ( L, self->Purge ( age ));

	return 1;
}

//================================================================//
// MOAIGfxResourceGL
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::Affirm () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->DoGPUUpdate ();
	}
	else {
		this->DoGPUCreate ();
	}
	return this->IsReady ();
}

//----------------------------------------------------------------//
u32 MOAIGfxResourceGL::Bind () {

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
		this->MOAIGfxResourceGL_OnGPUBind ();
	}
	return this->mState;
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::DoGPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// if we get here, load both CPU and GPU parts

	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->MOAIGfxResource_OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}

	if ( this->mState == STATE_READY_FOR_GPU_CREATE ) {
	
		// set this now; in immediate mode we'll go straight to STATE_READY_TO_BIND on success
		// otherwise we'll go there later when we get GFX_EVENT_CREATED
		// why can't we set STATE_READY_TO_BIND? becausein multi-threaded mode, it might be
		// a few more frames before we get GFX_EVENT_CREATED
		
		this->mState = STATE_PENDING;
		
		if ( this->MOAIGfxResourceGL_OnGPUCreate ()) {
			this->mGfxMgr->GetDrawingAPI ().Event ( this, GFX_EVENT_CREATED, 0 );
			this->MOAIGfxResource_OnCPUPurgeRecoverable ();
		}
		else {
			this->mState = STATE_ERROR;
		}
	}
	return this->mState == STATE_READY_TO_BIND;
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::DoGPUUpdate () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->mState = this->MOAIGfxResourceGL_OnGPUUpdate () ? STATE_READY_TO_BIND : STATE_ERROR;
	}
	return true;
}

//----------------------------------------------------------------//
MOAIGfxResourceGL::MOAIGfxResourceGL () :
	mGfxMgr ( &MOAIGfxMgrGL::Get ()),
	mState ( STATE_UNINITIALIZED ),
	mLastRenderCount ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
		RTTI_EXTEND ( MOAIInstanceEventSource )
	RTTI_END

	this->mMasterLink.Data ( this );
	this->mPendingLink.Data ( this );

	this->mGfxMgr->InsertGfxResource ( *this );
}

//----------------------------------------------------------------//
MOAIGfxResourceGL::~MOAIGfxResourceGL () {

	this->mGfxMgr->RemoveGfxResource ( *this );
	this->mReloader.Clear ();
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::Purge ( u32 age ) {

	if ( this->mState == STATE_ERROR ) return false;

	age = this->mGfxMgr->GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		this->MOAIGfxResource_OnCPUPurgeRecoverable ();
		this->MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( true );
		this->mState = STATE_READY_FOR_CPU_CREATE;
		
		this->ScheduleForGPUUpdate ( PipelineHint::RENDER_PIPELINE );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::Renew () {

	// any (valid) state other than error we go back to square zero
	if ( !(( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR ))) {
	
		this->MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( false ); // clear out the resource id (if any)
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->Reload ();
		this->DoGPUCreate ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::Unbind () {

	this->MOAIGfxResourceGL_OnGPUUnbind ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::MOAIGfxResource_DoCPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// whether or not GPU is deferred, do the CPU part
	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->MOAIGfxResource_OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}
	return this->mState != STATE_ERROR;
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::MOAIGfxResource_IsReady () const {

	return ( this->mState == STATE_READY_TO_BIND );
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::MOAIGfxResource_ScheduleForGPUDestroy () {

	this->MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( true );
	this->mState = STATE_UNINITIALIZED;
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::MOAIGfxResource_ScheduleForGPUUpdate ( PipelineHint hint ) {

	switch ( this->mState ) {
		
		case STATE_UNINITIALIZED:
		case STATE_ERROR:
			this->mState = STATE_READY_FOR_CPU_CREATE;
			break;
		
		case STATE_READY_FOR_CPU_CREATE:
			break;
			
		case STATE_READY_TO_BIND:
			this->mState = STATE_NEEDS_GPU_UPDATE;
			break;
		default:
			return false;
	}
	
	u32 queueID = hint == PipelineHint::RENDER_PIPELINE ? MOAIGfxMgrGL_DisplayListClerkGL::DRAWING_QUEUE : MOAIGfxMgrGL_DisplayListClerkGL::LOADING_QUEUE;
	
	this->mGfxMgr->ScheduleGPUAffirm ( *this, queueID );
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIInstanceEventSource, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
	state.SetField ( -1, "STATE_UNINITIALIZED",					( u32 )MOAIGfxResourceGL::STATE_UNINITIALIZED );
	state.SetField ( -1, "STATE_READY_FOR_CPU_CREATE",			( u32 )MOAIGfxResourceGL::STATE_READY_FOR_CPU_CREATE );
	state.SetField ( -1, "STATE_READY_FOR_GPU_CREATE",			( u32 )MOAIGfxResourceGL::STATE_READY_FOR_GPU_CREATE );
	state.SetField ( -1, "STATE_READY_TO_BIND",					( u32 )MOAIGfxResourceGL::STATE_READY_TO_BIND );
	state.SetField ( -1, "STATE_ERROR",							( u32 )MOAIGfxResourceGL::STATE_ERROR );
	
	state.SetField ( -1, "GFX_EVENT_CREATED",					( u32 )MOAIGfxResourceGL::GFX_EVENT_CREATED );
	
	state.SetField ( -1, "DRAWING_QUEUE",					( u32 )MOAIGfxMgrGL_DisplayListClerkGL::DRAWING_QUEUE );
	state.SetField ( -1, "LOADING_QUEUE",					( u32 )MOAIGfxMgrGL_DisplayListClerkGL::LOADING_QUEUE );
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIInstanceEventSource, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getAge",					_getAge },
		{ "getResourceState",		_getResourceState },
		{ "purge",					_purge },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( userdata );

	if ( event == GFX_EVENT_CREATED ) {
	
		// let Lua know the resource is ready for use
		this->InvokeListener ( MOAIGfxResourceGL::GFX_EVENT_CREATED );
	
		// we should only get this event if the creation sequence was successful
		this->mState = STATE_READY_TO_BIND;
	}
}
