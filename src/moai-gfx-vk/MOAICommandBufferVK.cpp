// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractCommandBufferResourceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAICommandBufferVK.h>

//================================================================//
// MOAICommandBufferVK
//================================================================//

//----------------------------------------------------------------//
void MOAICommandBufferVK::Invalidate () {
	this->mIsValid = false;
}

//----------------------------------------------------------------//
MOAICommandBufferVK::MOAICommandBufferVK () :
	mCommandBuffer ( VK_NULL_HANDLE ),
	mIsValid ( false ) {
}

//----------------------------------------------------------------//
MOAICommandBufferVK::~MOAICommandBufferVK () {
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::PinResource ( MOAIAbstractCommandBufferResourceVK& resource ) {

	resource.ForceUnpin ();
	this->mResources.PushBack ( resource.mLink );
	resource.mCommandBuffer = this;
}

//----------------------------------------------------------------//
void MOAICommandBufferVK::UnpinResources () {

	while ( this->mResources.Count ()) {
		this->mResources.Front ()->ForceUnpin ();
	}
}

