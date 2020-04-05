// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSTATE_H
#define MOAIDESCRIPTORSETSTATE_H

#include <moai-gfx-vk/MOAIAbstractDescriptorElementVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotFactoryVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

class MOAIDescriptorSetVK;

//================================================================//
// MOAIDescriptorStateVK
//================================================================//
class MOAIDescriptorStateVK :
	virtual public ZLRefCountedObject {
protected:

	friend class MOAIDescriptorSetStateVK;

	ZLLeanArray < ZLStrongPtr < MOAIAbstractDescriptorElementVK > > mElements;
	ZLLeanArray < u8 > mInfoArray;
	
	//----------------------------------------------------------------//
	bool		UpdateAndPin		( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer );
};

//================================================================//
// MOAIDescriptorSetStateVK
//================================================================//
class MOAIDescriptorSetStateVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutVK >,
	public MOAIAbstractSnapshotFactoryVK < MOAIDescriptorSetVK > {
protected:

	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetVK;

	ZLLeanArray < MOAIDescriptorStateVK >		mDescriptors;
	ZLLeanArray < VkWriteDescriptorSet >	mWriteDescriptors;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*			GetWriteDescriptorSet					( ZLIndex binding, ZLIndex arrayElement );
	
	//----------------------------------------------------------------//
	MOAIDescriptorSetVK*	MOAIAbstractSnapshotFactoryVK_GetSnapshot		( MOAICommandBufferVK& commandBuffer );
	
public:
	
	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetStateVK )
	
	//----------------------------------------------------------------//
	void			Initialize						( MOAIDescriptorSetLayoutVK& descriptorSetLayout );
					MOAIDescriptorSetStateVK		();
					~MOAIDescriptorSetStateVK		();
	void			SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementVK* descriptor );
};

#endif
