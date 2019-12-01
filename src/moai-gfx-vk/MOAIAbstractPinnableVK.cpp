// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractPinnableVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIAbstractPinnableVK
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractPinnableVK::IsPinned () {
	return ( this->mCommandBufferRefCount > 0 );
}

//----------------------------------------------------------------//
MOAIAbstractPinnableVK::MOAIAbstractPinnableVK () :
	mCommandBufferRefCount ( 0 ) {
}

//----------------------------------------------------------------//
MOAIAbstractPinnableVK::~MOAIAbstractPinnableVK () {

	assert ( !this->IsPinned ());
}

//----------------------------------------------------------------//
void MOAIAbstractPinnableVK::Pin () {

	this->Retain ();
	this->mCommandBufferRefCount++;
}

//----------------------------------------------------------------//
void MOAIAbstractPinnableVK::Unpin () {

	assert ( this->IsPinned ());
	this->mCommandBufferRefCount--;
	if ( this->mCommandBufferRefCount == 0 ) {
		this->MOAIAbstractSnapshotVK_OnUnpin ();
	}
	this->Release ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
