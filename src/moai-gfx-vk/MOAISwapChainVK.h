/*
* Class wrapping access to the swap chain
* 
* A swap chain is a collection of framebuffers used for rendering and presentation to the windowing system
*
* Copyright (C) 2016-2017 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

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

	//----------------------------------------------------------------//
	VkResult		AcquireNextImage		( MOAILogicalDeviceVK& logicalDevice, VkSemaphore presentCompleteSemaphore, uint32_t *imageIndex );
	void			Cleanup					( MOAILogicalDeviceVK& logicalDevice );
	void			Init					( MOAIGfxInstanceVK& instance, MOAIPhysicalDeviceVK& physicalDevice, MOAILogicalDeviceVK& logicalDevice, MOAISurfaceVK& surface, u32 width, u32 height );
					MOAISwapChainVK			();
	VkResult		QueuePresent			( MOAILogicalDeviceVK& logicalDevice, VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE );
};

#endif
