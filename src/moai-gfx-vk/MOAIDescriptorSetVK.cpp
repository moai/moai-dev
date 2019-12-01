// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSignatureVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

//================================================================//
// MOAIDescriptorSetVK
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetVK::MOAIDescriptorSetVK () :
	mDescriptorSet ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::~MOAIDescriptorSetVK () {
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::Update ( const MOAIDescriptorSetSignatureVK& descriptorSet ) {

	assert ( *this != false );

	ZLSize totalWrites = descriptorSet.Size ();
	VkWriteDescriptorSet* writeDescriptors = ( VkWriteDescriptorSet* )alloca ( totalWrites * sizeof ( VkWriteDescriptorSet ));
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalWrites; ++i ) {
		writeDescriptors [ i ] = descriptorSet [ i ];
		writeDescriptors [ i ].dstSet = this->mDescriptorSet;
	}

	MOAILogicalDeviceVK& logicalDevice = this->GetProvider < MOAIDescriptorSetLayoutVK >().GetProvider < MOAILogicalDeviceVK >();
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )totalWrites, writeDescriptors, 0, NULL );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::MOAIAbstractSnapshotVK_OnUnpin () {

	this->GetProvider < MOAIDescriptorSetLayoutVK >().RetireDescriptorSet ( *this );
}
