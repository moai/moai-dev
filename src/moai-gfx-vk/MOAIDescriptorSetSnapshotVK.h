// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSNAPSHOTVK_H
#define MOAIDESCRIPTORSETSNAPSHOTVK_H

#include <moai-gfx-vk/MOAIPipelineInputChunkSchemaVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

class MOAIPipelineInputChunkVK;

//================================================================//
// MOAIDescriptorSetSnapshotVK
//================================================================//
class MOAIDescriptorSetSnapshotVK :
	public MOAIAbstractSnapshotVK,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIPipelineInputChunkSchemaVK > {
private:

	friend class MOAIPipelineInputChunkSchemaVK;

	VkDescriptorSet		mDescriptorSet;

	//----------------------------------------------------------------//
	void		Update								( const MOAIPipelineInputChunkVK& signature );

	//----------------------------------------------------------------//
	void		MOAIAbstractSnapshotVK_OnUnpin		();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetSnapshotVK )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mDescriptorSet != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSet* () {
		return &this->mDescriptorSet;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSet& () {
		return this->mDescriptorSet;
	}
	
	//----------------------------------------------------------------//
				MOAIDescriptorSetSnapshotVK			();
				~MOAIDescriptorSetSnapshotVK		();
};

#endif
