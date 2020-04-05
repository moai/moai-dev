// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETVK_H
#define MOAIDESCRIPTORSETVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

class MOAIDescriptorSetStateVK;

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public MOAIAbstractSnapshotVK,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAIDescriptorSetLayoutVK > {
private:

	friend class MOAIDescriptorSetLayoutVK;

	VkDescriptorSet		mDescriptorSet;

	//----------------------------------------------------------------//
	void		MOAIAbstractSnapshotVK_OnUnpin		();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetVK )

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
				MOAIDescriptorSetVK			();
				~MOAIDescriptorSetVK		();
};

#endif
