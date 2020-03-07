// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxResourceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_PipelineClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_ResourceClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>

//================================================================//
// MOAIGfxMgrVK_ResourceClerkVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::DeleteOrDiscard ( const ZLGfxHandle& handle, bool shouldDelete ) {

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
void MOAIGfxMgrVK_ResourceClerkVK::DiscardResources () {

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->MOAIGfxResourceVK_OnGPUDeleteOrDiscard ( false );
	}
	
	ZLSize top = this->mDeleterStack.GetTop ();

	for ( ZLIndex i = 0; i < top; ++i ) {
		ZLGfx::DiscardResource ( this->mDeleterStack [ i ]);
	}
	this->mDeleterStack.Reset ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::InsertGfxResource ( MOAIGfxResourceVK& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_ResourceClerkVK::MOAIGfxMgrVK_ResourceClerkVK () {
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_ResourceClerkVK::~MOAIGfxMgrVK_ResourceClerkVK () {

	this->DiscardResources ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::ProcessDeleters ( ZLGfx& gfx ) {

	ZLSize top = this->mDeleterStack.GetTop ();

	if ( top ) {
	
		gfx.Flush ();
	
		for ( ZLIndex i = 0; i < top; ++i ) {
			gfx.DeleteResource ( this->mDeleterStack [ i ]);
		}
		this->mDeleterStack.Reset ();
	
		gfx.Flush ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::ProcessPending ( ZLGfx& gfx, ZLLeanList < MOAIGfxResourceVK* > &list ) {
	
	this->ProcessDeleters ( gfx );
	
	ResourceIt resourceIt = list.Head ();
	while ( resourceIt ) {
		MOAIGfxResourceVK* resource = resourceIt->Data ();
		resourceIt = resourceIt->Next ();
	
		resource->Affirm ();
	}
	list.Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::PurgeResources ( u32 age ) {
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::RemoveGfxResource ( MOAIGfxResourceVK& resource ) {

	this->mResources.Remove ( resource.mMasterLink );
	this->mPendingForLoadList.Remove ( resource.mPendingLink );
	this->mPendingForDrawList.Remove ( resource.mPendingLink );
}

//----------------------------------------------------------------//
// this gets called when the graphics context is renewed
void MOAIGfxMgrVK_ResourceClerkVK::RenewResources () {

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::ScheduleGPUAffirm ( MOAIGfxResourceVK& resource, u32 listID ) {

	switch ( listID ) {

		case MOAIGfxMgrVK_PipelineClerkVK::LOADING_QUEUE:
			this->mPendingForLoadList.PushBack ( resource.mPendingLink );
			break;
		
		case MOAIGfxMgrVK_PipelineClerkVK::DRAWING_QUEUE:
			this->mPendingForDrawList.PushBack ( resource.mPendingLink );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_ResourceClerkVK::Update () {

	MOAIGfxMgrVK_PipelineClerkVK& pipelineClerk = this->GetPipelineClerkVK ();

	if ( this->mDeleterStack.GetTop () || this->mPendingForLoadList.Count ()) {
	
		ZLGfx* gfxLoading = pipelineClerk.SelectDrawingAPI ( MOAIGfxMgrVK_PipelineClerkVK::LOADING_QUEUE );
		
		if ( gfxLoading ) {
		
			ZGL_COMMENT ( *gfxLoading, "RESOURCE MGR LOADING PIPELINE UPDATE" );
			this->ProcessDeleters ( *gfxLoading );
			this->ProcessPending ( *gfxLoading, this->mPendingForLoadList );
			this->GetGPUCache ().UnbindAll ();
		}
	}
	
	if ( this->mPendingForDrawList.Count ()) {
	
		ZLGfx* gfxDrawing = pipelineClerk.SelectDrawingAPI ( MOAIGfxMgrVK_PipelineClerkVK::DRAWING_QUEUE );
		
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
