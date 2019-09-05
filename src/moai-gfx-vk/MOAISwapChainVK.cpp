// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAISwapChainVK.h>

//================================================================//
// MOAISwapChainVK
//================================================================//

//----------------------------------------------------------------//
VkResult MOAISwapChainVK::AcquireNextImage ( MOAILogicalDeviceVK& logicalDevice, VkSemaphore presentCompleteSemaphore, ZLIndex& index ) {

	// By setting timeout to UINT64_MAX we will always wait until the next image has been acquired or an actual error is thrown
	// With that we don't have to handle VK_NOT_READY
	return logicalDevice.AcquireNextImageKHR ( this->mSwapChain, UINT64_MAX, presentCompleteSemaphore, ( VkFence )NULL, index );
}

//----------------------------------------------------------------//
void MOAISwapChainVK::Cleanup ( MOAILogicalDeviceVK& logicalDevice ) {

	if ( this->mSwapChain != VK_NULL_HANDLE ) {
		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mImages.Size (); ++i ) {
			vkDestroyImageView ( logicalDevice, this->mBuffers [ i ].view, NULL );
		}
		logicalDevice.DestroySwapchainKHR ( this->mSwapChain, NULL );
		this->mSwapChain = VK_NULL_HANDLE;
	}
}

//----------------------------------------------------------------//
void MOAISwapChainVK::Init ( MOAIGfxInstanceVK& instance, MOAIPhysicalDeviceVK& physicalDevice, MOAILogicalDeviceVK& logicalDevice, MOAISurfaceVK& surface, u32 width, u32 height ) {

	VkSwapchainKHR oldSwapchain = this->mSwapChain;

	this->mExtent				= physicalDevice.GetExtent ( width, height );
	this->mPresentMode			= physicalDevice.FindPresentMode ();
	this->mSurfaceFormat		= physicalDevice.FindSurfaceFormat ( VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR );

	u32 swapchainImageCount							= physicalDevice.SuggestSwapChainImageCount ( this->mPresentMode );
	VkSurfaceTransformFlagBitsKHR preTransform		= physicalDevice.FindSurfaceTransform ( VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR );
	VkCompositeAlphaFlagBitsKHR compositeAlpha		= physicalDevice.FindCompositeAlpha ();
	
	VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	
	u32 queueFamilyIndexCount = 0;
	uint32_t queueFamilyIndices [ 2 ];
	
	MOAIQueueVK graphicsQueue	= logicalDevice.GetGraphicsQueue ();
	MOAIQueueVK presentQueue	= logicalDevice.GetPresentQueue ();
	
	if ( graphicsQueue && presentQueue ) {
	
		if ( graphicsQueue.mIndex != presentQueue.mIndex ) {
			sharingMode = VK_SHARING_MODE_CONCURRENT;
			queueFamilyIndexCount = 2;
			queueFamilyIndices [ 0 ] = graphicsQueue.mIndex;
			queueFamilyIndices [ 1 ] = presentQueue.mIndex;
		}
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo = MOAIGfxStructVK::swapchainCreateInfoKHR (
		surface,
		swapchainImageCount,
		this->mSurfaceFormat.format,
		this->mSurfaceFormat.colorSpace,
		this->mExtent,
		1,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		queueFamilyIndices,
		queueFamilyIndexCount,
		preTransform,
		compositeAlpha,
		this->mPresentMode,
		VK_TRUE,
		oldSwapchain
	);

//	// Enable transfer source on swap chain images if supported
//	if ( physicalDevice.mSurfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) {
//		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//	}
//
//	// Enable transfer destination on swap chain images if supported
//	if ( physicalDevice.mSurfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ) {
//		swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
//	}

	this->mSwapChain = logicalDevice.CreateSwapchainKHR ( swapchainCreateInfo );

	// If an existing swap chain is re-created, destroy the old swap chain
	// This also cleans up all the presentable images
	if ( oldSwapchain != VK_NULL_HANDLE )  {
		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mImages.Size (); ++i ) {
			vkDestroyImageView ( logicalDevice, this->mBuffers [ i ].view, NULL );
		}
		logicalDevice.DestroySwapchainKHR ( oldSwapchain, NULL );
	}
	
	u32 imageCount;
	logicalDevice.GetSwapchainImagesKHR ( this->mSwapChain, imageCount );

	// Get the swap chain images
	this->mImages.Init ( imageCount );
	logicalDevice.GetSwapchainImagesKHR ( this->mSwapChain, imageCount, this->mImages.GetBuffer ());

	// Get the swap chain buffers containing the image and imageview
	this->mBuffers.Init ( imageCount );
	for ( ZLIndex i = ZLIndexOp::ZERO; i < imageCount; ++i ) {
	
		this->mBuffers [ i ].image = this->mImages [ i ];
	
		VkImageViewCreateInfo colorAttachmentView = MOAIGfxStructVK::imageViewCreateInfo (
			this->mBuffers [ i ].image,
			VK_IMAGE_VIEW_TYPE_2D,
			this->mSurfaceFormat.format,
			MOAIGfxStructVK::componentMapping (
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			),
			MOAIGfxStructVK::imageSubresourceRange ( VK_IMAGE_ASPECT_COLOR_BIT )
		);
		
		VK_CHECK_RESULT ( vkCreateImageView ( logicalDevice, &colorAttachmentView, NULL, &this->mBuffers [ i ].view ));
	}
}

//----------------------------------------------------------------//
MOAISwapChainVK::MOAISwapChainVK () :
	mSwapChain ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
// TODO: move to MOAILogicalDeviceVK
VkResult MOAISwapChainVK::QueuePresent ( MOAILogicalDeviceVK& logicalDevice, MOAIQueueVK& queue, uint32_t imageIndex, VkSemaphore waitSemaphore ) {

	VkPresentInfoKHR presentInfo = MOAIGfxStructVK::presentInfoKHR (
		NULL,
        0,
		&this->mSwapChain,
		1,
		&imageIndex,
		NULL
    );
	
	if ( waitSemaphore != VK_NULL_HANDLE ) {
		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.waitSemaphoreCount = 1;
	}
	return queue.PresentKHR ( logicalDevice, presentInfo );
}
