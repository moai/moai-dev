// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSET_H
#define MOAIDESCRIPTORSET_H

#include <moai-gfx-vk/MOAIAbstractDescriptorElementVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotFactoryVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIDescriptorVK
//================================================================//
class MOAIDescriptorVK :
	virtual public ZLRefCountedObject {
protected:

	friend class MOAIDescriptorSetVK;

	ZLLeanArray < ZLStrongPtr < MOAIAbstractDescriptorElementVK > > mElements;
	ZLLeanArray < u8 > mInfoArray;
	
	//----------------------------------------------------------------//
	bool		UpdateAndPin		( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer );
};

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutVK >,
	public MOAIAbstractSnapshotFactoryVK < MOAIDescriptorSetSnapshotVK > {
protected:

	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetSnapshotVK;

	ZLLeanArray < MOAIDescriptorVK >		mDescriptors;
	ZLLeanArray < VkWriteDescriptorSet >	mWriteDescriptors;

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
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementVK* descriptor );
//	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView );
//	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo );
//	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo );
};

#endif
