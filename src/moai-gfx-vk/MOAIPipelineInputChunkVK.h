// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINEINPUTCHUNKVK_H
#define MOAIPIPELINEINPUTCHUNKVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotFactoryVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkSchemaVK.h>

class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIPipelineInputChunkVK
//================================================================//
class MOAIPipelineInputChunkVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIPipelineInputChunkSchemaVK >,
	public MOAIAbstractSnapshotFactoryVK < MOAIDescriptorSetSnapshotVK >,
	public ZLLeanArray < VkWriteDescriptorSet > {
protected:

	friend class MOAIPipelineInputChunkSchemaVK;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*			GetWriteDescriptorSet 							( ZLIndex binding, ZLIndex arrayElement );
	
	//----------------------------------------------------------------//
	MOAIDescriptorSetSnapshotVK*	MOAIAbstractSnapshotFactoryVK_GetSnapshot		( MOAICommandBufferVK& commandBuffer );
	
public:
	
	IMPLEMENT_DEPENDS_ON ( MOAIPipelineInputChunkVK )
	
	//----------------------------------------------------------------//
	void			Initialize						( MOAIPipelineInputChunkSchemaVK& descriptorSetLayout );
					MOAIPipelineInputChunkVK		();
					~MOAIPipelineInputChunkVK		();
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo );
};

#endif
