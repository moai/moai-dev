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
void MOAIAbstractSnapshotVK::ForceRemove () {

	if ( this->mCommandBuffer ) {
		this->mCommandBuffer->Invalidate ();
		this->mLink.Remove ();
		this->mCommandBuffer = NULL;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractSnapshotVK::ForceUnpin () {

	if ( this->mCommandBuffer ) {
		this->ForceRemove ();
		this->MOAIAbstractSnapshotVK_OnUnpin ();
	}
}

//----------------------------------------------------------------//
bool MOAIAbstractSnapshotVK::IsPinned () {
	return ( this->mCommandBuffer != NULL );
}

//----------------------------------------------------------------//
MOAIAbstractSnapshotVK::MOAIAbstractSnapshotVK () :
	mLink ( this ),
	mCommandBuffer ( NULL ) {
}

//----------------------------------------------------------------//
MOAIAbstractSnapshotVK::~MOAIAbstractSnapshotVK () {
	this->ForceRemove ();
}
