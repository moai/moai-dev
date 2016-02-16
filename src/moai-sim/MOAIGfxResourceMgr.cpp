// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceMgr.h>

//================================================================//
// MOAIGfxResourceMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::InsertGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
MOAIGfxResourceMgr::MOAIGfxResourceMgr () {
}

//----------------------------------------------------------------//
MOAIGfxResourceMgr::~MOAIGfxResourceMgr () {
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::ProcessDeleters () {

	u32 top = this->mDeleterStack.GetTop ();

	if ( top ) {

		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

		ZLGfxDevice::Begin ();
	
		gfx.Flush ();
	
		for ( u32 i = 0; i < top; ++i ) {
			ZLGfxHandle* handle = this->mDeleterStack [ i ];
			gfx.DeleteHandle ( handle );
		}
		this->mDeleterStack.Reset ();
	
		gfx.Flush ();
	
		ZLGfxDevice::End ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::ProcessPending ( ZLLeanList < MOAIGfxResource* > &list ) {
	
	this->ProcessDeleters ();
	
	ResourceIt resourceIt = list.Head ();
	while ( resourceIt ) {
		MOAIGfxResource* resource = resourceIt->Data ();
		resourceIt = resourceIt->Next ();
	
		resource->Affirm ();
	}
	list.Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::PurgeResources ( u32 age ) {
	
	ZLGfxDevice::Begin ();
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
	
	ZLGfxDevice::End ();
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::PushDeleter ( ZLGfxHandle* handle ) {

	if ( handle ) {
		this->mDeleterStack.Push ( handle );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::RemoveGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.Remove ( resource.mMasterLink );
	this->mPendingForLoadList.Remove ( resource.mPendingLink );
	this->mPendingForDrawList.Remove ( resource.mPendingLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void MOAIGfxResourceMgr::RenewResources () {

	ZLGfxDevice::Begin ();

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
	
	ZLGfxDevice::End ();
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::ScheduleGPUAffirm ( MOAIGfxResource& resource, u32 listID ) {

	switch ( listID ) {

		case MOAIGfxPipelineClerk::LOADING_PIPELINE:
			this->mPendingForLoadList.PushBack ( resource.mPendingLink );
			break;
		
		case MOAIGfxPipelineClerk::DRAWING_PIPELINE:
			this->mPendingForDrawList.PushBack ( resource.mPendingLink );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::Update () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	ZLGfxDevice::Begin ();

	if ( this->mDeleterStack.GetTop () || this->mPendingForLoadList.Count ()) {
	
		ZLGfx& gfxLoading = gfxMgr.mPipelineMgr.SelectDrawingAPI ( MOAIGfxPipelineClerk::LOADING_PIPELINE, true );
		
		ZGL_COMMENT ( gfxLoading, "RESOURCE MGR LOADING PIPELINE UPDATE" );
		this->ProcessDeleters ();
		this->ProcessPending ( this->mPendingForLoadList );
		gfxMgr.mGfxState.UnbindAll ();
	}
	
	if ( this->mPendingForDrawList.Count ()) {
	
		ZLGfx& gfxDrawing = gfxMgr.mPipelineMgr.SelectDrawingAPI ( MOAIGfxPipelineClerk::DRAWING_PIPELINE, true );
		
		ZGL_COMMENT ( gfxDrawing, "RESOURCE MGR DRAWING PIPELINE UPDATE" );
		this->ProcessPending ( this->mPendingForDrawList );
		gfxMgr.mGfxState.UnbindAll ();
	}

	// TODO: think about cases where we can get async results back on the
	// same display list so we can remove the one-frame lag when creating resources
	// in retained mode.
	
	ZLGfxDevice::End ();
}
