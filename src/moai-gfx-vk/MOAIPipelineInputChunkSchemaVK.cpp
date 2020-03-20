// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIPipelineInputChunkSchemaVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkVK.h>
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
// MOAIPipelineInputChunkSchemaVK
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineInputChunkSchemaVK::AffirmDescritorSetLayout () {

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
void MOAIPipelineInputChunkSchemaVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize totalBindings ) {

	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	this->mLayoutBindings.Init ( totalBindings );
}

//----------------------------------------------------------------//
MOAIPipelineInputChunkSchemaVK::MOAIPipelineInputChunkSchemaVK () :
	mPool ( VK_NULL_HANDLE ),
	mLayout ( VK_NULL_HANDLE ),
	mSignatureSize ( 0 ) {
}

//----------------------------------------------------------------//
MOAIPipelineInputChunkSchemaVK::~MOAIPipelineInputChunkSchemaVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIPipelineInputChunkSchemaVK::ProcureDescriptorSet ( const MOAIPipelineInputChunkVK& descriptorSet ) {

	if ( this->mSnapshots.size () >= MAX_DESCRIPTOR_SETS ) return NULL;
	
	MOAIDescriptorSetSnapshotVK* snapshot = NULL;
	if ( this->mUnpinnedSpanshots.size ()) {
		snapshot = *this->mUnpinnedSpanshots.begin ();
		this->mUnpinnedSpanshots.erase ( snapshot );
	}
	else {
		snapshot = new MOAIDescriptorSetSnapshotVK ();
		snapshot->Retain ();
		
		snapshot->SetDependency < MOAIPipelineInputChunkSchemaVK >( *this );
		VkDescriptorSetAllocateInfo allocInfo = MOAIGfxStructVK::descriptorSetAllocateInfo ( this->mPool, &this->mLayout );
		VK_CHECK_RESULT ( vkAllocateDescriptorSets ( this->GetDependency < MOAILogicalDeviceVK >(), &allocInfo, &snapshot->mDescriptorSet ));
		
		this->mSnapshots.insert ( snapshot );
	}
	snapshot->Update ( descriptorSet );
	return snapshot;
}

//----------------------------------------------------------------//
void MOAIPipelineInputChunkSchemaVK::RetireDescriptorSet ( MOAIDescriptorSetSnapshotVK& snapshot ) {

	if ( snapshot == NULL ) return;
	if ( !this->mSnapshots.contains ( &snapshot )) return;

	this->mUnpinnedSpanshots.insert ( &snapshot );
}

//----------------------------------------------------------------//
void MOAIPipelineInputChunkSchemaVK::SetBinding ( ZLIndex index, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, ZLSize descriptorCount ) {

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
void MOAIPipelineInputChunkSchemaVK::_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		vkDestroyDescriptorSetLayout ( logicalDevice, this->mLayout, NULL );
		vkDestroyDescriptorPool ( logicalDevice, this->mPool, NULL );
	}
	
	// TODO: clean up snapshots
	STLSet < MOAIDescriptorSetSnapshotVK* >::iterator snapshotIt = this->mSnapshots.begin ();
	for ( ; snapshotIt != this->mSnapshots.end (); ++snapshotIt ) {
		( *snapshotIt )->Release ();
	}
}
