// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIQUEUEVK_H
#define MOAIQUEUEVK_H

#include <vector>
#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>
#include <moai-gfx-vk/MOAISurfaceVK.h>

//================================================================//
// MOAIQueueVK
//================================================================//
class MOAIQueueVK {
private:

	friend class MOAILogicalDeviceVK;

	PFN_vkQueuePresentKHR	mQueuePresentKHR;

public:

	u32				mIndex;
	VkCommandPool	mPool;
	VkQueue			mQueue;

	//----------------------------------------------------------------//
	operator bool () const {
	
		return (( this->mPool != VK_NULL_HANDLE ) && ( this->mQueue != VK_NULL_HANDLE ));
	}
	
	//----------------------------------------------------------------//
					MOAIQueueVK 			();
					~MOAIQueueVK			();
	VkResult		PresentKHR				( const VkPresentInfoKHR& presentInfo );
	VkResult		Submit					( const VkSubmitInfo* submits, u32 submitCount, VkFence fence );
	VkResult		WaitIdle				();
};

#endif
