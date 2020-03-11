// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETVK_H
#define MOAIDESCRIPTORSETVK_H

#include <moai-gfx-vk/MOAIAbstractSnapshotFactoryVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutVK >,
	public MOAIAbstractSnapshotFactoryVK < MOAIDescriptorSetSnapshotVK >,
	public ZLLeanArray < VkWriteDescriptorSet > {
protected:

	friend class MOAIDescriptorSetLayoutVK;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*			GetWriteDescriptorSet 							( ZLIndex binding, ZLIndex arrayElement );
	
	//----------------------------------------------------------------//
	MOAIDescriptorSetSnapshotVK*	MOAIAbstractSnapshotFactoryVK_GetSnapshot		( MOAICommandBufferVK& commandBuffer );
	
public:
	
	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetVK )
	
	//----------------------------------------------------------------//
	void			Initialize						( MOAIDescriptorSetLayoutVK& descriptorSetLayout );
					MOAIDescriptorSetVK				();
					~MOAIDescriptorSetVK			();
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo );
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo );
};

#endif
