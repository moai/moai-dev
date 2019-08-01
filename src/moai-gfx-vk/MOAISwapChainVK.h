// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISWAPCHAINVK_H
#define MOAISWAPCHAINVK_H

#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <vector>

#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAISurfaceVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>

typedef struct _SwapChainBuffers {
	VkImage image;
	VkImageView view;
} SwapChainBuffer;

//================================================================//
// MOAISwapChainBuffersVK
//================================================================//
class MOAISwapChainBuffersVK {
public:
	VkImage			mImage;
	VkImageView		mView;
};

//================================================================//
// MOAISwapChainVK
//================================================================//
class MOAISwapChainVK {
public:

	VkSwapchainKHR						mSwapChain;
	ZLLeanArray < VkImage >				mImages;
	ZLLeanArray < SwapChainBuffer >		mBuffers;

	VkExtent2D							mExtent;
	VkPresentModeKHR					mPresentMode;
	VkSurfaceFormatKHR					mSurfaceFormat;

	GET ( u32, Height, this->mExtent.height );
	GET ( u32, Width, this->mExtent.width );

	//----------------------------------------------------------------//
	VkResult		AcquireNextImage		( MOAILogicalDeviceVK& logicalDevice, VkSemaphore presentCompleteSemaphore, ZLIndex& index );
	void			Cleanup					( MOAILogicalDeviceVK& logicalDevice );
	void			Init					( MOAIGfxInstanceVK& instance, MOAIPhysicalDeviceVK& physicalDevice, MOAILogicalDeviceVK& logicalDevice, MOAISurfaceVK& surface, u32 width, u32 height );
					MOAISwapChainVK			();
	VkResult		QueuePresent			( MOAIQueueVK& queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE );
};

#endif
