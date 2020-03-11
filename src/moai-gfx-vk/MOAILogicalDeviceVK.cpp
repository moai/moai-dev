// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>

//================================================================//
// MOAIPhysicalDeviceVK
//================================================================//

//----------------------------------------------------------------//
VkResult MOAILogicalDeviceVK::AcquireNextImageKHR ( VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, ZLIndex& index ) {

	u32 index32 = 0;
	VkResult result = this->mAcquireNextImageKHR ( this->mDevice, swapchain, timeout, semaphore, fence, &index32 );
	index = index;
	return result;
}

//----------------------------------------------------------------//
VkDeviceMemory MOAILogicalDeviceVK::AllocateMemory ( const VkMemoryAllocateInfo& allocateInfo, const VkAllocationCallbacks* pAllocator ) {

	VkDeviceMemory deviceMemory;
	VK_CHECK_RESULT ( vkAllocateMemory ( this->mDevice, &allocateInfo, pAllocator, &deviceMemory ));
	return deviceMemory;
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::BindImageMemory ( VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset ) {

	VK_CHECK_RESULT ( vkBindImageMemory ( this->mDevice, image, memory, memoryOffset ));
}

//----------------------------------------------------------------//
VkImage MOAILogicalDeviceVK::CreateImage ( const VkImageCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator  ) {

	VkImage image;
	VK_CHECK_RESULT ( vkCreateImage ( this->mDevice, &createInfo, pAllocator, &image ));
	return image;
}

//----------------------------------------------------------------//
VkImageView MOAILogicalDeviceVK::CreateImageView ( const VkImageViewCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator  ) {

	VkImageView imageView;
	VK_CHECK_RESULT ( vkCreateImageView ( this->mDevice, &createInfo, pAllocator, &imageView ));
	return imageView;
}

//----------------------------------------------------------------//
VkFramebuffer MOAILogicalDeviceVK::CreateFramebuffer ( const VkFramebufferCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator ) {

	VkFramebuffer framebuffer;
	VK_CHECK_RESULT ( vkCreateFramebuffer ( this->mDevice, &createInfo, pAllocator, &framebuffer ));
	return framebuffer;
}

//----------------------------------------------------------------//
VkRenderPass MOAILogicalDeviceVK::CreateRenderPass ( const VkRenderPassCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator ) {

	VkRenderPass renderPass;
	VK_CHECK_RESULT ( vkCreateRenderPass ( this->mDevice, &createInfo, pAllocator, &renderPass ));
	return renderPass;
}

//----------------------------------------------------------------//
VkSwapchainKHR MOAILogicalDeviceVK::CreateSwapchainKHR ( const VkSwapchainCreateInfoKHR& createInfo, const VkAllocationCallbacks* pAllocator ) {

	VkSwapchainKHR swapChain;
	VK_CHECK_RESULT ( this->mCreateSwapchainKHR ( this->mDevice, &createInfo, pAllocator, &swapChain ));
	return swapChain;
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::DestroySwapchainKHR ( VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator ) {

	this->mDestroySwapchainKHR ( this->mDevice, swapchain, pAllocator );
}

//----------------------------------------------------------------//
VkMemoryRequirements MOAILogicalDeviceVK::GetImageMemoryRequirements ( VkImage image ) {
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::GetSwapchainImagesKHR ( VkSwapchainKHR swapchain, ZLSize imageCount, VkImage* images ) {

	u32 swapchainImageCount = ( u32 )imageCount;
	VK_CHECK_RESULT ( this->mGetSwapchainImagesKHR ( this->mDevice, swapchain, &swapchainImageCount, images ));
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::Initialize ( MOAIPhysicalDeviceVK& physicalDevice, VkQueueFlags requestedQueueTypes, bool requestPresent ) {

	assert ( physicalDevice );
	this->SetDependency < MOAIPhysicalDeviceVK >( physicalDevice );

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
		this->InitQueue ( this->mQueues [ COMPUTE_QUEUE ], queueSet.mComputeQueueIndex );
    }
	
	if ( requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT ) {
		this->InitQueue ( this->mQueues [ GRAPHICS_QUEUE ], queueSet.mGraphicsQueueIndex );
    }
	
    if ( requestPresent ) {
		this->InitQueue ( this->mQueues [ PRESENT_QUEUE ], queueSet.mPresentQueueIndex );
    }
	
	if ( requestedQueueTypes & VK_QUEUE_TRANSFER_BIT ) {
		this->InitQueue ( this->mQueues [ TRANSFER_QUEUE ], queueSet.mTransferQueueIndex );
    }
}

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::InitQueue ( MOAIQueueVK& queue, u32 index ) {

	queue.mIndex = index;
	vkGetDeviceQueue ( this->mDevice, index, 0, &queue.mQueue );
	
	if ( !this->mCommandPools.contains ( index )) {
		VkCommandPoolCreateInfo commandPoolCreateInfo = MOAIGfxStructVK::commandPoolCreateInfo ( index, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT );
		VK_CHECK_RESULT ( vkCreateCommandPool ( this->mDevice, &commandPoolCreateInfo, NULL, &this->mCommandPools [ index ]));
	}
	queue.mPool = this->mCommandPools [ index ];
	queue.SetDependency < MOAILogicalDeviceVK >( *this );
}

//----------------------------------------------------------------//
MOAILogicalDeviceVK::MOAILogicalDeviceVK () :
	mDevice ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAILogicalDeviceVK::~MOAILogicalDeviceVK () {

	this->Destruct ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAILogicalDeviceVK::Visitor_Finalize () {

	vkDestroyDevice ( this->mDevice, NULL );
}
