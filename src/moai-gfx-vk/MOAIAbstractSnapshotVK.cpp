// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIAbstractSnapshotVK
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractSnapshotVK::IsPinned () {
	return ( this->mCommandBufferRefCount > 0 );
}

//----------------------------------------------------------------//
MOAIAbstractSnapshotVK::MOAIAbstractSnapshotVK () :
	mCommandBufferRefCount ( 0 ) {
}

//----------------------------------------------------------------//
MOAIAbstractSnapshotVK::~MOAIAbstractSnapshotVK () {
}

//----------------------------------------------------------------//
void MOAIAbstractSnapshotVK::Pin ( MOAICommandBufferVK& commandBuffer ) {

	this->Retain ();
	this->mCommandBufferRefCount++;
	this->MOAIAbstractSnapshotVK_OnPin ( commandBuffer );
}

//----------------------------------------------------------------//
void MOAIAbstractSnapshotVK::Unpin () {

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
