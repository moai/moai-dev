// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISWAPCHAINVK_H
#define MOAISWAPCHAINVK_H

#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <vector>

#include <moai-gfx-vk/MOAIFenceVK.h>
#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAISurfaceVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>

class MOAIImageBufferSnapshotVK;

//================================================================//
// MOAISwapChainVK
//================================================================//
class MOAISwapChainVK :
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
private:

	VkSwapchainKHR					mSwapChain;
//	ZLLeanArray < VkImage >			mImages;
//	ZLLeanArray < VkImageView >		mViews;
	ZLLeanArray < ZLStrongPtr < MOAIImageBufferSnapshotVK > > mImageViews;

	MOAIFenceVK						mAcquireImageFence;

	VkExtent2D						mExtent;
	VkPresentModeKHR				mPresentMode;
	VkSurfaceFormatKHR				mSurfaceFormat;

	ZLSize							mImageCount;
	ZLIndex							mImageIndex; // updated in BeginFrame ()

	//----------------------------------------------------------------//
	void			InitializeDepthStencil		();
	void			InitializeFramebuffers		();
	void			InitializeImages			();
	
	//----------------------------------------------------------------//
	void 			_Finalize					();

public:

	IMPLEMENT_DEPENDS_ON ( MOAISwapChainVK )

	GET_CONST ( VkFormat, Format, this->mSurfaceFormat.format )
	GET_CONST ( VkColorSpaceKHR, ColorSpace, this->mSurfaceFormat.colorSpace )
	GET_CONST ( u32, Width, this->mExtent.width )
	GET_CONST ( u32, Height, this->mExtent.height )
	GET_CONST ( ZLIndex, ImageIndex, this->mImageIndex )

	//----------------------------------------------------------------//
	VkResult						AcquireNextImage			();
	VkResult						AcquireNextImage			( VkSemaphore presentCompleteSemaphore );
	MOAIImageBufferSnapshotVK&		GetImageView				( ZLIndex i );
	VkRect2D						GetRect						() const;
	void							Initialize					( MOAILogicalDeviceVK& logicalDevice, MOAISurfaceVK& surface, u32 width, u32 height );
									MOAISwapChainVK				();
									~MOAISwapChainVK			();
	VkResult						QueuePresent				( MOAIQueueVK& queue, VkSemaphore waitSemaphore = VK_NULL_HANDLE );
	ZLSize							Size						() const;
};

#endif
