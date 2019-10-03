// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAISemaphoreVK.h>

//================================================================//
// MOAISemaphoreVK
//================================================================//

//----------------------------------------------------------------//
void MOAISemaphoreVK::Initialize ( MOAILogicalDeviceVK& logicalDevice) {

	VkSemaphoreCreateInfo semaphoreCreateInfo = MOAIGfxStructVK::semaphoreCreateInfo ();
	VK_CHECK_RESULT ( vkCreateSemaphore ( logicalDevice, &semaphoreCreateInfo, NULL, &this->mSemaphore ));
    this->SetProvider < MOAILogicalDeviceVK >( logicalDevice );
}

//----------------------------------------------------------------//
MOAISemaphoreVK::MOAISemaphoreVK () :
	mSemaphore ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAISemaphoreVK::~MOAISemaphoreVK () {

	if ( this->HasProvider < MOAILogicalDeviceVK >()) {
		vkDestroySemaphore ( this->GetProvider < MOAILogicalDeviceVK >(), this->mSemaphore, NULL );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
