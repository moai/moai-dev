// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETVK_H
#define MOAIDESCRIPTORSETVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIAbstractPinnableVK.h>

class MOAIDescriptorSetSignatureVK;

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public MOAIAbstractPinnableVK,
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAIDescriptorSetLayoutVK > {
private:

	friend class MOAIDescriptorSetLayoutVK;

	VkDescriptorSet		mDescriptorSet;

	//----------------------------------------------------------------//
	void		Update								( const MOAIDescriptorSetSignatureVK& signature );

	//----------------------------------------------------------------//
	void		MOAIAbstractSnapshotVK_OnUnpin		();

public:

	IMPLEMENT_FINALIZABLE ( MOAIDescriptorSetVK )

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
