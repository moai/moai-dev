// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINELAYOUTVK_H
#define MOAIPIPELINELAYOUTVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

//================================================================//
// MOAIPipelineLayoutVK
//================================================================//
class MOAIPipelineLayoutVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
protected:

	friend class MOAIDescriptorSetArrayVK;

	VkPipelineLayout											mPipelineLayout;
	ZLLeanArray < ZLStrongPtr < MOAIDescriptorSetLayoutVK > >	mDescriptorSetLayouts;

	//----------------------------------------------------------------//
	void 							_Finalize							();
	
public:

	IMPLEMENT_DEPENDS_ON ( MOAIPipelineLayoutVK )

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
	void							AffirmPipelineLayout		();
	MOAIDescriptorSetLayoutVK&		GetDescriptorSetLayout		( ZLIndex index );
	void							Initialize					( MOAILogicalDeviceVK& logicalDevice, ZLSize size );
									MOAIPipelineLayoutVK		();
									~MOAIPipelineLayoutVK		();
	void							SetDescriptorSetLayout		( ZLIndex index, MOAIDescriptorSetLayoutVK& layout );
};

#endif
