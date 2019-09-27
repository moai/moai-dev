// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIQUEUEVK_H
#define MOAIQUEUEVK_H

#include <moai-gfx-vk/MOAILogicalDeviceClientVK.h>

class MOAICommandBufferVK;
class MOAILogicalDeviceVK;

//================================================================//
// MOAIQueueVK
//================================================================//
class MOAIQueueVK :
	public MOAILogicalDeviceClientVK,
	public MOAILifecycleProviderVK < MOAIQueueVK > {
private:

	friend class MOAILogicalDeviceVK;
	
	//----------------------------------------------------------------//
	void			MOAIAbstractLifecycleClientVK_Finalize		();
	
public:

	u32				mIndex;
	VkCommandPool	mPool;
	VkQueue			mQueue;

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
	VkResult			Submit						( const VkSubmitInfo* submits, u32 submitCount, VkFence fence );
	VkResult			WaitIdle					();
};

//================================================================//
// MOAIQueueClientVK
//================================================================//
class MOAIQueueClientVK :
	public MOAIAbstractLifecycleClientVK < MOAIQueueVK > {
public:

	//----------------------------------------------------------------//
	MOAIQueueVK& GetQueue () {
	
		MOAIQueueVK* queue = this->GetLifecycleProvider ();
		assert ( queue );
		return *queue;
	}
};

#endif
