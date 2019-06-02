// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxResourceGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_PipelineClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_ResourceClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_GPUCacheGL.h>

//================================================================//
// MOAIGfxMgrGL_ResourceClerkGL
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrGL_ResourceClerkGL::DeleteOrDiscard ( const ZLGfxHandle& handle, bool shouldDelete ) {

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
void MOAIGfxMgrGL_ResourceClerkGL::DiscardResources () {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( false );
	}
	
	ZLSize top = this->mDeleterStack.GetTop ();

	for ( ZLIndex i = ZLIndexOp::ZERO; i < top; ++i ) {
		ZLGfx::DiscardResource ( this->mDeleterStack [ i ]);
	}
	this->mDeleterStack.Reset ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_ResourceClerkGL::InsertGfxResource ( MOAIGfxResourceGL& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_ResourceClerkGL::MOAIGfxMgrGL_ResourceClerkGL () {
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_ResourceClerkGL::~MOAIGfxMgrGL_ResourceClerkGL () {

	this->DiscardResources ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_ResourceClerkGL::ProcessDeleters ( ZLGfx& gfx ) {

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
void MOAIGfxMgrGL_ResourceClerkGL::ProcessPending ( ZLGfx& gfx, ZLLeanList < MOAIGfxResourceGL* > &list ) {
	
	this->ProcessDeleters ( gfx );
	
	ResourceIt resourceIt = list.Head ();
	while ( resourceIt ) {
		MOAIGfxResourceGL* resource = resourceIt->Data ();
		resourceIt = resourceIt->Next ();
	
		resource->Affirm ();
	}
	list.Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_ResourceClerkGL::PurgeResources ( u32 age ) {
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_ResourceClerkGL::RemoveGfxResource ( MOAIGfxResourceGL& resource ) {

	this->mResources.Remove ( resource.mMasterLink );
	this->mPendingForLoadList.Remove ( resource.mPendingLink );
	this->mPendingForDrawList.Remove ( resource.mPendingLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void MOAIGfxMgrGL_ResourceClerkGL::RenewResources () {

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_ResourceClerkGL::ScheduleGPUAffirm ( MOAIGfxResourceGL& resource, u32 listID ) {

	switch ( listID ) {

		case MOAIGfxMgrGL_PipelineClerkGL::LOADING_PIPELINE:
			this->mPendingForLoadList.PushBack ( resource.mPendingLink );
			break;
		
		case MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE:
			this->mPendingForDrawList.PushBack ( resource.mPendingLink );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_ResourceClerkGL::Update () {

	MOAIGfxMgrGL_PipelineClerkGL& pipelineClerk = this->GetPipelineClerkGL ();

	if ( this->mDeleterStack.GetTop () || this->mPendingForLoadList.Count ()) {
	
		ZLGfx* gfxLoading = pipelineClerk.SelectDrawingAPI ( MOAIGfxMgrGL_PipelineClerkGL::LOADING_PIPELINE );
		
		if ( gfxLoading ) {
		
			ZGL_COMMENT ( *gfxLoading, "RESOURCE MGR LOADING PIPELINE UPDATE" );
			this->ProcessDeleters ( *gfxLoading );
			this->ProcessPending ( *gfxLoading, this->mPendingForLoadList );
			this->GetGPUCache ().UnbindAll ();
		}
	}
	
	if ( this->mPendingForDrawList.Count ()) {
	
		ZLGfx* gfxDrawing = pipelineClerk.SelectDrawingAPI ( MOAIGfxMgrGL_PipelineClerkGL::DRAWING_PIPELINE );
		
		if ( gfxDrawing ) {
		
			ZGL_COMMENT ( *gfxDrawing, "RESOURCE MGR DRAWING PIPELINE UPDATE" );
			this->ProcessPending ( *gfxDrawing, this->mPendingForDrawList );
			this->GetGPUCache ().UnbindAll ();
		}
	}

	// TODO: think about cases where we can get async results back on the
	// same display list so we can remove the one-frame lag when creating resources
	// in retained mode.
}
