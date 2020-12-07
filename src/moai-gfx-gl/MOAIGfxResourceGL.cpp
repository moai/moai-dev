// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxResourceGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_DisplayListClerkGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	destroy
	@text	Releases any CPU memory associated with buffer and schedules
			the GPU resource (if any) for destruction.
 
	@in		MOAIGfxBuffer self
	@out	nil
*/
int	MOAIGfxResourceGL::_destroy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResourceGL, "U" )
	
	self->Finalize ();
	return 0;
}

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

	u32 age = MOAIGfxMgr::Get ().GetRenderCounter () - self->GetLastRenderCount ();
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

//----------------------------------------------------------------//
/**	@lua	scheduleForGPUUpdate
	@text	Trigger an update of the GPU-side buffer. Call this when
			the backing buffer has been altered.
 
	@in		MOAIGfxResourceGL self
	@out	nil
*/
int MOAIGfxResourceGL::_scheduleForGPUUpdate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResourceGL, "U" )
	
	self->ScheduleForGPUUpdate ();
	return 0;
}

//================================================================//
// MOAIGfxResourceGL
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::Affirm () {

	// TODO: ZLGfx
//	if ( !MOAIGfxMgr::Get ().GetHasContext ()) {
//		MOAILog ( 0, MOAISTRING_MOAIGfxResource_MissingDevice );
//		return false;
//	}

	if ( this->mState != STATE_READY_TO_BIND ) {
	
		if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
			this->DoGPUUpdate ();
		}
		else {
			this->DoGPUCreate ();
		}
	}
	
	if ( this->mState == STATE_READY_TO_BIND ) {
		this->mLastRenderCount = this->mGfxMgr->GetRenderCounter ();
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::Bind () {

	if ( this->Affirm ()) {
		this->MOAIGfxResourceGL_OnGPUBind ();
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::DoGPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	if ( this->mState == STATE_READY_FOR_GPU_CREATE ) {
	
		// set this now; in immediate mode we'll go straight to STATE_READY_TO_BIND on success
		// otherwise we'll go there later when we get GFX_EVENT_CREATED
		// why can't we set STATE_READY_TO_BIND? because in multi-threaded mode, it might be
		// a few more frames before we get GFX_EVENT_CREATED
		
		this->mState = STATE_PENDING;
		
		if ( this->MOAIGfxResourceGL_OnGPUCreate ()) {
			this->mGfxMgr->GetDrawingAPI ().Event ( this, GFX_EVENT_CREATED, 0 );
			this->ClearReloadable ();
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

	RTTI_BEGIN ( MOAIGfxResourceGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxResourceGL >)
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
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::Purge ( u32 age ) {

	if ( this->mState == STATE_ERROR ) return false;

	age = this->mGfxMgr->GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		this->ClearReloadable ();
		this->MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( true );
		this->mState = STATE_READY_FOR_GPU_CREATE;
		
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
		this->mState = STATE_READY_FOR_GPU_CREATE;
		this->Reload ();
		this->DoGPUCreate ();
	}
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::ScheduleForGPUUpdate ( PipelineHint hint ) {
	
	switch ( this->mState ) {
		
		case STATE_UNINITIALIZED:
		case STATE_ERROR:
			this->mState = STATE_READY_FOR_GPU_CREATE;
			break;
			
		case STATE_READY_TO_BIND:
			this->mState = STATE_NEEDS_GPU_UPDATE;
			break;
		default:
			return false;
	}
	
	this->mGfxMgr->ScheduleGPUAffirm ( *this, PipelineHint::RENDER_PIPELINE );
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::Unbind () {

	this->MOAIGfxResourceGL_OnGPUUnbind ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResourceGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	state.SetField ( -1, "STATE_UNINITIALIZED",					( u32 )MOAIGfxResourceGL::STATE_UNINITIALIZED );
	state.SetField ( -1, "STATE_READY_FOR_GPU_CREATE",			( u32 )MOAIGfxResourceGL::STATE_READY_FOR_GPU_CREATE );
	state.SetField ( -1, "STATE_READY_TO_BIND",					( u32 )MOAIGfxResourceGL::STATE_READY_TO_BIND );
	state.SetField ( -1, "STATE_ERROR",							( u32 )MOAIGfxResourceGL::STATE_ERROR );
	
	state.SetField ( -1, "GFX_EVENT_CREATED",					( u32 )MOAIGfxResourceGL::GFX_EVENT_CREATED );
	
	state.SetField ( -1, "DRAWING_QUEUE",						( u32 )MOAIGfxMgrGL_DisplayListClerkGL::DRAWING_QUEUE );
	state.SetField ( -1, "LOADING_QUEUE",						( u32 )MOAIGfxMgrGL_DisplayListClerkGL::LOADING_QUEUE );
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "clear",						_destroy }, // TODO: deprecate
		{ "destroy",					_destroy },
		{ "getAge",						_getAge },
		{ "getResourceState",			_getResourceState },
		{ "release",					_destroy }, // TODO: deprecate
		{ "purge",						_purge },
		{ "scheduleFlush",				_scheduleForGPUUpdate },
		{ "scheduleForGPUUpdate",		_scheduleForGPUUpdate },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::MOAIGfxResource_IsReadyForUse () const {

	return (( this->mState == STATE_READY_TO_BIND ) || ( this->mState == STATE_NEEDS_GPU_UPDATE ));
}

//----------------------------------------------------------------//
void MOAIGfxResourceGL::MOAIGfxResource_ScheduleForGPUDestroy () {

	if ( this->IsReadyForUse ()) {
		this->MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( true );
	}
	this->mState = STATE_UNINITIALIZED;
}

//----------------------------------------------------------------//
bool MOAIGfxResourceGL::MOAIGfxResource_ScheduleForGPUUpdate () {

	return this->ScheduleForGPUUpdate ( RENDER_PIPELINE );
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
