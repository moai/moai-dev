// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTVK_H
#define MOAIDESCRIPTORSETLAYOUTVK_H

class MOAILogicalDeviceVK;
class MOAIDescriptorSetSignatureVK;
class MOAIDescriptorSetSignatureVK;
class MOAIDescriptorSetVK;

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//
class MOAIDescriptorSetLayoutVK :
	public ZLRefCountedObject,
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK > {
private:

	friend class MOAIDescriptorSetSignatureVK;
	friend class MOAIDescriptorSetVK;
	friend class MOAIDescriptorSetSignatureVK;

	static const ZLSize MAX_DESCRIPTOR_SETS = 16;

	ZLLeanArray < VkDescriptorSetLayoutBinding >	mLayoutBindings;
	ZLLeanArray < ZLIndex >							mSignatureOffsets;
	ZLSize											mSignatureSize;

	VkDescriptorPool								mPool; // TODO: need to create more pools on the fly
	VkDescriptorSetLayout							mLayout;

	STLSet < MOAIDescriptorSetVK* >			mSnapshots;
	STLSet < MOAIDescriptorSetVK* >			mUnpinnedSpanshots;

public:

	IMPLEMENT_FINALIZABLE ( MOAIDescriptorSetLayoutVK )

	GET_CONST ( ZLSize, Size, mLayoutBindings.Size ())
	GET_CONST ( ZLSize, SignatureSize, mSignatureSize )

	//----------------------------------------------------------------//
	operator bool () const {
		return ( this->mLayout != VK_NULL_HANDLE );
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSetLayout* () {
		this->AffirmDescritorSetLayout ();
		return &this->mLayout;
	}
	
	//----------------------------------------------------------------//
	operator VkDescriptorSetLayout& () {
		this->AffirmDescritorSetLayout ();
		return this->mLayout;
	}
	
	//----------------------------------------------------------------//
	void						AffirmDescritorSetLayout			();
	void						Initialize							( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings );
								MOAIDescriptorSetLayoutVK			();
								~MOAIDescriptorSetLayoutVK			();
	MOAIDescriptorSetVK*		ProcureDescriptorSet				( const MOAIDescriptorSetSignatureVK& descriptorSet );
	void						RetireDescriptorSet					( MOAIDescriptorSetVK& snapshot );
	void						SetBinding							( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount = 1 );
};

#endif
