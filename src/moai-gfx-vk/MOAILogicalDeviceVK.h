// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILOGICALDEVICEVK_H
#define MOAILOGICALDEVICEVK_H

#include <vector>
#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>
#include <moai-gfx-vk/MOAIQueueVK.h>
#include <moai-gfx-vk/MOAISurfaceVK.h>

//================================================================//
// MOAILogicalDeviceVK
//================================================================//
class MOAILogicalDeviceVK :
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIPhysicalDeviceVK > {
private:

	friend class MOAIPhysicalDeviceVK;
	friend class MOAIQueueVK;

	enum QueueID {
		COMPUTE_QUEUE,
		GRAPHICS_QUEUE,
		PRESENT_QUEUE,
		TRANSFER_QUEUE,
		TOTAL_QUEUES,
	};

	PFN_vkAcquireNextImageKHR		mAcquireNextImageKHR;
	PFN_vkCreateSwapchainKHR		mCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR		mDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR		mGetSwapchainImagesKHR;
	PFN_vkQueuePresentKHR			mQueuePresentKHR;

	STLMap < u32, VkCommandPool >	mCommandPools;

	MOAIQueueVK						mQueues [ TOTAL_QUEUES ];

	VkDevice						mDevice;

	//----------------------------------------------------------------//
	void			InitQueue		( MOAIQueueVK& queueAndPool, u32 index );
	
	//----------------------------------------------------------------//
	void 			_Finalize		();

public:
	
	IMPLEMENT_DEPENDS_ON ( MOAILogicalDeviceVK )
	
	GET ( MOAIQueueVK&, ComputeQueue, this->mQueues [ COMPUTE_QUEUE ]);
	GET ( MOAIQueueVK&, GraphicsQueue, this->mQueues [ GRAPHICS_QUEUE ]);
	GET ( MOAIQueueVK&, PresentQueue, this->mQueues [ PRESENT_QUEUE ]);
	GET ( MOAIQueueVK&, TransferQueue, this->mQueues [ TRANSFER_QUEUE ]);
		
	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mDevice != VK_NULL_HANDLE );
	}

	//----------------------------------------------------------------//
	operator VkDevice () {
		return this->mDevice;
	}

	//----------------------------------------------------------------//
	VkResult				AcquireNextImageKHR				( VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, ZLIndex& index );
	VkDeviceMemory 			AllocateMemory					( const VkMemoryAllocateInfo& allocateInfo, const VkAllocationCallbacks* pAllocator = NULL );
	void					BindImageMemory					( VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset = 0 );
	VkImage					CreateImage						( const VkImageCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator = NULL );
	VkImageView				CreateImageView					( const VkImageViewCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator = NULL );
	VkFramebuffer			CreateFramebuffer				( const VkFramebufferCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator = NULL );
	VkRenderPass			CreateRenderPass				( const VkRenderPassCreateInfo& createInfo, const VkAllocationCallbacks* pAllocator = NULL );
	VkSwapchainKHR			CreateSwapchainKHR				( const VkSwapchainCreateInfoKHR& createInfo, const VkAllocationCallbacks* pAllocator = NULL );
	void					DestroySwapchainKHR				( VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator = NULL );
	VkMemoryRequirements	GetImageMemoryRequirements		( VkImage image );
	ZLSize					GetSwapchainImagesKHR			( VkSwapchainKHR swapchain, ZLSize imageCount = 0, VkImage* images = NULL );
	void					Initialize						( MOAIPhysicalDeviceVK& physicalDevice, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT, bool requestPresent = true );
							MOAILogicalDeviceVK				();
							~MOAILogicalDeviceVK			();
};

#endif
