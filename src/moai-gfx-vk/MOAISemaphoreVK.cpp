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
    logicalDevice.AddClient ( logicalDevice, *this );
}

//----------------------------------------------------------------//
MOAISemaphoreVK::MOAISemaphoreVK () :
	mSemaphore ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAISemaphoreVK::~MOAISemaphoreVK () {

	this->Finalize ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISemaphoreVK::MOAIAbstractLifecycleClientVK_Finalize () {

	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
	vkDestroySemaphore( logicalDevice, this->mSemaphore, NULL );
	logicalDevice.RemoveClient ( *this );
}
