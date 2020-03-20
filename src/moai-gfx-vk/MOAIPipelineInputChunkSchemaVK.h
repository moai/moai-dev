// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPIPELINEINPUTCHUNKSCHEMAVK_H
#define MOAIPIPELINEINPUTCHUNKSCHEMAVK_H

class MOAILogicalDeviceVK;
class MOAIPipelineInputChunkVK;
class MOAIDescriptorSetSnapshotVK;

//================================================================//
// MOAIPipelineInputChunkSchemaVK
//================================================================//
class MOAIPipelineInputChunkSchemaVK :
	public ZLRefCountedObject,
	public ZLFinalizable,
	public ZLFinalizable_DependsOn < MOAILogicalDeviceVK > {
private:

	friend class MOAIPipelineInputChunkVK;
	friend class MOAIDescriptorSetSnapshotVK;
	friend class MOAIPipelineInputChunkVK;

	static const ZLSize MAX_DESCRIPTOR_SETS = 16;

	ZLLeanArray < VkDescriptorSetLayoutBinding >	mLayoutBindings;
	ZLLeanArray < ZLIndex >							mSignatureOffsets;
	ZLSize											mSignatureSize;

	VkDescriptorPool								mPool; // TODO: need to create more pools on the fly
	VkDescriptorSetLayout							mLayout;

	STLSet < MOAIDescriptorSetSnapshotVK* >			mSnapshots;
	STLSet < MOAIDescriptorSetSnapshotVK* >			mUnpinnedSpanshots;

	//----------------------------------------------------------------//
	void							_Finalize 					();

public:

	IMPLEMENT_DEPENDS_ON ( MOAIPipelineInputChunkSchemaVK )

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
									MOAIPipelineInputChunkSchemaVK			();
									~MOAIPipelineInputChunkSchemaVK			();
	MOAIDescriptorSetSnapshotVK*	ProcureDescriptorSet				( const MOAIPipelineInputChunkVK& descriptorSet );
	void							RetireDescriptorSet					( MOAIDescriptorSetSnapshotVK& snapshot );
	void							SetBinding							( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount = 1 );
};

#endif
