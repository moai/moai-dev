// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetStateVK.h>
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
void MOAIDescriptorSetLayoutVK::AffirmDescriptorSetLayout () {

	if ( this->mLayout ) return;
	
	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	
	ZLSize totalBindings = this->mLayoutBindings.Size ();
	
	// set up the layout
	VkDescriptorSetLayoutCreateInfo descriptorLayout = MOAIGfxStructVK::descriptorSetLayoutCreateInfo ( this->mLayoutBindings.GetBuffer (), ( u32 )totalBindings );
	VK_CHECK_RESULT ( vkCreateDescriptorSetLayout ( logicalDevice, &descriptorLayout, NULL, &this->mLayout ));

	// do some analysis - build histogram (for pool), find offsets (for write array)
	this->mSignatureOffsets.Init ( totalBindings );
	STLMap < VkDescriptorType, ZLPrimitiveWithDefault < u32, 0 > > histogram;

	this->mSignatureSize = 0;
	for ( ZLIndex i = 0; i < totalBindings; ++i ) {
		const VkDescriptorSetLayoutBinding& binding = this->mLayoutBindings [ i ];
		histogram [ binding.descriptorType ] += binding.descriptorCount;
		this->mSignatureOffsets [ i ] = this->mSignatureSize;
		this->mSignatureSize += binding.descriptorCount;
	}

	ZLSize nTypes = histogram.size ();
	VkDescriptorPoolSize* typeCounts = ( VkDescriptorPoolSize* )alloca ( nTypes * sizeof ( VkDescriptorPoolSize ));

	STLMap < VkDescriptorType, ZLPrimitiveWithDefault < u32, 0 > >::const_iterator histogramIt = histogram.cbegin ();
	for ( ZLIndex i = 0; histogramIt != histogram.cend (); ++histogramIt, ++i ) {
		typeCounts [ i ].type = histogramIt->first;
		typeCounts [ i ].descriptorCount = histogramIt->second;
	}

	VkDescriptorPoolCreateInfo descriptorPoolInfo = MOAIGfxStructVK::descriptorPoolCreateInfo ( typeCounts, ( u32 )nTypes, MAX_DESCRIPTOR_SETS ); // TODO: max sets is hardcoded; need a pool of pools
	VK_CHECK_RESULT ( vkCreateDescriptorPool ( logicalDevice, &descriptorPoolInfo, NULL, &this->mPool ));
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings ) {

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
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

	this->Destruct ();
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK* MOAIDescriptorSetLayoutVK::ProcureDescriptorSet ( const MOAIDescriptorSetStateVK& descriptorSetState ) {

	if ( this->mAllSnapshots.size () >= MAX_DESCRIPTOR_SETS ) return NULL;
	
	MOAIDescriptorSetKeyVK key ( descriptorSetState.mSignature );
	if ( this->mActiveSnapshots.contains ( key )) {
		return this->mActiveSnapshots [ key ];
	}
	
	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	MOAIDescriptorSetVK* snapshot = NULL;
	
	if ( this->mExpiredSnapshots.size ()) {
		snapshot = *this->mExpiredSnapshots.begin ();
		this->mExpiredSnapshots.erase ( snapshot );
	}
	else {
		snapshot = new MOAIDescriptorSetVK ();
		snapshot->Retain ();
		
		snapshot->SetDependency < MOAIDescriptorSetLayoutVK >( *this );
		VkDescriptorSetAllocateInfo allocInfo = MOAIGfxStructVK::descriptorSetAllocateInfo ( this->mPool, &this->mLayout );
		VK_CHECK_RESULT ( vkAllocateDescriptorSets ( logicalDevice, &allocInfo, &snapshot->mDescriptorSet ));
		
		this->mAllSnapshots.insert ( snapshot );
	}
	
	ZLSize totalSets = descriptorSetState.mDescriptors.Size ();
	
	VkWriteDescriptorSet* writeDescriptors = ( VkWriteDescriptorSet* )alloca ( totalSets * sizeof ( VkWriteDescriptorSet ));
	for ( ZLIndex i = 0; i < totalSets; ++i ) {
		writeDescriptors [ i ] = descriptorSetState.mWriteDescriptors [ i ];
		writeDescriptors [ i ].dstSet = *snapshot;
	}
	snapshot->mSignature.CloneFrom ( descriptorSetState.mSignature );
	snapshot->mIsValid = true;
	
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )totalSets, writeDescriptors, 0, NULL );
	
	this->mActiveSnapshots [ *snapshot ] = snapshot;
	
	return snapshot;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::RetireDescriptorSet ( MOAIDescriptorSetVK& snapshot ) {

	if ( !this->mAllSnapshots.contains ( &snapshot )) return;

	if ( !snapshot.IsValid ()) {
		if ( this->mActiveSnapshots.contains ( snapshot )) {
			this->mActiveSnapshots.erase ( snapshot );
		}
		this->mExpiredSnapshots.insert ( &snapshot );
	}
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
void MOAIDescriptorSetLayoutVK::_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyDescriptorSetLayout ( logicalDevice, this->mLayout, NULL );
		vkDestroyDescriptorPool ( logicalDevice, this->mPool, NULL );
	}
	
	// TODO: clean up snapshots
	STLSet < MOAIDescriptorSetVK* >::iterator snapshotIt = this->mAllSnapshots.begin ();
	for ( ; snapshotIt != this->mAllSnapshots.end (); ++snapshotIt ) {
		( *snapshotIt )->Release ();
	}
}
