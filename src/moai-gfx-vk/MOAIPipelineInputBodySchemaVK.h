// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINEINPUTBODYSCHEMAVK_H
#define MOAIPIPELINEINPUTBODYSCHEMAVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkSchemaVK.h>

//================================================================//
// MOAIPipelineInputBodySchemaVK
//================================================================//
class MOAIPipelineInputBodySchemaVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
protected:

	friend class MOAIPipelineInputBodyVK;

	VkPipelineLayout													mPipelineLayout;
	ZLLeanArray < ZLStrongPtr < MOAIPipelineInputChunkSchemaVK > >		mDescriptorSetLayouts;

	//----------------------------------------------------------------//
	void 							_Finalize				();
	
public:

	IMPLEMENT_DEPENDS_ON ( MOAIPipelineInputBodySchemaVK )

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
	void								AffirmPipelineLayout				();
	MOAIPipelineInputChunkSchemaVK&		GetDescriptorSetLayout				( ZLIndex index );
	void								Initialize							( MOAILogicalDeviceVK& logicalDevice, ZLSize size );
										MOAIPipelineInputBodySchemaVK		();
										~MOAIPipelineInputBodySchemaVK		();
	void								SetDescriptorSetLayout 				( ZLIndex index, MOAIPipelineInputChunkSchemaVK& layout );
};

#endif
