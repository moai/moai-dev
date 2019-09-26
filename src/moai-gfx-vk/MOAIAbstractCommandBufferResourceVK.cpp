// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractCommandBufferResourceVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIAbstractCommandBufferResourceVK
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractCommandBufferResourceVK::ForceRemove () {

	if ( this->mCommandBuffer ) {
		this->mCommandBuffer->Invalidate ();
		this->mLink.Remove ();
		this->mCommandBuffer = NULL;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractCommandBufferResourceVK::ForceUnpin () {

	if ( this->mCommandBuffer ) {
		this->ForceRemove ();
		this->MOAIAbstractCommandBufferResourceVK_Unpin ();
	}
}

//----------------------------------------------------------------//
bool MOAIAbstractCommandBufferResourceVK::IsPinned () {
	return ( this->mCommandBuffer != NULL );
}

//----------------------------------------------------------------//
MOAIAbstractCommandBufferResourceVK::MOAIAbstractCommandBufferResourceVK () :
	mLink ( this ),
	mCommandBuffer ( NULL ) {
}

//----------------------------------------------------------------//
MOAIAbstractCommandBufferResourceVK::~MOAIAbstractCommandBufferResourceVK () {
	this->ForceRemove ();
}
