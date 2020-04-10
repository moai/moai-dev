// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSTATE_H
#define MOAIDESCRIPTORSETSTATE_H

#include <moai-gfx-vk/MOAIAbstractDescriptorElementStateVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

class MOAIDescriptorSetVK;

//================================================================//
// MOAIDescriptorStateVK
//================================================================//
class MOAIDescriptorStateVK {
protected:

	friend class MOAIDescriptorSetStateVK;

	ZLLeanArray < ZLWeakPtr < MOAIAbstractDescriptorElementStateVK > > mElements;
	ZLLeanArray < u8 > mInfoArray;
};

//================================================================//
// MOAIDescriptorSetStateVK
//================================================================//
class MOAIDescriptorSetStateVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutVK > {
protected:

	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetVK;

	ZLLeanArray < MOAIDescriptorStateVK >				mDescriptors;
	ZLLeanArray < VkWriteDescriptorSet >				mWriteDescriptors;
	ZLLeanArray < MOAIMutableWriteDescriptorSetVK >		mMutableWriteDescriptors;
	ZLLeanArray < MOAIAbstractDescriptorElementVK* >	mSignature;

	ZLStrongPtr < MOAIDescriptorSetVK >					mDescriptorSet;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*	GetWriteDescriptorSet			( ZLIndex binding, ZLIndex arrayElement );
	
public:
	
	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetStateVK )
	
	//----------------------------------------------------------------//
	MOAIDescriptorSetVK*	GetDescriptorSet				();
	void					Initialize						( MOAIDescriptorSetLayoutVK& descriptorSetLayout );
							MOAIDescriptorSetStateVK		();
							~MOAIDescriptorSetStateVK		();
	void					SetDescriptor					( ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementStateVK* descriptor );
};

#endif
