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
// TODO: doxygen
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
// TODO: doxygen
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

	this->mResources.PushBack ( resource.mLink );
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
	
		resource->DoGPUCreate ();
		resource->mScheduled = false;
		
		this->mResources.PushBack ( resource->mLink );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::PurgeResources ( u32 age ) {
	
	ResourceIt resourceIt = this->mResources.Head ();
	for ( ; resourceIt; resourceIt = resourceIt->Next ()) {
		resourceIt->Data ()->Purge ( age );
	}
	this->mResources.Clear ();
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

	this->mResources.Remove ( resource.mLink );
	this->mPendingForLoadList.Remove ( resource.mLink );
	this->mPendingForDrawList.Remove ( resource.mLink );
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

		case MOAIGfxDevice::LOADING_LIST:
			this->mPendingForLoadList.PushBack ( resource.mLink );
			break;
		
		case MOAIGfxDevice::DRAWING_LIST:
			this->mPendingForDrawList.PushBack ( resource.mLink );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResourceMgr::Update () {

	ZLGfxDevice::Begin ();

	MOAIGfxDevice::Get ().SelectList ( MOAIGfxDevice::LOADING_LIST );
	this->ProcessDeleters ();
	this->ProcessPending ( this->mPendingForLoadList );
	
	MOAIGfxDevice::Get ().SelectList ( MOAIGfxDevice::DRAWING_LIST );
	this->ProcessPending ( this->mPendingForDrawList );
	
	ZLGfxDevice::End ();
}
