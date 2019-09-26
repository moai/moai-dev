// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
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
void MOAIDescriptorSetLayoutVK::DiscardDescriptorSet ( MOAIDescriptorSetVK& descriptorSet ) {

	if ( descriptorSet == NULL ) return;
	if ( !this->mPinnedSets.contains ( &descriptorSet )) return;
	
	this->mPinnedSets.erase ( &descriptorSet );
	this->mUnpinnedSets.insert ( &descriptorSet );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutVK::Init ( MOAILogicalDeviceVK& logicalDevice, const MOAIDescriptorSetLayoutNameVK& name ) {

	this->SetLogicalDevice ( logicalDevice );

	this->mName = name;
	ZLSize nameSize = name.Size ();
	
	// set up the layout
	VkDescriptorSetLayoutCreateInfo descriptorLayout = MOAIGfxStructVK::descriptorSetLayoutCreateInfo ( this->mName.GetBuffer (), nameSize );
	VK_CHECK_RESULT ( vkCreateDescriptorSetLayout ( logicalDevice, &descriptorLayout, NULL, &this->mLayout ));
	
	// set up the pool
	STLMap < VkDescriptorType, ZLPrimitiveWithDefault < u32, 0 > > histogram;
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nameSize; ++i ) {
		const VkDescriptorSetLayoutBinding& binding = this->mName [ i ];
		histogram [ binding.descriptorType ] += binding.descriptorCount;
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
MOAIDescriptorSetLayoutVK::MOAIDescriptorSetLayoutVK () :
	mPool ( VK_NULL_HANDLE ),
	mLayout ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK::~MOAIDescriptorSetLayoutVK () {

	vkDestroyDescriptorSetLayout ( this->GetLogicalDevice (), this->mLayout, NULL );
	vkDestroyDescriptorPool ( this->GetLogicalDevice (), this->mPool, NULL );
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK* MOAIDescriptorSetLayoutVK::ProcureDescriptorSet () {

	if (( this->mPinnedSets.size () + this->mUnpinnedSets.size ()) >= MAX_DESCRIPTOR_SETS ) return NULL;
	
	MOAIDescriptorSetVK* descriptorSet = NULL;
	if ( this->mUnpinnedSets.size ()) {
		descriptorSet = *this->mUnpinnedSets.begin ();
		this->mUnpinnedSets.erase ( descriptorSet );
	}
	else {
	
		descriptorSet = new MOAIDescriptorSetVK ();
		descriptorSet->mLayout = this;

		VkDescriptorSetAllocateInfo allocInfo = MOAIGfxStructVK::descriptorSetAllocateInfo ( this->mPool, &this->mLayout );
		VK_CHECK_RESULT ( vkAllocateDescriptorSets ( this->GetLogicalDevice (), &allocInfo, &descriptorSet->mDescriptorSet ));
		descriptorSet->InitWriteArray ( this->mName );
	}
	this->mPinnedSets.insert ( descriptorSet );
	return descriptorSet;
}
