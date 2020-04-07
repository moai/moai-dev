// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTVK_H
#define MOAIDESCRIPTORSETLAYOUTVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutImplVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>

class MOAIDescriptorSetLayoutVK;
class MOAIDescriptorSetStateVK;
class MOAILogicalDeviceVK;

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//
class MOAIDescriptorSetLayoutVK :
	public ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
private:

	friend class MOAIDescriptorSetStateVK;
	friend class MOAIDescriptorSetVK;
	friend class MOAIDescriptorSetStateVK;

	ZLLeanArray < VkDescriptorSetLayoutBinding >*	mLayoutBindings;
	ZLStrongPtr < MOAIDescriptorSetLayoutImplVK >	mImpl;

	//----------------------------------------------------------------//
	void					DeletePool						( MOAIDescriptorPoolVK* pool );

	//----------------------------------------------------------------//
	void					_Finalize						();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetLayoutVK )

	GET_CONST ( ZLSize, Size, mLayoutBindings->Size ())
	GET_CONST ( ZLSize, SignatureSize, this->mImpl->mSignatureSize )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mImpl != NULL );
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSetLayout* () {
		this->AffirmDescriptorSetLayout ();
		return &this->mImpl->mLayout;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSetLayout& () {
		this->AffirmDescriptorSetLayout ();
		return this->mImpl->mLayout;
	}
	
	//----------------------------------------------------------------//
	void						AffirmDescriptorSetLayout			();
	void						Initialize							( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings );
								MOAIDescriptorSetLayoutVK			();
								~MOAIDescriptorSetLayoutVK			();
	MOAIDescriptorSetVK*		ProcureDescriptorSet				( const MOAIDescriptorSetStateVK& descriptorSetState );
	void						SetBinding							( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount = 1 );
};

#endif
