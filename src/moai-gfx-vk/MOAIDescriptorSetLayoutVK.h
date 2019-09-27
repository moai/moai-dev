// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTVK_H
#define MOAIDESCRIPTORSETLAYOUTVK_H

#include <moai-gfx-vk/MOAIInitializerVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutNameVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>

class MOAIDescriptorSetVK;

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//
class MOAIDescriptorSetLayoutVK :
	public ZLRefCountedObject,
	public MOAILogicalDeviceClientVK,
	public MOAIInitializerVK < MOAIDescriptorSetLayoutVK > {
private:

	static const ZLSize MAX_DESCRIPTOR_SETS = 16;

	VkDescriptorPool						mPool; // TODO: need to create more pools on the fly
	VkDescriptorSetLayout					mLayout;
	MOAIDescriptorSetLayoutNameVK			mName;

	STLSet < MOAIDescriptorSetVK* >			mPinnedSets;
	STLSet < MOAIDescriptorSetVK* >			mUnpinnedSets;

	//----------------------------------------------------------------//
	void						MOAIAbstractInitializerClientVK_Finalize		();

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
	void						InvalidateDescriptorSet				( MOAIDescriptorSetVK& descriptorSet );
								MOAIDescriptorSetLayoutVK			();
								~MOAIDescriptorSetLayoutVK			();
	MOAIDescriptorSetVK*		ProcureDescriptorSet				();
};

//================================================================//
// MOAIDescriptorSetLayoutClientVK
//================================================================//
class MOAIDescriptorSetLayoutClientVK :
	public MOAIAbstractInitializerClientVK < MOAIDescriptorSetLayoutVK > {
public:

	//----------------------------------------------------------------//
	MOAIDescriptorSetLayoutVK& GetLayout () {
	
		MOAIDescriptorSetLayoutVK* layout = this->GetInitializer ();
		assert ( layout );
		return *layout;
	}
};

#endif
