// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLAbstractGfxResource.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// ZLAbstractGfxResource
//================================================================//

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::Affirm () {

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
u32 ZLAbstractGfxResource::Bind () {

//	if ( !MOAIGfxMgr::Get ().GetHasContext ()) {
//		MOAILog ( 0, MOAISTRING_MOAIGfxResource_MissingDevice );
//		return false;
//	}

	if ( this->mState != STATE_READY_TO_BIND ) {
		this->Affirm ();
	}

	// we're ready to bind, so do it
	if ( this->mState == STATE_READY_TO_BIND ) {
		this->mLastRenderCount = MOAIRenderMgr::Get ().GetRenderCounter ();
		this->ZLAbstractGfxResource_OnGPUBind ();
	}
	return this->mState;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResource::Destroy () {

	this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
	this->ZLAbstractGfxResource_OnCPUDestroy ();
	this->mState = STATE_UNINITIALIZED;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::DoCPUCreate () {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_ERROR ) || ( this->mState == STATE_UNINITIALIZED )) return false;

	// whether or not GPU is deferred, do the CPU part
	if ( this->mState == STATE_READY_FOR_CPU_CREATE ) {
		this->mState = this->ZLAbstractGfxResource_OnCPUCreate () ? STATE_READY_FOR_GPU_CREATE : STATE_ERROR;
	}
	return this->mState != STATE_ERROR;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::DoGPUCreate () {

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
			MOAIGfxMgr::GetDrawingAPI ().Event ( this, GFX_EVENT_CREATED, 0 );
			this->ZLAbstractGfxResource_OnCPUDestroy ();
		}
		else {
			this->mState = STATE_ERROR;
		}
	}
	return this->mState == STATE_READY_TO_BIND;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::DoGPUUpdate () {

	if ( this->mState == STATE_NEEDS_GPU_UPDATE ) {
		this->mState = this->ZLAbstractGfxResource_OnGPUUpdate () ? STATE_READY_TO_BIND : STATE_ERROR;
	}
	return true;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResource::FinishInit () {

	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) {
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->ScheduleForGPUCreate ( MOAIGfxPipelineClerk::DRAWING_PIPELINE );
	}
	else {
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::HasLoader () {

	return this->ZLAbstractGfxResource_HasLoader ();
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::InvokeLoader () {

	return this->ZLAbstractGfxResource_InvokeLoader ();
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::Purge ( u32 age ) {

	if ( this->mState == STATE_ERROR ) return false;

	age = MOAIRenderMgr::Get ().GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		this->ZLAbstractGfxResource_OnCPUDestroy ();
		this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( true );
		this->mState = STATE_READY_FOR_CPU_CREATE;
		
		this->ScheduleForGPUCreate ( MOAIGfxPipelineClerk::DRAWING_PIPELINE );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResource::Renew () {

	// any (valid) state other than error we go back to square zero
	if ( !(( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR ))) {
	
		this->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( false ); // clear out the resource id (if any)
		this->mState = STATE_READY_FOR_CPU_CREATE;
		this->InvokeLoader ();
		this->DoGPUCreate ();
	}
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::ScheduleForGPUCreate ( u32 pipelineID ) {

	if ( this->mState == STATE_READY_TO_BIND ) return true;
	if (( this->mState == STATE_UNINITIALIZED ) || ( this->mState == STATE_ERROR )) return false;
	
	if ( MOAIGfxMgr::IsValid ()) {
		MOAIGfxMgr::Get ().mResourceMgr.ScheduleGPUAffirm ( *this, pipelineID );
	}
	return true;
}

//----------------------------------------------------------------//
bool ZLAbstractGfxResource::ScheduleForGPUUpdate () {

	if ( this->mState != STATE_READY_TO_BIND ) return false;

	this->mState = STATE_NEEDS_GPU_UPDATE;

	if ( MOAIGfxMgr::IsValid ()) {
		MOAIGfxMgr::Get ().mResourceMgr.ScheduleGPUAffirm ( *this, MOAIGfxPipelineClerk::DRAWING_PIPELINE ); // always update in the drawing pipeline
	}
	return true;
}

//----------------------------------------------------------------//
void ZLAbstractGfxResource::Unbind () {

	this->ZLAbstractGfxResource_OnGPUUnbind ();
}

//----------------------------------------------------------------//
ZLAbstractGfxResource::ZLAbstractGfxResource () :
	mState ( STATE_UNINITIALIZED ),
	mLastRenderCount ( 0 ) {

	RTTI_SINGLE ( RTTIBase )

	this->mMasterLink.Data ( this );
	this->mPendingLink.Data ( this );
	
	MOAIGfxMgr::Get ().mResourceMgr.InsertGfxResource ( *this );
}

//----------------------------------------------------------------//
ZLAbstractGfxResource::~ZLAbstractGfxResource () {

	if ( MOAIGfxMgr::IsValid ()) {
		MOAIGfxMgr::Get ().mResourceMgr.RemoveGfxResource ( *this );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void ZLAbstractGfxResource::ZLAbstractGfxResource_OnClearDirty () {
}

//----------------------------------------------------------------//
void ZLAbstractGfxResource::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( userdata );

	if ( event == GFX_EVENT_CREATED ) {
	
		// we should only get this event if the creation sequence was successful
		this->mState = STATE_READY_TO_BIND;
	}
}
