// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIQUEUEVK_H
#define MOAIQUEUEVK_H

#include <moai-gfx-vk/MOAIFenceVK.h>

class MOAICommandBufferVK;
class MOAILogicalDeviceVK;

//================================================================//
// MOAIQueueVK
//================================================================//
class MOAIQueueVK :
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
private:

	friend class MOAICommandBufferVK;
	friend class MOAILogicalDeviceVK;
	
	//----------------------------------------------------------------//
	VkResult			Submit						( const VkSubmitInfo& submitInfo );
	
public:

	IMPLEMENT_DEPENDS_ON ( MOAIQueueVK )

	u32				mIndex;
	VkCommandPool	mPool;
	VkQueue			mQueue;
	MOAIFenceVK		mFence;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return (( this->mPool != VK_NULL_HANDLE ) && ( this->mQueue != VK_NULL_HANDLE ));
	}
	
	//----------------------------------------------------------------//
	void				CreateCommandBuffer			( MOAICommandBufferVK& commandBuffer, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, bool begin = false );
	void				FlushAndFreeCommandBuffer	( MOAICommandBufferVK& commandBuffer, u64 timeout = VK_DEFAULT_FENCE_TIMEOUT );
						MOAIQueueVK 				();
						~MOAIQueueVK				();
	VkResult			PresentKHR					( const VkPresentInfoKHR& presentInfo );
	VkResult			WaitIdle					();
};

#endif
