// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSignatureVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// ZLPrimitiveWithDefault
//================================================================//
template < typename TYPE, TYPE DEFAULT_VALUE = 0 >
class ZLPrimitiveWithDefault {
private:

	TYPE mValue;

public:

	//----------------------------------------------------------------//
	operator TYPE& () {
		return this->mValue;
	}
	
	//----------------------------------------------------------------//
	operator const TYPE& () const {
		return this->mValue;
	}

	//----------------------------------------------------------------//
	ZLPrimitiveWithDefault () :
		mValue ( DEFAULT_VALUE ) {
	}

	//----------------------------------------------------------------//
	ZLPrimitiveWithDefault ( const TYPE& value ) :
		mValue ( value ) {
	}
};

//================================================================//
// MOAIDescriptorSetLayoutVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::AffirmDescritorSetLayout () {

	if ( this->mLayout ) return;
	
	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
	
	ZLSize totalBindings = this->mLayoutBindings.Size ();
	
	// set up the layout
	VkDescriptorSetLayoutCreateInfo descriptorLayout = MOAIGfxStructVK::descriptorSetLayoutCreateInfo ( this->mLayoutBindings.GetBuffer (), ( u32 )totalBindings );
	VK_CHECK_RESULT ( vkCreateDescriptorSetLayout ( logicalDevice, &descriptorLayout, NULL, &this->mLayout ));

	// do some analysis - build histogram (for pool), find offsets (for write array)
	this->mSignatureOffsets.Init ( totalBindings );
	STLMap < VkDescriptorType, ZLPrimitiveWithDefault < u32, 0 > > histogram;

	this->mSignatureSize = 0;
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalBindings; ++i ) {
		const VkDescriptorSetLayoutBinding& binding = this->mLayoutBindings [ i ];
		histogram [ binding.descriptorType ] += binding.descriptorCount;
		this->mSignatureOffsets [ i ] = ZLIndexCast ( this->mSignatureSize );
		this->mSignatureSize += binding.descriptorCount;
	}

	ZLSize nTypes = histogram.size ();
	VkDescriptorPoolSize* typeCounts = ( VkDescriptorPoolSize* )alloca ( nTypes * sizeof ( VkDescriptorPoolSize ));

	STLMap < VkDescriptorType, ZLPrimitiveWithDefault < u32, 0 > >::const_iterator histogramIt = histogram.cbegin ();
	for ( ZLIndex i = ZLIndexOp::ZERO; histogramIt != histogram.cend (); ++histogramIt, ++i ) {
		typeCounts [ i ].type = histogramIt->first;
		typeCounts [ i ].descriptorCount = histogramIt->second;
	}

	VkDescriptorPoolCreateInfo descriptorPoolInfo = MOAIGfxStructVK::descriptorPoolCreateInfo ( typeCounts, ( u32 )nTypes, MAX_DESCRIPTOR_SETS ); // TODO: max sets is hardcoded; need a pool of pools
	VK_CHECK_RESULT ( vkCreateDescriptorPool ( logicalDevice, &descriptorPoolInfo, NULL, &this->mPool ));
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings ) {

	logicalDevice.AddClient ( logicalDevice, *this );
	this->mLayoutBindings.Init ( totalBindings );
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK::MOAIDescriptorSetLayoutVK () :
	mPool ( VK_NULL_HANDLE ),
	mLayout ( VK_NULL_HANDLE ),
	mSignatureSize ( 0 ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK::~MOAIDescriptorSetLayoutVK () {

	// TODO: clean up snapshots
	this->Finalize ();
}

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIDescriptorSetLayoutVK::ProcureDescriptorSetSnapshot ( const MOAIDescriptorSetSignatureVK& signature ) {

	if (( this->mPinnedSets.size () + this->mUnpinnedSets.size ()) >= MAX_DESCRIPTOR_SETS ) return NULL;
	
	MOAIDescriptorSetSnapshotVK* snapshot = NULL;
	if ( this->mUnpinnedSets.size ()) {
		snapshot = *this->mUnpinnedSets.begin ();
		this->mUnpinnedSets.erase ( snapshot );
	}
	else {
		snapshot = new MOAIDescriptorSetSnapshotVK ();
		this->AddClient ( *this, *snapshot );
		VkDescriptorSetAllocateInfo allocInfo = MOAIGfxStructVK::descriptorSetAllocateInfo ( this->mPool, &this->mLayout );
		VK_CHECK_RESULT ( vkAllocateDescriptorSets ( this->GetLogicalDevice (), &allocInfo, &snapshot->mDescriptorSet ));
	}
	snapshot->Update ( signature );
	this->mPinnedSets.insert ( snapshot );
	return snapshot;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::RetireDescriptorSetSnapshot ( MOAIDescriptorSetSnapshotVK& snapshot ) {

	if ( snapshot == NULL ) return;
	if ( !this->mPinnedSets.contains ( &snapshot )) return;

	this->mPinnedSets.erase ( &snapshot );
	this->mUnpinnedSets.insert ( &snapshot );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::SetBinding ( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount ) {

	assert ( index < this->mLayoutBindings.Size ());
	this->mLayoutBindings [ index ] = MOAIGfxStructVK::descriptorSetLayoutBinding (
		( u32 )index,
		descriptorType,
		stageFlags,
		( u32 )descriptorCount,
		NULL
	);
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::MOAIAbstractLifecycleClientVK_Finalize () {

	this->FinalizeClients ();
	
	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
	vkDestroyDescriptorSetLayout ( logicalDevice, this->mLayout, NULL );
	vkDestroyDescriptorPool ( logicalDevice, this->mPool, NULL );
	logicalDevice.RemoveClient ( *this );
}

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIDescriptorSetLayoutVK::MOAIAbstractSnapshotCacheVK_GetSnapshot ( const MOAIDescriptorSetSignatureVK& signature ) {

	return NULL; // never match a snapshot
}
