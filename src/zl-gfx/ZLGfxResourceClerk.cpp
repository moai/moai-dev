// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLAbstractGfxResource.h>
#include <zl-gfx/ZLGfxPipelineClerk.h>
#include <zl-gfx/ZLGfxResourceClerk.h>
#include <zl-gfx/ZLGfxMgrGL.h>

//================================================================//
// ZLGfxResourceClerk
//================================================================//

//----------------------------------------------------------------//
void ZLGfxResourceClerk::DeleteOrDiscard ( const ZLGfxHandle& handle, bool shouldDelete ) {

	if ( handle ) {
		if ( shouldDelete ) {
			this->mDeleterStack.Push ( handle );
		}
		else {
			ZLGfx::DiscardResource ( handle );
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::DiscardResources () {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( false );
	}
	
	ZLSize top = this->mDeleterStack.GetTop ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < top; ++i ) {
		ZLGfx::DiscardResource ( this->mDeleterStack [ i ]);
	}
	this->mDeleterStack.Reset ();
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::InsertGfxResource ( ZLAbstractGfxResource& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::ProcessDeleters ( ZLGfx& gfx ) {

	ZLSize top = this->mDeleterStack.GetTop ();

	if ( top ) {
	
		gfx.Flush ();
	
		for ( ZLIndex i = ZLIndexOp::ZERO; i < top; ++i ) {
			gfx.DeleteResource ( this->mDeleterStack [ i ]);
		}
		this->mDeleterStack.Reset ();
	
		gfx.Flush ();
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::ProcessPending ( ZLGfx& gfx, ZLLeanList < ZLAbstractGfxResource* > &list ) {
	
	this->ProcessDeleters ( gfx );
	
	ResourceIt resourceIt = list.Head ();
	while ( resourceIt ) {
		ZLAbstractGfxResource* resource = resourceIt->Data ();
		resourceIt = resourceIt->Next ();
	
		resource->Affirm ();
	}
	list.Clear ();
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::PurgeResources ( u32 age ) {
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::RemoveGfxResource ( ZLAbstractGfxResource& resource ) {

	this->mResources.Remove ( resource.mMasterLink );
	this->mPendingForLoadList.Remove ( resource.mPendingLink );
	this->mPendingForDrawList.Remove ( resource.mPendingLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void ZLGfxResourceClerk::RenewResources () {

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::ScheduleGPUAffirm ( ZLAbstractGfxResource& resource, u32 listID ) {

	switch ( listID ) {

		case ZLGfxPipelineClerk::LOADING_PIPELINE:
			this->mPendingForLoadList.PushBack ( resource.mPendingLink );
			break;
		
		case ZLGfxPipelineClerk::DRAWING_PIPELINE:
			this->mPendingForDrawList.PushBack ( resource.mPendingLink );
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerk::Update () {

	ZLGfxPipelineClerk& pipelineClerk = this->GetGfxPipelineClerk ();

	if ( this->mDeleterStack.GetTop () || this->mPendingForLoadList.Count ()) {
	
		ZLGfx* gfxLoading = pipelineClerk.SelectDrawingAPI ( ZLGfxPipelineClerk::LOADING_PIPELINE );
		
		if ( gfxLoading ) {
		
			ZGL_COMMENT ( *gfxLoading, "RESOURCE MGR LOADING PIPELINE UPDATE" );
			this->ProcessDeleters ( *gfxLoading );
			this->ProcessPending ( *gfxLoading, this->mPendingForLoadList );
			this->GetGfxStateCacheGPU ().UnbindAll ();
		}
	}
	
	if ( this->mPendingForDrawList.Count ()) {
	
		ZLGfx* gfxDrawing = pipelineClerk.SelectDrawingAPI ( ZLGfxPipelineClerk::DRAWING_PIPELINE );
		
		if ( gfxDrawing ) {
		
			ZGL_COMMENT ( *gfxDrawing, "RESOURCE MGR DRAWING PIPELINE UPDATE" );
			this->ProcessPending ( *gfxDrawing, this->mPendingForDrawList );
			this->GetGfxStateCacheGPU ().UnbindAll ();
		}
	}

	// TODO: think about cases where we can get async results back on the
	// same display list so we can remove the one-frame lag when creating resources
	// in retained mode.
}

//----------------------------------------------------------------//
ZLGfxResourceClerk::ZLGfxResourceClerk () {
}

//----------------------------------------------------------------//
ZLGfxResourceClerk::~ZLGfxResourceClerk () {

	this->DiscardResources ();
}
