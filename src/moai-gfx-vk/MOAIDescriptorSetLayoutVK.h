// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDESCRIPTORSETLAYOUTVK_H
#define MOAIDESCRIPTORSETLAYOUTVK_H

class MOAILogicalDeviceVK;
class MOAIDescriptorSetVK;
class MOAIDescriptorSetVK;
class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//
class MOAIDescriptorSetLayoutVK :
	public ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
private:

	friend class MOAIDescriptorSetVK;
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

	//----------------------------------------------------------------//
	void							Visitor_Finalize 					();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIDescriptorSetLayoutVK )

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
	void							AffirmDescritorSetLayout			();
	void							Initialize							( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings );
									MOAIDescriptorSetLayoutVK			();
									~MOAIDescriptorSetLayoutVK			();
	MOAIDescriptorSetSnapshotVK*	ProcureDescriptorSet				( const MOAIDescriptorSetVK& descriptorSet );
	void							RetireDescriptorSet					( MOAIDescriptorSetSnapshotVK& snapshot );
	void							SetBinding							( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount = 1 );
};

#endif
