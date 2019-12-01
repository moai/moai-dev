// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMMANDBUFFERVK_H
#define MOAICOMMANDBUFFERVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotSubjectVK.h>
#include <moai-gfx-vk/MOAIQueueVK.h>

class MOAIDescriptorSetVK;
class MOAIPipelineLayoutVK;
class MOAIPipelineVK;

//================================================================//
// MOAICommandBufferVK
//================================================================//
class MOAICommandBufferVK :
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAIQueueVK > {
private:

	friend class MOAIAbstractSnapshotVK;

	VkCommandBuffer								mCommandBuffer;
	ZLLeanList < MOAIAbstractSnapshotVK* >		mSnapshots;
	bool										mIsValid;

	//----------------------------------------------------------------//
	void				Invalidate					();
	void				PinSnapshot					( MOAIAbstractSnapshotVK& snapshot );

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
	void				Begin						();
	void				BindDescriptorSet 			( VkPipelineBindPoint pipelineBindPoint, MOAIDescriptorSetVK& descriptorSet, MOAIPipelineLayoutVK& pipelineLayout, u32 firstSet );
	void				BindPipeline				( VkPipelineBindPoint pipelineBindPoint, MOAIPipelineVK& pipeline );
	void				End							();
						MOAICommandBufferVK			();
						~MOAICommandBufferVK		();
	void				Submit						();
	void				Submit						(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT );
	void				UnpinSnapshots				();

	//----------------------------------------------------------------//
	template < typename SNAPSHOT_TYPE >
	SNAPSHOT_TYPE* MakeSnapshot ( MOAIAbstractSnapshotSubjectVK < SNAPSHOT_TYPE >& subject ) {
		SNAPSHOT_TYPE* snapshot = subject.MakeSnapshot ();
		if ( snapshot ) {
			this->PinSnapshot ( *snapshot );
		}
		return snapshot;
	}
};

#endif
