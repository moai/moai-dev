// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSET_H
#define MOAIDESCRIPTORSET_H

#include <moai-gfx-vk/MOAIAbstractDescriptorElementStateVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAISnapshotVK.h>

class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIDescriptorVK
//================================================================//
class MOAIDescriptorVK {
protected:

	friend class MOAIDescriptorSetVK;

	ZLLeanArray < ZLWeakPtr < MOAIAbstractDescriptorElementStateVK > > mElements;
	ZLLeanArray < u8 > mInfoArray;
};

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public virtual ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutVK > {
protected:

	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetSnapshotVK;

	ZLLeanArray < MOAIDescriptorVK >					mDescriptors;
	ZLLeanArray < VkWriteDescriptorSet >				mWriteDescriptors;
	ZLLeanArray < MOAIMutableWriteDescriptorSetVK >		mMutableWriteDescriptors;
	ZLLeanArray < MOAIAbstractDescriptorElementVK* >	mSignature;

	ZLStrongPtr < MOAIDescriptorSetSnapshotVK >			mDescriptorSet;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*			GetWriteDescriptorSet		( ZLIndex binding, ZLIndex arrayElement );
	
public:
	
	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetVK )
	
	//----------------------------------------------------------------//
	MOAIDescriptorSetSnapshotVK*	GetDescriptorSet			();
	void							Initialize					( MOAIDescriptorSetLayoutVK& descriptorSetLayout );
									MOAIDescriptorSetVK			();
									~MOAIDescriptorSetVK		();
	void							SetDescriptor				( ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementStateVK* descriptor );
};

#endif
