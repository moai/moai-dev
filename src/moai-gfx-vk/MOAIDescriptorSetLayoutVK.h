// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTVK_H
#define MOAIDESCRIPTORSETLAYOUTVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutNameVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceClientVK.h>

class MOAIDescriptorSetVK;
class MOAILogicalDeviceVK;

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//
class MOAIDescriptorSetLayoutVK :
	public ZLRefCountedObject,
	public MOAILogicalDeviceClientVK {
private:

	static const ZLSize MAX_DESCRIPTOR_SETS = 16;

	VkDescriptorPool						mPool; // TODO: need to create more pools on the fly
	VkDescriptorSetLayout					mLayout;
	MOAIDescriptorSetLayoutNameVK			mName;

	STLSet < MOAIDescriptorSetVK* >			mPinnedSets;
	STLSet < MOAIDescriptorSetVK* >			mUnpinnedSets;

public:

	GET_CONST ( MOAIDescriptorSetLayoutNameVK&, Name, mName );

	//----------------------------------------------------------------//
	operator bool () const {
	
		return ( this->mLayout != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSetLayout* () {
	
		return &this->mLayout;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSetLayout& () {
	
		return this->mLayout;
	}
	
	//----------------------------------------------------------------//
	void						DiscardDescriptorSet				( MOAIDescriptorSetVK& descriptorSet );
	void						Init								( MOAILogicalDeviceVK& logicalDevice, const MOAIDescriptorSetLayoutNameVK& name );
								MOAIDescriptorSetLayoutVK			();
								~MOAIDescriptorSetLayoutVK			();
	MOAIDescriptorSetVK*		ProcureDescriptorSet				();
};

#endif
