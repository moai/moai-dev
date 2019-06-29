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

#include <moai-gfx-vk/MOAIGfxUtilVK.h>

typedef struct _SwapChainBuffers {
	VkImage image;
	VkImageView view;
} SwapChainBuffer;

//================================================================//
// MOAISwapChainVK
//================================================================//
class MOAISwapChainVK {
public:

	VkSurfaceKHR						mSurface;

	VkFormat							mColorFormat;
	VkColorSpaceKHR						mColorSpace;
	VkSwapchainKHR						mSwapChain = VK_NULL_HANDLE; // Handle to the current swap chain, required for recreation
	uint32_t							mImageCount;
	std::vector < VkImage >				mImages;
	std::vector < SwapChainBuffer >		mBuffers;
	u32									mQueueNodeIndex = UINT32_MAX; // Queue family index of the detected graphics and presenting device queue

	u32									mWidth;
	u32									mHeight;

	//----------------------------------------------------------------//
	// Acquires the next image in the swap chain
	// @param presentCompleteSemaphore (Optional) Semaphore that is signaled when the image is ready for use
	// @param imageIndex Pointer to the image index that will be increased if the next image could be acquired
	// @note The function will always wait until the next image has been acquired by setting timeout to UINT64_MAX
	// @return VkResult of the image acquisition
	VkResult acquireNextImage ( VkDevice device, VkSemaphore presentCompleteSemaphore, uint32_t *imageIndex ) {
	
		PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR = VK_GET_DEVICE_PROC_ADDR ( device, AcquireNextImageKHR );
	
		// By setting timeout to UINT64_MAX we will always wait until the next image has been acquired or an actual error is thrown
		// With that we don't have to handle VK_NOT_READY
		return fpAcquireNextImageKHR ( device, this->mSwapChain, UINT64_MAX, presentCompleteSemaphore, ( VkFence )nullptr, imageIndex );
	}

	//----------------------------------------------------------------//
	// Destroy and free Vulkan resources used for the swapchain
	void cleanup ( VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice ) {
	
		PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR = VK_GET_DEVICE_PROC_ADDR ( device, DestroySwapchainKHR );
	
		if ( this->mSwapChain != VK_NULL_HANDLE ) {
			for ( uint32_t i = 0; i < this->mImageCount; i++ ) {
				vkDestroyImageView ( device, this->mBuffers [ i ].view, nullptr );
			}
		}
		if ( this->mSurface != VK_NULL_HANDLE ) {
			fpDestroySwapchainKHR ( device, this->mSwapChain, nullptr );
			vkDestroySurfaceKHR ( instance, this->mSurface, nullptr );
		}
		this->mSurface = VK_NULL_HANDLE;
		this->mSwapChain = VK_NULL_HANDLE;
	}

    //----------------------------------------------------------------//
    // Create the swapchain and get it's images with given width and height
    // @param width Width of the swapchain (may be adjusted to fit the requirements of the swapchain)
    // @param height Height of the swapchain (may be adjusted to fit the requirements of the swapchain)
    // @param vsync (Optional) Can be used to force vsync'd rendering (by using VK_PRESENT_MODE_FIFO_KHR as presentation mode)
	void createSwapChain ( VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, u32 width, u32 height, bool vsync ) {

		this->mWidth = width;
		this->mHeight = height;

		PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR			= VK_GET_INSTANCE_PROC_ADDR ( instance, GetPhysicalDeviceSurfaceCapabilitiesKHR );
		PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR			= VK_GET_INSTANCE_PROC_ADDR ( instance, GetPhysicalDeviceSurfacePresentModesKHR );

		PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR			= VK_GET_DEVICE_PROC_ADDR ( device, CreateSwapchainKHR );
		PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR 		= VK_GET_DEVICE_PROC_ADDR ( device, DestroySwapchainKHR );
		PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR		= VK_GET_DEVICE_PROC_ADDR ( device, GetSwapchainImagesKHR );

		VkSwapchainKHR oldSwapchain = this->mSwapChain;

		// Get physical device surface properties and formats
		VkSurfaceCapabilitiesKHR surfCaps;
		VK_CHECK_RESULT ( fpGetPhysicalDeviceSurfaceCapabilitiesKHR ( physicalDevice, this->mSurface, &surfCaps ));

		// Get available present modes
		uint32_t presentModeCount;
		VK_CHECK_RESULT ( fpGetPhysicalDeviceSurfacePresentModesKHR ( physicalDevice, this->mSurface, &presentModeCount, NULL ));
		assert ( presentModeCount > 0 );

		std::vector < VkPresentModeKHR > presentModes ( presentModeCount );
		VK_CHECK_RESULT ( fpGetPhysicalDeviceSurfacePresentModesKHR ( physicalDevice, this->mSurface, &presentModeCount, presentModes.data ()));

		VkExtent2D swapchainExtent = {};
		// If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
		if ( surfCaps.currentExtent.width == ( uint32_t )-1 ) {
			// If the surface size is undefined, the size is set to
			// the size of the images requested.
			swapchainExtent.width	= this->mWidth;
			swapchainExtent.height	= this->mHeight;
		}
		else {
			// If the surface size is defined, the swap chain size must match
			swapchainExtent		= surfCaps.currentExtent;
			this->mWidth		= surfCaps.currentExtent.width;
			this->mHeight		= surfCaps.currentExtent.height;
		}

		// Select a present mode for the swapchain

		// The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
		// This mode waits for the vertical blank ("v-sync")
		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

		// If v-sync is not requested, try to find a mailbox mode
		// It's the lowest latency non-tearing present mode available
		if ( !vsync ) {
			for ( size_t i = 0; i < presentModeCount; i++ ) {
				if ( presentModes [ i ] == VK_PRESENT_MODE_MAILBOX_KHR ) {
					swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
				if (( swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR ) && ( presentModes [ i ] == VK_PRESENT_MODE_IMMEDIATE_KHR )) {
					swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				}
			}
		}

		// Determine the number of images
		uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
		if (( surfCaps.maxImageCount > 0 ) && ( desiredNumberOfSwapchainImages > surfCaps.maxImageCount )) {
			desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
		}

		// Find the transformation of the surface
		VkSurfaceTransformFlagsKHR preTransform;
		if ( surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) {
			// We prefer a non-rotated transform
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else {
			preTransform = surfCaps.currentTransform;
		}

		// Find a supported composite alpha format (not all devices support alpha opaque)
		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		// Simply select the first composite alpha format available
		std::vector < VkCompositeAlphaFlagBitsKHR > compositeAlphaFlags;
		compositeAlphaFlags.push_back ( VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR );
		compositeAlphaFlags.push_back ( VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR );
		compositeAlphaFlags.push_back ( VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR );
		compositeAlphaFlags.push_back ( VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR );

		for ( auto& compositeAlphaFlag : compositeAlphaFlags ) {
			if ( surfCaps.supportedCompositeAlpha & compositeAlphaFlag ) {
				compositeAlpha = compositeAlphaFlag;
				break;
			};
		}

		VkSwapchainCreateInfoKHR swapchainCI = MOAIGfxStructVK::swapchainCreateInfoKHR (
			this->mSurface,
			desiredNumberOfSwapchainImages,
			this->mColorFormat,
			this->mColorSpace,
			MOAIGfxStructVK::extent2D ( swapchainExtent.width, swapchainExtent.height ),
			1,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			NULL,
			( VkSurfaceTransformFlagBitsKHR )preTransform,
			compositeAlpha,
			swapchainPresentMode,
			VK_TRUE,
			oldSwapchain
		);

		// Enable transfer source on swap chain images if supported
		if ( surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		// Enable transfer destination on swap chain images if supported
		if ( surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		VK_CHECK_RESULT(fpCreateSwapchainKHR ( device, &swapchainCI, nullptr, &this->mSwapChain ));

		// If an existing swap chain is re-created, destroy the old swap chain
		// This also cleans up all the presentable images
		if ( oldSwapchain != VK_NULL_HANDLE )  {
			for (uint32_t i = 0; i < this->mImageCount; i++) {
				vkDestroyImageView ( device, this->mBuffers[i].view, nullptr );
			}
			fpDestroySwapchainKHR ( device, oldSwapchain, nullptr );
		}
		VK_CHECK_RESULT ( fpGetSwapchainImagesKHR ( device, this->mSwapChain, &this->mImageCount, NULL ));

		// Get the swap chain images
		this->mImages.resize ( this->mImageCount );
		VK_CHECK_RESULT ( fpGetSwapchainImagesKHR(device, this->mSwapChain, &this->mImageCount, this->mImages.data ()));

		// Get the swap chain buffers containing the image and imageview
		this->mBuffers.resize ( this->mImageCount );
		for ( uint32_t i = 0; i < this->mImageCount; i++ ) {
		
			this->mBuffers [ i ].image = this->mImages [ i ];
		
			VkImageViewCreateInfo colorAttachmentView = MOAIGfxStructVK::imageViewCreateInfo (
				VK_IMAGE_VIEW_TYPE_2D,
				this->mBuffers [ i ].image,
				this->mColorFormat,
				MOAIGfxStructVK::componentMapping (
					VK_COMPONENT_SWIZZLE_R,
					VK_COMPONENT_SWIZZLE_G,
					VK_COMPONENT_SWIZZLE_B,
					VK_COMPONENT_SWIZZLE_A
				),
				MOAIGfxStructVK::imageSubresourceRange ( VK_IMAGE_ASPECT_COLOR_BIT )
			);
			
			VK_CHECK_RESULT ( vkCreateImageView ( device, &colorAttachmentView, nullptr, &this->mBuffers [ i ].view ));
		}
	}
    
	//----------------------------------------------------------------//
	void initSurface ( VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface ) {

		PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR = VK_GET_INSTANCE_PROC_ADDR ( instance, GetPhysicalDeviceSurfaceSupportKHR );
		PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR = VK_GET_INSTANCE_PROC_ADDR ( instance, GetPhysicalDeviceSurfaceFormatsKHR );

		assert ( surface );
		this->mSurface = surface;

		// Get available queue family properties
		uint32_t queueCount;
		vkGetPhysicalDeviceQueueFamilyProperties ( physicalDevice, &queueCount, NULL );
		assert ( queueCount >= 1 );

		std::vector < VkQueueFamilyProperties > queueProps ( queueCount );
		vkGetPhysicalDeviceQueueFamilyProperties ( physicalDevice, &queueCount, queueProps.data ());

		// Iterate over each queue to learn whether it supports presenting:
		// Find a queue with present support
		// Will be used to present the swap chain images to the windowing system
		std::vector < VkBool32 > supportsPresent ( queueCount );
		for ( uint32_t i = 0; i < queueCount; i++ ) {
			fpGetPhysicalDeviceSurfaceSupportKHR ( physicalDevice, i, this->mSurface, &supportsPresent [ i ]);
		}

		// Search for a graphics and a present queue in the array of queue
		// families, try to find one that supports both
		uint32_t graphicsQueueNodeIndex = UINT32_MAX;
		uint32_t presentQueueNodeIndex = UINT32_MAX;
		for ( uint32_t i = 0; i < queueCount; i++ )  {
			if (( queueProps [ i] .queueFlags & VK_QUEUE_GRAPHICS_BIT ) != 0 ) {
			
				if ( graphicsQueueNodeIndex == UINT32_MAX ) {
					graphicsQueueNodeIndex = i;
				}

				if ( supportsPresent [ i ] == VK_TRUE ) {
					graphicsQueueNodeIndex = i;
					presentQueueNodeIndex = i;
					break;
				}
			}
		}
		
		if ( presentQueueNodeIndex == UINT32_MAX ) {
			// If there's no queue that supports both present and graphics
			// try to find a separate present queue
			for ( uint32_t i = 0; i < queueCount; ++i ) {
				if ( supportsPresent [ i ] == VK_TRUE ) {
					presentQueueNodeIndex = i;
					break;
				}
			}
		}

		// Exit if either a graphics or a presenting queue hasn't been found
		assert (( graphicsQueueNodeIndex < UINT32_MAX ) || ( presentQueueNodeIndex < UINT32_MAX ));

		// todo : Add support for separate graphics and presenting queue
		assert ( graphicsQueueNodeIndex == presentQueueNodeIndex );

		this->mQueueNodeIndex = graphicsQueueNodeIndex;

		// Get list of supported surface formats
		uint32_t formatCount;
		VK_CHECK_RESULT ( fpGetPhysicalDeviceSurfaceFormatsKHR ( physicalDevice, this->mSurface, &formatCount, NULL ));
		assert ( formatCount > 0 );

		std::vector<VkSurfaceFormatKHR> surfaceFormats ( formatCount );
		VK_CHECK_RESULT ( fpGetPhysicalDeviceSurfaceFormatsKHR ( physicalDevice, this->mSurface, &formatCount, surfaceFormats.data ()));

		// If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
		// there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
		if (( formatCount == 1 ) && ( surfaceFormats [ 0 ].format == VK_FORMAT_UNDEFINED )) {
			this->mColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
			this->mColorSpace = surfaceFormats [ 0 ].colorSpace;
		}
		else {
			// iterate over the list of available surface format and
			// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
			bool found_B8G8R8A8_UNORM = false;
			for ( auto&& surfaceFormat : surfaceFormats ) {
				if ( surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM ) {
					this->mColorFormat = surfaceFormat.format;
					this->mColorSpace = surfaceFormat.colorSpace;
					found_B8G8R8A8_UNORM = true;
					break;
				}
			}

			// in case VK_FORMAT_B8G8R8A8_UNORM is not available
			// select the first available color format
			if ( !found_B8G8R8A8_UNORM ) {
				this->mColorFormat = surfaceFormats [ 0 ].format;
				this->mColorSpace = surfaceFormats [ 0 ].colorSpace;
			}
		}
	}
    
	//----------------------------------------------------------------//
	// Queue an image for presentation
    // @param queue Presentation queue for presenting the image
	// @param imageIndex Index of the swapchain image to queue for presentation
	// @param waitSemaphore (Optional) Semaphore that is waited on before the image is presented (only used if != VK_NULL_HANDLE)
    // @return VkResult of the queue presentation
	VkResult queuePresent ( VkDevice device, VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE ) {
	
		PFN_vkQueuePresentKHR fpQueuePresentKHR = VK_GET_DEVICE_PROC_ADDR ( device, QueuePresentKHR );
	
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = NULL;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &this->mSwapChain;
		presentInfo.pImageIndices = &imageIndex;
		// Check if a wait semaphore has been specified to wait for before presenting the image
		if ( waitSemaphore != VK_NULL_HANDLE ) {
			presentInfo.pWaitSemaphores = &waitSemaphore;
			presentInfo.waitSemaphoreCount = 1;
		}
		return fpQueuePresentKHR ( queue, &presentInfo );
	}
};

#endif

