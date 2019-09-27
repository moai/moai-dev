// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETVK_H
#define MOAIDESCRIPTORSETVK_H

#include <moai-gfx-vk/MOAIAbstractCommandBufferResourceVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

class MOAIDescriptorSetLayoutNameVK;
class MOAIDescriptorSetLayoutVK;

//================================================================//
// MOAIDescriptorSetVK
//================================================================//
class MOAIDescriptorSetVK :
	public MOAIAbstractCommandBufferResourceVK,
	public MOAIDescriptorSetLayoutClientVK {
private:

	friend class MOAIDescriptorSetLayoutVK;

	ZLLeanArray < VkWriteDescriptorSet >	mWriteArray;
	ZLLeanArray < VkWriteDescriptorSet* >	mWriteArrayBindings;
	VkDescriptorSet							mDescriptorSet;

	//----------------------------------------------------------------//
	VkWriteDescriptorSet*			GetWriteDescriptorSet 		( u32 binding, u32 arrayElement );

	//----------------------------------------------------------------//
	void		MOAIAbstractCommandBufferResourceVK_Unpin		();
	void		MOAIAbstractInitializerClientVK_Finalize		();

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
	void		InitWriteArray				( const MOAIDescriptorSetLayoutNameVK& name );
				MOAIDescriptorSetVK			();
				~MOAIDescriptorSetVK		();
	void		SetDescriptor				( u32 binding, u32 arrayElement, VkBufferView* texelBufferView );
	void		SetDescriptor				( u32 binding, u32 arrayElement, VkDescriptorBufferInfo* bufferInfo );
	void		SetDescriptor				( u32 binding, u32 arrayElement, VkDescriptorImageInfo* imageInfo );
	void		Update						();
};

#endif
