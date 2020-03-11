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
    this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
}

//----------------------------------------------------------------//
MOAIFenceVK::MOAIFenceVK () :
	mFence ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIFenceVK::~MOAIFenceVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIFenceVK::Wait () {

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	VK_CHECK_RESULT ( vkWaitForFences ( logicalDevice, 1, &this->mFence, VK_TRUE, UINT64_MAX ));
	VK_CHECK_RESULT ( vkResetFences ( logicalDevice, 1, &this->mFence ));
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFenceVK::_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyFence ( logicalDevice, this->mFence, NULL );
	}
}
