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
	return ( this->mCommandBuffer != NULL );
}

//----------------------------------------------------------------//
MOAIAbstractSnapshotVK::MOAIAbstractSnapshotVK () :
	mLink ( this ),
	mCommandBuffer ( NULL ) {
}

//----------------------------------------------------------------//
MOAIAbstractSnapshotVK::~MOAIAbstractSnapshotVK () {
	this->Remove ();
}

//----------------------------------------------------------------//
void MOAIAbstractSnapshotVK::Pin ( MOAICommandBufferVK& commandBuffer ) {

	this->Retain ();
	this->Unpin ();
	commandBuffer.mSnapshots.PushBack ( this->mLink );
	this->mCommandBuffer = &commandBuffer;
}

//----------------------------------------------------------------//
void MOAIAbstractSnapshotVK::Remove () {

	if ( this->mCommandBuffer ) {
		this->mCommandBuffer->Invalidate ();
		this->mLink.Remove ();
		this->mCommandBuffer = NULL;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractSnapshotVK::Unpin () {

	if ( this->mCommandBuffer ) {
		this->Remove ();
		this->MOAIAbstractSnapshotVK_OnUnpin ();
		this->Release ();
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {
}
