// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIAbstractGfxResourceGL.h>
#include <moai-gfx-gl/MOAIGfxPipelineClerkGL.h>
#include <moai-gfx-gl/MOAIGfxResourceClerkGL.h>
#include <moai-gfx-gl/MOAIGfxStateGPUCacheGL.h>

//================================================================//
// MOAIGfxResourceClerkGL
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResourceClerkGL::DeleteOrDiscard ( const ZLGfxHandle& handle, bool shouldDelete ) {

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
void MOAIGfxResourceClerkGL::DiscardResources () {

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
void MOAIGfxResourceClerkGL::InsertGfxResource ( MOAIAbstractGfxResourceGL& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerkGL::ProcessDeleters ( ZLGfx& gfx ) {

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
void MOAIGfxResourceClerkGL::ProcessPending ( ZLGfx& gfx, ZLLeanList < MOAIAbstractGfxResourceGL* > &list ) {
	
	this->ProcessDeleters ( gfx );
	
	ResourceIt resourceIt = list.Head ();
	while ( resourceIt ) {
		MOAIAbstractGfxResourceGL* resource = resourceIt->Data ();
		resourceIt = resourceIt->Next ();
	
		resource->Affirm ();
	}
	list.Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerkGL::PurgeResources ( u32 age ) {
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerkGL::RemoveGfxResource ( MOAIAbstractGfxResourceGL& resource ) {

	this->mResources.Remove ( resource.mMasterLink );
	this->mPendingForLoadList.Remove ( resource.mPendingLink );
	this->mPendingForDrawList.Remove ( resource.mPendingLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void MOAIGfxResourceClerkGL::RenewResources () {

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerkGL::ScheduleGPUAffirm ( MOAIAbstractGfxResourceGL& resource, u32 listID ) {

	switch ( listID ) {

		case MOAIGfxPipelineClerkGL::LOADING_PIPELINE:
			this->mPendingForLoadList.PushBack ( resource.mPendingLink );
			break;
		
		case MOAIGfxPipelineClerkGL::DRAWING_PIPELINE:
			this->mPendingForDrawList.PushBack ( resource.mPendingLink );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceClerkGL::Update () {

	MOAIGfxPipelineClerkGL& pipelineClerk = this->GetGfxPipelineClerkGL ();

	if ( this->mDeleterStack.GetTop () || this->mPendingForLoadList.Count ()) {
	
		ZLGfx* gfxLoading = pipelineClerk.SelectDrawingAPI ( MOAIGfxPipelineClerkGL::LOADING_PIPELINE );
		
		if ( gfxLoading ) {
		
			ZGL_COMMENT ( *gfxLoading, "RESOURCE MGR LOADING PIPELINE UPDATE" );
			this->ProcessDeleters ( *gfxLoading );
			this->ProcessPending ( *gfxLoading, this->mPendingForLoadList );
			this->GetGfxStateGPUCacheGL ().UnbindAll ();
		}
	}
	
	if ( this->mPendingForDrawList.Count ()) {
	
		ZLGfx* gfxDrawing = pipelineClerk.SelectDrawingAPI ( MOAIGfxPipelineClerkGL::DRAWING_PIPELINE );
		
		if ( gfxDrawing ) {
		
			ZGL_COMMENT ( *gfxDrawing, "RESOURCE MGR DRAWING PIPELINE UPDATE" );
			this->ProcessPending ( *gfxDrawing, this->mPendingForDrawList );
			this->GetGfxStateGPUCacheGL ().UnbindAll ();
		}
	}

	// TODO: think about cases where we can get async results back on the
	// same display list so we can remove the one-frame lag when creating resources
	// in retained mode.
}

//----------------------------------------------------------------//
MOAIGfxResourceClerkGL::MOAIGfxResourceClerkGL () {
}

//----------------------------------------------------------------//
MOAIGfxResourceClerkGL::~MOAIGfxResourceClerkGL () {

	this->DiscardResources ();
}
