// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAISwapChainVK.h>

//================================================================//
// MOAISwapChainVK
//================================================================//

//----------------------------------------------------------------//
VkResult MOAISwapChainVK::AcquireNextImage () {

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	if ( !this->mAcquireImageFence ) {
		this->mAcquireImageFence.Initialize ( logicalDevice );
	}

	VkResult result = logicalDevice.AcquireNextImageKHR ( this->mSwapChain, UINT64_MAX, VK_NULL_HANDLE, this->mAcquireImageFence, this->mImageIndex );
	this->mAcquireImageFence.Wait ();
	return result;
}

//----------------------------------------------------------------//
VkResult MOAISwapChainVK::AcquireNextImage ( VkSemaphore presentCompleteSemaphore ) {

	// By setting timeout to UINT64_MAX we will always wait until the next image has been acquired or an actual error is thrown
	// With that we don't have to handle VK_NOT_READY

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	return logicalDevice.AcquireNextImageKHR ( this->mSwapChain, UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, this->mImageIndex );
}

//----------------------------------------------------------------//
VkImageView MOAISwapChainVK::GetImageView ( ZLIndex i ) {

	assert ( i < this->mImageCount );
	return this->mViews [ i ];
}

//----------------------------------------------------------------//
VkRect2D MOAISwapChainVK::GetRect () const {

	return MOAIGfxStructVK::rect2D ( this->mExtent.width, this->mExtent.height );
}

//----------------------------------------------------------------//
void MOAISwapChainVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, MOAISurfaceVK& surface, u32 width, u32 height ) {

	MOAIPhysicalDeviceVK& physicalDevice = logicalDevice.GetDependency < MOAIPhysicalDeviceVK >();
	MOAIGfxInstanceVK& instance = physicalDevice.GetDependency < MOAIGfxInstanceVK >();

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );

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
	
	MOAIQueueVK& graphicsQueue	= logicalDevice.GetGraphicsQueue ();
	MOAIQueueVK& presentQueue	= logicalDevice.GetPresentQueue ();
	
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
		for ( ZLIndex i = 0; i < this->mImages.Size (); ++i ) {
			vkDestroyImageView ( logicalDevice, this->mViews [ i ], NULL );
		}
		logicalDevice.DestroySwapchainKHR ( oldSwapchain, NULL );
	}
	
	u32 imageCount;
	logicalDevice.GetSwapchainImagesKHR ( this->mSwapChain, imageCount );
	this->mImageCount = imageCount;
	
	// Get the swap chain images
	this->mImages.Init ( this->mImageCount );
	logicalDevice.GetSwapchainImagesKHR ( this->mSwapChain, this->mImageCount, this->mImages.GetBuffer ());

	// Create a view for each image
	this->mViews.Init ( this->mImageCount );
	for ( ZLIndex i = 0; i < this->mImageCount; ++i ) {
		
		VkImageViewCreateInfo colorAttachmentView = MOAIGfxStructVK::imageViewCreateInfo (
			this->mImages [ i ],
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
		
		VK_CHECK_RESULT ( vkCreateImageView ( logicalDevice, &colorAttachmentView, NULL, &this->mViews [ i ]));
	}
}

//----------------------------------------------------------------//
MOAISwapChainVK::MOAISwapChainVK () :
	mSwapChain ( VK_NULL_HANDLE ),
	mImageCount ( 0 ),
	mImageIndex ( 0 ) {
}

//----------------------------------------------------------------//
MOAISwapChainVK::~MOAISwapChainVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
// TODO: move to MOAILogicalDeviceVK
VkResult MOAISwapChainVK::QueuePresent ( MOAIQueueVK& queue, VkSemaphore waitSemaphore ) {

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();

	u32 imageIndex = ( u32 )this->mImageIndex;

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
	return queue.PresentKHR ( presentInfo );
}

//----------------------------------------------------------------//
ZLSize MOAISwapChainVK::Size () const {

	return this->mImageCount;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISwapChainVK::_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();

		if ( this->mSwapChain != VK_NULL_HANDLE ) {
			for ( ZLIndex i = 0; i < this->mImages.Size (); ++i ) {
				vkDestroyImageView ( logicalDevice, this->mViews [ i ], NULL );
			}
			logicalDevice.DestroySwapchainKHR ( this->mSwapChain, NULL );
			this->mSwapChain = VK_NULL_HANDLE;
		}
	}
}
