// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>

//================================================================//
// MOAIPhysicalDeviceVK
//================================================================//

//----------------------------------------------------------------//
VkResult MOAILogicalDeviceVK::AcquireNextImageKHR ( VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex ) {

	return this->mAcquireNextImageKHR ( this->mDevice, swapchain, timeout, semaphore, fence, pImageIndex );
}

//----------------------------------------------------------------//
VkResult MOAILogicalDeviceVK::CreateSwapchainKHR ( const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain ) {

	return this->mCreateSwapchainKHR ( this->mDevice, pCreateInfo, pAllocator, pSwapchain );
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::DestroySwapchainKHR ( VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator ) {

	this->mDestroySwapchainKHR ( this->mDevice, swapchain, pAllocator );
}

//----------------------------------------------------------------//
VkResult MOAILogicalDeviceVK::GetSwapchainImagesKHR ( VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages ) {

	return this->mGetSwapchainImagesKHR ( this->mDevice, swapchain, pSwapchainImageCount, pSwapchainImages );
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::Init ( MOAIPhysicalDeviceVK& physicalDevice, VkQueueFlags requestedQueueTypes, bool requestPresent ) {

	assert ( physicalDevice );

    // Desired queues need to be requested upon logical device creation
    // Due to differing queue family configurations of Vulkan implementations this can be a bit tricky, especially if the application
    // requests different queue types
	
	// make sure all requested queue types are known
	assert (( requestedQueueTypes & MOAIGfxQueueSetVK::KNOWN_QUEUE_TYPES ) == requestedQueueTypes );
	
	MOAIGfxQueueSetVK queueSet;
	physicalDevice.FindQueues ( queueSet, requestedQueueTypes, requestPresent );
	assert (( queueSet.mQueueTypes & requestedQueueTypes ) == requestedQueueTypes );
	assert ( queueSet.mCanPresent == requestPresent );
	
	std::vector < const char* > deviceExtensions;
	deviceExtensions.push_back ( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

//    // Enable the debug marker extension if it is present (likely meaning a debugging tool is present)
//    if ( supportedExtensions.find ( VK_EXT_DEBUG_MARKER_EXTENSION_NAME ) != supportedExtensions.end ()) {
//        deviceExtensions.push_back ( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
//        vks::debugmarker::setup ( mDevice );
//    }
	
    // Create the logical device representation
	VkDeviceCreateInfo deviceCreateInfo = MOAIGfxStructVK::deviceCreateInfo (
		queueSet.mCreateInfos.GetBuffer (),
		( u32 )queueSet.mCreateInfos.Size (),
		NULL, // layers
		0, // layerCount
		deviceExtensions.data (),
		( u32 )deviceExtensions.size ()
	);

    VK_CHECK_RESULT ( vkCreateDevice ( physicalDevice, &deviceCreateInfo, nullptr, &this->mDevice ));
	
	this->mAcquireNextImageKHR		= VK_GET_DEVICE_PROC_ADDR ( this->mDevice, vkAcquireNextImageKHR );
	this->mCreateSwapchainKHR		= VK_GET_DEVICE_PROC_ADDR ( this->mDevice, vkCreateSwapchainKHR );
	this->mDestroySwapchainKHR		= VK_GET_DEVICE_PROC_ADDR ( this->mDevice, vkDestroySwapchainKHR );
	this->mGetSwapchainImagesKHR	= VK_GET_DEVICE_PROC_ADDR ( this->mDevice, vkGetSwapchainImagesKHR );
	this->mQueuePresentKHR			= VK_GET_DEVICE_PROC_ADDR ( this->mDevice, vkQueuePresentKHR );
	
	if ( requestedQueueTypes & VK_QUEUE_COMPUTE_BIT ) {
		this->InitQueueAndPool ( this->mCompute, queueSet.mComputeQueueIndex );
    }
	
	if ( requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT ) {
		this->InitQueueAndPool ( this->mGraphics, queueSet.mGraphicsQueueIndex );
    }
	
    if ( requestPresent ) {
		this->InitQueueAndPool ( this->mPresent, queueSet.mPresentQueueIndex );
    }
	
	if ( requestedQueueTypes & VK_QUEUE_TRANSFER_BIT ) {
		this->InitQueueAndPool ( this->mTransfer, queueSet.mTransferQueueIndex );
    }
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::InitQueueAndPool ( MOAIGfxQueueAndPoolVK& queueAndPool, u32 index ) {

	queueAndPool.mIndex = index;
	vkGetDeviceQueue ( this->mDevice, index, 0, &queueAndPool.mQueue );
	
	if ( !this->mCommandPools.contains ( index )) {
		VkCommandPoolCreateInfo commandPoolCreateInfo = MOAIGfxStructVK::commandPoolCreateInfo ( index, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );
		VK_CHECK_RESULT ( vkCreateCommandPool ( this->mDevice, &commandPoolCreateInfo, NULL, &this->mCommandPools [ index ] ));
	}
	queueAndPool.mPool = this->mCommandPools [ index ];
}

//----------------------------------------------------------------//
MOAILogicalDeviceVK::MOAILogicalDeviceVK () :
	mDevice ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
VkResult MOAILogicalDeviceVK::QueuePresentKHR ( VkQueue queue, const VkPresentInfoKHR* pPresentInfo ) {

	return this->mQueuePresentKHR ( queue, pPresentInfo );
}
