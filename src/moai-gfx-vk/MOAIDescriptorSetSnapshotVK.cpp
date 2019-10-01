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

	this->Finalize ();
}

//----------------------------------------------------------------//
void MOAIDescriptorSetSnapshotVK::Update ( const MOAIDescriptorSetSignatureVK& signature ) {

	ZLSize totalWrites = signature.Size ();
	VkWriteDescriptorSet* writeDescriptors = ( VkWriteDescriptorSet* )alloca ( signature.Size () * sizeof ( VkWriteDescriptorSet ));
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalWrites; ++i ) {
		writeDescriptors [ i ] = signature [ i ];
		writeDescriptors [ i ].dstSet = this->mDescriptorSet;
	}

	MOAILogicalDeviceVK& logicalDevice = this->GetLayout ().GetLogicalDevice ();
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )totalWrites, writeDescriptors, 0, NULL );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetSnapshotVK::MOAIAbstractLifecycleClientVK_Finalize () {

	this->ForceUnpin ();
	this->GetLayout ().RemoveClient ( *this );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {

	this->GetLayout ().RetireDescriptorSetSnapshot ( *this );
}
