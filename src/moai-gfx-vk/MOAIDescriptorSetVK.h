// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETVK_H
#define MOAIDESCRIPTORSETVK_H

#include <moai-gfx-vk/MOAISnapshotVK.h>

class MOAIAbstractDescriptorElementVK;
class MOAIDescriptorSetLayoutImplVK;
class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorPoolVK;
class MOAIDescriptorSetStateVK;

typedef ZLArrayKey < MOAIAbstractDescriptorElementVK* > MOAIDescriptorSetKeyVK;

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public MOAISnapshotVK < MOAIDescriptorSetVK >,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutImplVK > {
private:

	friend class MOAIAbstractDescriptorElementVK;
	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetStateVK;

	MOAIDescriptorSetKeyVK								mKey;
	ZLLeanArray < MOAIAbstractDescriptorElementVK* >	mSignature;
	VkDescriptorSet										mDescriptorSet;
	bool												mIsValid;
	MOAIDescriptorPoolVK*								mPool;

	//----------------------------------------------------------------//
	void					Invalidate							();

	//----------------------------------------------------------------//
	void					MOAIAbstractSnapshotVK_OnPin		( MOAICommandBufferVK& commandBuffer );
	void					MOAIAbstractSnapshotVK_OnUnpin		();
	MOAIDescriptorSetVK*	MOAISnapshotFactoryVK_GetSnapshot 	();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetVK )

	IS ( Valid, mIsValid, true )

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
	operator MOAIDescriptorSetKeyVK& () {
		return this->mKey;
	}
	
	//----------------------------------------------------------------//
					MOAIDescriptorSetVK					();
					~MOAIDescriptorSetVK				();
};

#endif
