// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
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
void MOAIDescriptorSetSnapshotVK::Update ( const MOAIDescriptorSetVK& descriptorSet ) {

	assert ( *this != false );

	ZLSize totalWrites = descriptorSet.Size ();
	VkWriteDescriptorSet* writeDescriptors = ( VkWriteDescriptorSet* )alloca ( totalWrites * sizeof ( VkWriteDescriptorSet ));
	for ( ZLIndex i = 0; i < totalWrites; ++i ) {
		writeDescriptors [ i ] = descriptorSet [ i ];
		writeDescriptors [ i ].dstSet = this->mDescriptorSet;
	}

	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAIDescriptorSetLayoutVK >().GetDependency < MOAILogicalDeviceVK >();
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )totalWrites, writeDescriptors, 0, NULL );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {

	this->GetDependency < MOAIDescriptorSetLayoutVK >().RetireDescriptorSet ( *this );
}
