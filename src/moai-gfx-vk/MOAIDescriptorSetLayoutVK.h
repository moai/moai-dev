// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTVK_H
#define MOAIDESCRIPTORSETLAYOUTVK_H

class MOAILogicalDeviceVK;
class MOAIDescriptorSetVK;
class MOAIDescriptorSetSignatureVK;
class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//
class MOAIDescriptorSetLayoutVK :
	public ZLRefCountedObject,
	public ZLAbstractFinalizable,
	public ZLAbstractFinalizable_HasDependencyOn < MOAILogicalDeviceVK > {
private:

	friend class MOAIDescriptorSetSignatureVK;
	friend class MOAIDescriptorSetSnapshotVK;
	friend class MOAIDescriptorSetVK;

	static const ZLSize MAX_DESCRIPTOR_SETS = 16;

	ZLLeanArray < VkDescriptorSetLayoutBinding >	mLayoutBindings;
	ZLLeanArray < ZLIndex >							mSignatureOffsets;
	ZLSize											mSignatureSize;

	VkDescriptorPool								mPool; // TODO: need to create more pools on the fly
	VkDescriptorSetLayout							mLayout;

	STLSet < MOAIDescriptorSetSnapshotVK* >			mSnapshots;
	STLSet < MOAIDescriptorSetSnapshotVK* >			mUnpinnedSpanshots;

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
	void								AffirmDescritorSetLayout			();
	void								Initialize							( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings );
										MOAIDescriptorSetLayoutVK			();
										~MOAIDescriptorSetLayoutVK			();
	MOAIDescriptorSetSnapshotVK*		ProcureDescriptorSetSnapshot		( const MOAIDescriptorSetVK& descriptorSet );
	void								RetireDescriptorSetSnapshot			( MOAIDescriptorSetSnapshotVK& snapshot );
	void								SetBinding							( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount = 1 );
};

#endif
