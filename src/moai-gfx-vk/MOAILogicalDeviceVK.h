// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILOGICALDEVICEVK_H
#define MOAILOGICALDEVICEVK_H

#include <vector>
#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>
#include <moai-gfx-vk/MOAISurfaceVK.h>

//================================================================//
// MOAIGfxQueueAndPoolVK
//================================================================//
class MOAIGfxQueueAndPoolVK {
public:

	u32				mIndex;
	VkCommandPool	mPool;
	VkQueue			mQueue;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return (( this->mPool != VK_NULL_HANDLE ) && ( this->mQueue != VK_NULL_HANDLE ));
	}
	
	//----------------------------------------------------------------//
	MOAIGfxQueueAndPoolVK () :
		mIndex (( u32 )-1 ),
		mPool ( VK_NULL_HANDLE ),
		mQueue ( VK_NULL_HANDLE ) {
	}
};

//================================================================//
// MOAILogicalDeviceVK
//================================================================//
class MOAILogicalDeviceVK {
private:

	PFN_vkAcquireNextImageKHR		mAcquireNextImageKHR;
	PFN_vkCreateSwapchainKHR		mCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR		mDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR		mGetSwapchainImagesKHR;
	PFN_vkQueuePresentKHR			mQueuePresentKHR;

	//----------------------------------------------------------------//
	void			InitQueueAndPool				( MOAIGfxQueueAndPoolVK& queueAndPool, u32 index );

public:

	VkDevice			mDevice;
	
	MOAIGfxQueueAndPoolVK	mGraphics;
	MOAIGfxQueueAndPoolVK	mCompute;
	MOAIGfxQueueAndPoolVK	mPresent;
	MOAIGfxQueueAndPoolVK	mTransfer;
	
	STLMap < u32, VkCommandPool > mCommandPools;
	
	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mDevice != VK_NULL_HANDLE );
	}

	//----------------------------------------------------------------//
	operator VkDevice () {
		return this->mDevice;
	}

	//----------------------------------------------------------------//
	VkResult		AcquireNextImageKHR				( VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex );
	VkResult		CreateSwapchainKHR				( const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain );
	void			DestroySwapchainKHR				( VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator );
	VkResult		GetSwapchainImagesKHR			( VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages );
	void			Init							( MOAIPhysicalDeviceVK& physicalDevice, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT, bool requestPresentat = true );
					MOAILogicalDeviceVK				();
	VkResult		QueuePresentKHR					( VkQueue queue, const VkPresentInfoKHR* pPresentInfo );
};

#endif
