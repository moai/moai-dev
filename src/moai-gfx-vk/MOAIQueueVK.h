// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIQUEUEVK_H
#define MOAIQUEUEVK_H

class MOAILogicalDeviceVK;

//================================================================//
// MOAIQueueVK
//================================================================//
class MOAIQueueVK {
private:

	friend class MOAILogicalDeviceVK;
	
public:

	u32				mIndex;
	VkCommandPool	mPool;
	VkQueue			mQueue;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return (( this->mPool != VK_NULL_HANDLE ) && ( this->mQueue != VK_NULL_HANDLE ));
	}
	
	//----------------------------------------------------------------//
	VkCommandBuffer		CreateCommandBuffer			( MOAILogicalDeviceVK& logicalDevice, bool begin = true, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY );
	void				FlushAndFreeCommandBuffer	( MOAILogicalDeviceVK& logicalDevice, VkCommandBuffer commandBuffer, u64 timeout = VK_DEFAULT_FENCE_TIMEOUT );
						MOAIQueueVK 				();
						~MOAIQueueVK				();
	VkResult			PresentKHR					( MOAILogicalDeviceVK& logicalDevice, const VkPresentInfoKHR& presentInfo );
	VkResult			Submit						( const VkSubmitInfo* submits, u32 submitCount, VkFence fence );
	VkResult			WaitIdle					();
};

#endif
