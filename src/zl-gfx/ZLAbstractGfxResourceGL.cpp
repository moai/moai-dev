// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLAbstractGfxResourceGL.h>
#include <zl-gfx/ZLGfxMgrGL.h>
#include <zl-gfx/ZLGfxResourceClerkGL.h>

//================================================================//
// ZLAbstractGfxResourceGL
//================================================================//

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::Affirm () {

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
u32 ZLAbstractGfxResourceGL::Bind () {

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
void ZLAbstractGfxResourceGL::Destroy () {

	this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
	this->ZLAbstractGfxResource_OnCPUDestroy ();
	this->mState = STATE_UNINITIALIZED;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::DoCPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// whether or not GPU is deferred, do the CPU part
	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->ZLAbstractGfxResource_OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}
	return this->mState != STATE_ERROR;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::DoGPUCreate () {

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
bool ZLAbstractGfxResourceGL::DoGPUUpdate () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->mState = this->ZLAbstractGfxResource_OnGPUUpdate () ? STATE_READY_TO_BIND : STATE_ERROR;
	}
	return true;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResourceGL::FinishInit () {

	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) {
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->ScheduleForGPUCreate ( ZLGfxPipelineClerkGL::DRAWING_PIPELINE );
	}
	else {
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::HasLoader () {

	return this->ZLAbstractGfxResource_HasLoader ();
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::InvokeLoader () {

	return this->ZLAbstractGfxResource_InvokeLoader ();
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::Purge ( u32 age ) {

	if ( this->mState == STATE_ERROR ) return false;

	age = this->mGfxMgr->GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		this->ZLAbstractGfxResource_OnCPUDestroy ();
		this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
		this->mState = STATE_READY_FOR_CPU_CREATE;
		
		this->ScheduleForGPUCreate ( ZLGfxPipelineClerkGL::DRAWING_PIPELINE );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResourceGL::Renew () {

	// any (valid) state other than error we go back to square zero
	if ( !(( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR ))) {
	
		this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( false ); // clear out the resource id (if any)
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->InvokeLoader ();
		this->DoGPUCreate ();
	}
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::ScheduleForGPUCreate ( u32 pipelineID ) {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) return false;
	
	this->mGfxMgr->ScheduleGPUAffirm ( *this, pipelineID );
	return true;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::ScheduleForGPUUpdate () {

	if ( this->mState != STATE_READY_TO_BIND ) return false;

	this->mState = STATE_NEEDS_GPU_UPDATE;

	this->mGfxMgr->ScheduleGPUAffirm ( *this, ZLGfxPipelineClerkGL::DRAWING_PIPELINE ); // always update in the drawing pipeline
	return true;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResourceGL::SetGfxMgr ( ZLGfxMgrGL& gfxMgr ) {

	this->mGfxMgr = &gfxMgr;
	this->mGfxMgr->InsertGfxResource ( *this );
}

//----------------------------------------------------------------//
void ZLAbstractGfxResourceGL::Unbind () {

	this->ZLAbstractGfxResource_OnGPUUnbind ();
}

//----------------------------------------------------------------//
ZLAbstractGfxResourceGL::ZLAbstractGfxResourceGL () :
	mGfxMgr ( NULL ),
	mState ( STATE_UNINITIALIZED ),
	mLastRenderCount ( 0 ) {

	RTTI_SINGLE ( RTTIBase )

	this->mMasterLink.Data ( this );
	this->mPendingLink.Data ( this );
}

//----------------------------------------------------------------//
ZLAbstractGfxResourceGL::~ZLAbstractGfxResourceGL () {

	this->mGfxMgr->GetGfxResourceClerk ().RemoveGfxResource ( *this );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::ZLAbstractGfxResource_HasLoader () {

	return false;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResourceGL::ZLAbstractGfxResource_InvokeLoader () {

	return false;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResourceGL::ZLAbstractGfxResource_OnClearDirty () {
}

//----------------------------------------------------------------//
void ZLAbstractGfxResourceGL::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( userdata );

	if ( event == GFX_EVENT_CREATED ) {
	
		// we should only get this event if the creation sequence was successful
		this->mState = STATE_READY_TO_BIND;
	}
}
