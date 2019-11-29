// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetSignatureVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

//================================================================//
// MOAIDescriptorSetSnapshotVK
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK::MOAIDescriptorSetSnapshotVK () :
	mDescriptorSet ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK::~MOAIDescriptorSetSnapshotVK () {
}

//----------------------------------------------------------------//
void MOAIDescriptorSetSnapshotVK::Update ( const MOAIDescriptorSetSignatureVK& signature ) {

	assert ( *this != false );

	ZLSize totalWrites = signature.Size ();
	VkWriteDescriptorSet* writeDescriptors = ( VkWriteDescriptorSet* )alloca ( totalWrites * sizeof ( VkWriteDescriptorSet ));
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalWrites; ++i ) {
		writeDescriptors [ i ] = signature [ i ];
		writeDescriptors [ i ].dstSet = this->mDescriptorSet;
	}

	MOAILogicalDeviceVK& logicalDevice = this->GetProvider < MOAIDescriptorSetLayoutVK >().GetProvider < MOAILogicalDeviceVK >();
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )totalWrites, writeDescriptors, 0, NULL );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {

	this->GetProvider < MOAIDescriptorSetLayoutVK >().RetireDescriptorSetSnapshot ( *this );
}
