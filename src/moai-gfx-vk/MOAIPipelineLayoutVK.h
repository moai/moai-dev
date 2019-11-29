// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINELAYOUTVK_H
#define MOAIPIPELINELAYOUTVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

//================================================================//
// MOAIPipelineLayoutVK
//================================================================//
class MOAIPipelineLayoutVK :
	public ZLRefCountedObject,
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK >,
	public MOAIAbstractSnapshotSubjectVK < MOAIPipelineLayoutVK >,
	public MOAIAbstractSnapshotVK {
protected:

	VkPipelineLayout							mPipelineLayout;
	ZLLeanArray < MOAIDescriptorSetLayoutVK > 	mDescriptorSetLayouts;

	//----------------------------------------------------------------//
	MOAIPipelineLayoutVK*		MOAIAbstractSnapshotSubjectVK_MakeSnapshot		();

public:

	IMPLEMENT_FINALIZABLE ( MOAIPipelineLayoutVK )

	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mPipelineLayout != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkPipelineLayout* () {
	
		this->AffirmPipelineLayout ();
		return &this->mPipelineLayout;
	}
	
	//----------------------------------------------------------------//
	operator VkPipelineLayout& () {
	
		this->AffirmPipelineLayout ();
		return this->mPipelineLayout;
	}
	
	//----------------------------------------------------------------//
	void							AffirmPipelineLayout			();
	void							Initialize						( MOAILogicalDeviceVK& logicalDevice, ZLSize size );
	MOAIDescriptorSetLayoutVK&		GetDescriptorSetLayout			( ZLIndex index );
	MOAIDescriptorSetLayoutVK&		InitializeDescriptorSetLayout	( ZLIndex index, ZLSize size );
									MOAIPipelineLayoutVK			();
									~MOAIPipelineLayoutVK			();
//	MOAIDescriptorSetVK*			ProcureDescriptorSet			( ZLIndex index );
//	void							SetDescriptorSetLayout			( ZLIndex index, MOAIDescriptorSetLayoutNameVK& name );
};

#endif
