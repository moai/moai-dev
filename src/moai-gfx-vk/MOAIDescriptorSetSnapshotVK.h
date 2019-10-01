// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETSNAPSHOTVK_H
#define MOAIDESCRIPTORSETSNAPSHOTVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIAbstractSnapshotVK.h>

class MOAIDescriptorSetSignatureVK;

//================================================================//
// MOAIDescriptorSetSnapshotVK
//================================================================//
class MOAIDescriptorSetSnapshotVK :
	public MOAIDescriptorSetLayoutClientVK,
	public MOAIAbstractSnapshotVK {
private:

	friend class MOAIDescriptorSetLayoutVK;

	VkDescriptorSet		mDescriptorSet;

	//----------------------------------------------------------------//
	void		Update								( const MOAIDescriptorSetSignatureVK& signature );

	//----------------------------------------------------------------//
	void		MOAIAbstractLifecycleClientVK_Finalize		();
	void		MOAIAbstractSnapshotVK_OnUnpin				();

public:

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
				MOAIDescriptorSetSnapshotVK			();
				~MOAIDescriptorSetSnapshotVK		();
};

#endif
