// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETARRAYVK_H
#define MOAIDESCRIPTORSETARRAYVK_H

class MOAICommandBufferVK;
class MOAIDescriptorSetVK;
class MOAIGfxMgrVK;
class MOAIPipelineLayoutVK;

//================================================================//
// MOAIDescriptorSetArrayVK
//================================================================//
class MOAIDescriptorSetArrayVK :
	public virtual ZLRefCountedObject,
	public virtual ZLFinalizable {
protected:

	ZLStrongPtr < MOAIPipelineLayoutVK >	mPipelineLayout;
	ZLLeanArray < MOAIDescriptorSetVK >		mDescriptorSets;

public:

	IMPLEMENT_DEPENDS_ON ( MOAIPipelineInputBodyComposerVK )
	
	//----------------------------------------------------------------//
	void						Bind								( MOAICommandBufferVK& commandBuffer, VkPipelineBindPoint pipelineBindPoint );
	MOAIDescriptorSetVK&		GetDescriptorSet					( ZLIndex index );
	MOAIPipelineLayoutVK&		GetPipelineLayout					();
								MOAIDescriptorSetArrayVK			();
								~MOAIDescriptorSetArrayVK			();
	void						SetPipelineLayout					( MOAIPipelineLayoutVK& pipelineLayout );
};

#endif
