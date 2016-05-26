// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceClerk.h>

//================================================================//
// MOAIGfxResourceClerk
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResourceClerk::DeleteOrDiscardHandle ( ZLGfxHandle*& handle, bool shouldDelete ) {

	if ( handle ) {
		if ( shouldDelete && MOAIGfxMgr::IsValid ()) {
		
			MOAIGfxMgr::Get ().mResourceMgr.mDeleterStack.Push ( handle );
			return;
		}
		ZLGfx::Discard ( handle );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerk::DiscardResources () {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->OnGPUDeleteOrDiscard ( false );
	}
	
	size_t top = this->mDeleterStack.GetTop ();

	for ( size_t i = 0; i < top; ++i ) {
		ZLGfx::Discard ( this->mDeleterStack [ i ]);
	}
	this->mDeleterStack.Reset ();
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerk::InsertGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
MOAIGfxResourceClerk::MOAIGfxResourceClerk () {
}

//----------------------------------------------------------------//
MOAIGfxResourceClerk::~MOAIGfxResourceClerk () {
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerk::ProcessDeleters () {

	size_t top = this->mDeleterStack.GetTop ();

	if ( top ) {

		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

		ZLGfxDevice::Begin ();
	
		gfx.Flush ();
	
		for ( size_t i = 0; i < top; ++i ) {
			ZLGfxHandle* handle = this->mDeleterStack [ i ];
			gfx.Delete ( handle );
		}
		this->mDeleterStack.Reset ();
	
		gfx.Flush ();
	
		ZLGfxDevice::End ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerk::ProcessPending ( ZLLeanList < MOAIGfxResource* > &list ) {
	
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
void MOAIGfxResourceClerk::PurgeResources ( u32 age ) {
	
	ZLGfxDevice::Begin ();
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
	
	ZLGfxDevice::End ();
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerk::RemoveGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.Remove ( resource.mMasterLink );
	this->mPendingForLoadList.Remove ( resource.mPendingLink );
	this->mPendingForDrawList.Remove ( resource.mPendingLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void MOAIGfxResourceClerk::RenewResources () {

	ZLGfxDevice::Begin ();

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
	
	ZLGfxDevice::End ();
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerk::ScheduleGPUAffirm ( MOAIGfxResource& resource, u32 listID ) {

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
void MOAIGfxResourceClerk::Update () {

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
