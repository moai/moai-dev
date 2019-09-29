// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIFenceVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>

//================================================================//
// MOAIFenceVK
//================================================================//

//----------------------------------------------------------------//
void MOAIFenceVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, bool signaled ) {

	VkFenceCreateInfo fenceCreateInfo = MOAIGfxStructVK::fenceCreateInfo ( signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0 );
	VK_CHECK_RESULT ( vkCreateFence ( logicalDevice, &fenceCreateInfo, NULL, &this->mFence ));
    logicalDevice.AddClient ( logicalDevice, *this );
}

//----------------------------------------------------------------//
MOAIFenceVK::MOAIFenceVK () :
	mFence ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIFenceVK::~MOAIFenceVK () {

	this->Finalize ();
}

//----------------------------------------------------------------//
void MOAIFenceVK::Wait () {

	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
	VK_CHECK_RESULT ( vkWaitForFences ( logicalDevice, 1, &this->mFence, VK_TRUE, UINT64_MAX ));
	VK_CHECK_RESULT ( vkResetFences ( logicalDevice, 1, &this->mFence ));
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFenceVK::MOAIAbstractLifecycleClientVK_Finalize () {

	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
	vkDestroyFence ( logicalDevice, this->mFence, NULL );
	logicalDevice.RemoveClient ( *this );
}
