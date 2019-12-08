// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMMANDBUFFERVK_H
#define MOAICOMMANDBUFFERVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAIQueueVK.h>

class MOAIDescriptorSetSnapshotVK;
class MOAIPipelineLayoutVK;
class MOAIPipelineSnapshotVK;

//================================================================//
// MOAICommandBufferVK
//================================================================//
class MOAICommandBufferVK :
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAIQueueVK > {
private:

	typedef STLMap < const MOAIAbstractSnapshotVK*, ZLWeakPtr < MOAIAbstractSnapshotVK > >		SnapshotMap;
	typedef SnapshotMap::iterator																SnapshotMapIt;

	friend class MOAIAbstractSnapshotVK;

	VkCommandBuffer		mCommandBuffer;
	SnapshotMap			mSnapshots;
	bool				mIsValid;

	//----------------------------------------------------------------//
	void				Invalidate					();

public:

	IMPLEMENT_FINALIZABLE ( MOAICommandBufferVK )

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
	void					Begin						();
	void					BindDescriptorSet 			( VkPipelineBindPoint pipelineBindPoint, MOAIDescriptorSetSnapshotVK& descriptorSet, MOAIPipelineLayoutVK& pipelineLayout, u32 firstSet );
	void					BindPipeline				( VkPipelineBindPoint pipelineBindPoint, MOAIPipelineSnapshotVK& pipeline );
	void					End							();
	MOAILogicalDeviceVK&	GetLogicalDevice			();
							MOAICommandBufferVK			();
							~MOAICommandBufferVK		();
	void					Pin							( MOAIAbstractSnapshotVK& snapshot );
	void					Submit						();
	void					Submit						(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT );
	void					UnpinAll					();
};

#endif
