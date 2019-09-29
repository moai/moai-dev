// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMMANDBUFFERVK_H
#define MOAICOMMANDBUFFERVK_H

#include <moai-gfx-vk/MOAIQueueVK.h>

class MOAIAbstractCommandBufferResourceVK;

//================================================================//
// MOAICommandBufferVK
//================================================================//
class MOAICommandBufferVK :
	public MOAIQueueClientVK {
private:

	friend class MOAIAbstractCommandBufferResourceVK;

	VkCommandBuffer											mCommandBuffer;
	ZLLeanList < MOAIAbstractCommandBufferResourceVK* >		mResources;
	bool													mIsValid;

	//----------------------------------------------------------------//
	void			Invalidate									();

	//----------------------------------------------------------------//
	void			MOAIAbstractLifecycleClientVK_Finalize		();

public:

	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mCommandBuffer != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkCommandBuffer* () {
	
		return &this->mCommandBuffer;
	}
	
	//----------------------------------------------------------------//
	operator VkCommandBuffer& () {
	
		return this->mCommandBuffer;
	}
	
	//----------------------------------------------------------------//
	void				Begin						();
	void				End							();
						MOAICommandBufferVK			();
						~MOAICommandBufferVK		();
	void				PinResource					( MOAIAbstractCommandBufferResourceVK& resource );
	void				Submit						();
	void				Submit						(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT );
	void				UnpinResources				();
};

#endif
