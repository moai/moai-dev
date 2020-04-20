// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSNAPSHOTVK_H
#define MOAIDESCRIPTORSETSNAPSHOTVK_H

#include <moai-gfx-vk/MOAISnapshotVK.h>

class MOAIAbstractDescriptorElementVK;
class MOAIDescriptorSetLayoutImplVK;
class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorPoolVK;
class MOAIDescriptorSetVK;

typedef ZLArrayKey < MOAIAbstractDescriptorElementVK* > MOAIDescriptorSetKeyVK;

//================================================================//
// MOAIDescriptorSetSnapshotVK
//================================================================//
class MOAIDescriptorSetSnapshotVK :
	public MOAISnapshotVK < MOAIDescriptorSetSnapshotVK >,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutImplVK > {
private:

	friend class MOAIAbstractDescriptorElementVK;
	friend class MOAIDescriptorSetLayoutImplVK;
	friend class MOAIDescriptorSetLayoutVK;
	friend class MOAIDescriptorSetVK;

	MOAIDescriptorSetKeyVK								mKey;
	ZLLeanArray < MOAIAbstractDescriptorElementVK* >	mSignature;
	VkDescriptorSet										mDescriptorSet;
	bool												mIsValid;
	MOAIDescriptorPoolVK*								mPool;

	//----------------------------------------------------------------//
	void					Invalidate					();

	//----------------------------------------------------------------//
	void							MOAIAbstractSnapshotVK_OnPin		( MOAICommandBufferVK& commandBuffer );
	void							MOAIAbstractSnapshotVK_OnUnpin		();
	MOAIDescriptorSetSnapshotVK*	MOAISnapshotFactoryVK_GetSnapshot 	();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetSnapshotVK )

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
					MOAIDescriptorSetSnapshotVK					();
					~MOAIDescriptorSetSnapshotVK				();
};

#endif
