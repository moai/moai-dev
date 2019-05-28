// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLAbstractGfxResourceGL.h>
#include <zl-gfx/ZLGfxPipelineClerkGL.h>
#include <zl-gfx/ZLGfxResourceClerkGL.h>
#include <zl-gfx/ZLGfxMgrGL.h>

//================================================================//
// ZLGfxResourceClerkGL
//================================================================//

//----------------------------------------------------------------//
void ZLGfxResourceClerkGL::DeleteOrDiscard ( const ZLGfxHandleGL& handle, bool shouldDelete ) {

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
void ZLGfxResourceClerkGL::DiscardResources () {

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
void ZLGfxResourceClerkGL::InsertGfxResource ( ZLAbstractGfxResourceGL& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
void ZLGfxResourceClerkGL::ProcessDeleters ( ZLGfx& gfx ) {

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
void ZLGfxResourceClerkGL::ProcessPending ( ZLGfx& gfx, ZLLeanList < ZLAbstractGfxResourceGL* > &list ) {
	
	this->ProcessDeleters ( gfx );
	
	ResourceIt resourceIt = list.Head ();
	while ( resourceIt ) {
		ZLAbstractGfxResourceGL* resource = resourceIt->Data ();
		resourceIt = resourceIt->Next ();
	
		resource->Affirm ();
	}
	list.Clear ();
}

//----------------------------------------------------------------//
void ZLGfxResourceClerkGL::PurgeResources ( u32 age ) {
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerkGL::RemoveGfxResource ( ZLAbstractGfxResourceGL& resource ) {

	this->mResources.Remove ( resource.mMasterLink );
	this->mPendingForLoadList.Remove ( resource.mPendingLink );
	this->mPendingForDrawList.Remove ( resource.mPendingLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void ZLGfxResourceClerkGL::RenewResources () {

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerkGL::ScheduleGPUAffirm ( ZLAbstractGfxResourceGL& resource, u32 listID ) {

	switch ( listID ) {

		case ZLGfxPipelineClerkGL::LOADING_PIPELINE:
			this->mPendingForLoadList.PushBack ( resource.mPendingLink );
			break;
		
		case ZLGfxPipelineClerkGL::DRAWING_PIPELINE:
			this->mPendingForDrawList.PushBack ( resource.mPendingLink );
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxResourceClerkGL::Update () {

	ZLGfxPipelineClerkGL& pipelineClerk = this->GetGfxPipelineClerk ();

	if ( this->mDeleterStack.GetTop () || this->mPendingForLoadList.Count ()) {
	
		ZLGfx* gfxLoading = pipelineClerk.SelectDrawingAPI ( ZLGfxPipelineClerkGL::LOADING_PIPELINE );
		
		if ( gfxLoading ) {
		
			ZGL_COMMENT ( *gfxLoading, "RESOURCE MGR LOADING PIPELINE UPDATE" );
			this->ProcessDeleters ( *gfxLoading );
			this->ProcessPending ( *gfxLoading, this->mPendingForLoadList );
			this->GetGfxStateCacheGPU ().UnbindAll ();
		}
	}
	
	if ( this->mPendingForDrawList.Count ()) {
	
		ZLGfx* gfxDrawing = pipelineClerk.SelectDrawingAPI ( ZLGfxPipelineClerkGL::DRAWING_PIPELINE );
		
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
ZLGfxResourceClerkGL::ZLGfxResourceClerkGL () {
}

//----------------------------------------------------------------//
ZLGfxResourceClerkGL::~ZLGfxResourceClerkGL () {

	this->DiscardResources ();
}
