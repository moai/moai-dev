// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	purgeResources
	@text	Purges all resources older that a given age (in render cycles).
			If age is 0 then all resources are purged.
 
	@opt	number age		Default value is 0.
	@out	nil
*/
int MOAIGfxResourceMgr::_purgeResources ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 age = state.GetValue < u32 >( 1, 0 );

	ZLGfx& gfx = MOAIGfxDevice::GetDrawingAPI ();

	ZLGfxDevice::Begin ();
	MOAIGfxResourceMgr::Get ().PurgeResources ( age );
	ZLGfxDevice::End ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	renewResources
	@text	Renews all resources.
 
	@out	nil
*/
int MOAIGfxResourceMgr::_renewResources ( lua_State* L ) {
	MOAILuaState state ( L );

	ZLGfx& gfx = MOAIGfxDevice::GetDrawingAPI ();

	ZLGfxDevice::Begin ();
	MOAIGfxResourceMgr::Get ().RenewResources ();
	ZLGfxDevice::End ();
	
	return 0;
}

//================================================================//
// MOAIGfxResourceMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::InsertGfxResource ( MOAIGfxResource& resource ) {

	this->mResources.PushBack ( resource.mMasterLink );
}

//----------------------------------------------------------------//
MOAIGfxResourceMgr::MOAIGfxResourceMgr () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGfxResourceMgr::~MOAIGfxResourceMgr () {
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::ProcessDeleters () {

	ZLGfx& gfx = MOAIGfxDevice::GetDrawingAPI ();

	ZLGfxDevice::Begin ();
	
	u32 top = this->mDeleterStack.GetTop ();
	
	if ( top ) {
		gfx.Flush ();
	}
	
	for ( u32 i = 0; i < top; ++i ) {
		ZLGfxHandle* handle = this->mDeleterStack [ i ];
		gfx.DeleteHandle ( handle );
	}
	this->mDeleterStack.Reset ();
	
	if ( top ) {
		gfx.Flush ();
	}
	
	ZLGfxDevice::End ();
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
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::PushDeleter ( ZLGfxHandle* handle ) {

	if ( handle ) {
		this->mDeleterStack.Push ( handle );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "purgeResources",				_purgeResources },
		{ "renewResources",				_renewResources },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
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

	this->mDeleterStack.Reset ();

	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Renew ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::ScheduleGPUAffirm ( MOAIGfxResource& resource, u32 listID ) {

	switch ( listID ) {

		case MOAIGfxDevice::LOADING_PIPELINE:
			this->mPendingForLoadList.PushBack ( resource.mPendingLink );
			break;
		
		case MOAIGfxDevice::DRAWING_PIPELINE:
			this->mPendingForDrawList.PushBack ( resource.mPendingLink );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::Update () {

	ZLGfxDevice::Begin ();

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	if ( this->mDeleterStack.GetTop () || this->mPendingForLoadList.Count ()) {
	
		ZLGfx& gfxLoading = gfxDevice.SelectDrawingAPI ( MOAIGfxDevice::LOADING_PIPELINE, true );
		
		ZGL_COMMENT ( gfxLoading, "RESOURCE MGR LOADING PIPELINE UPDATE" );
		gfxDevice.ResetState ();
		this->ProcessDeleters ();
		this->ProcessPending ( this->mPendingForLoadList );
		gfxDevice.UnbindAll ();
	}
	
	if ( this->mPendingForDrawList.Count ()) {
	
		ZLGfx& gfxDrawing = gfxDevice.SelectDrawingAPI ( MOAIGfxDevice::DRAWING_PIPELINE, true );
		
		ZGL_COMMENT ( gfxDrawing, "RESOURCE MGR DRAWING PIPELINE UPDATE" );
		gfxDevice.ResetState ();
		this->ProcessPending ( this->mPendingForDrawList );
		gfxDevice.UnbindAll ();
	}

	// TODO: think about cases where we can get async results back on the
	// same display list so we can remove the one-frame lag when creating resources
	// in retained mode.
	
	ZLGfxDevice::End ();
}
