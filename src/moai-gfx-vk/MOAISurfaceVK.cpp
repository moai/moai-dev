// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAISurfaceVK.h>

//================================================================//
// MOAISurfaceVK
//================================================================//

//----------------------------------------------------------------//
// Destroy and free Vulkan resources used for the swapchain
void MOAISurfaceVK::Cleanup ( MOAIGfxInstanceVK& instance ) {
	
	if ( this->mSurface != VK_NULL_HANDLE ) {
		vkDestroySurfaceKHR ( instance, this->mSurface, nullptr );
	}
	this->mSurface = VK_NULL_HANDLE;
}

//----------------------------------------------------------------//
void MOAISurfaceVK::Init ( VkSurfaceKHR surface ) {

	this->mSurface = surface;
}

//----------------------------------------------------------------//
MOAISurfaceVK::MOAISurfaceVK () :
	mSurface ( VK_NULL_HANDLE ) {
}
