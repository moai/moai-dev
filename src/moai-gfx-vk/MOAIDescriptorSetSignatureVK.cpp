// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSignatureVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorSetSignatureVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetSignatureVK::Initialize ( const MOAIDescriptorSetLayoutVK& layout ) {

	// set up the pool
	ZLSize nBindings = layout.GetSize ();
	ZLSize signatureSize = layout.GetSignatureSize ();

	// set up the write array
	this->Init ( signatureSize );

	ZLIndex writeIndex = ZLIndexOp::ZERO;
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nBindings; ++i ) {

		const VkDescriptorSetLayoutBinding& binding = layout.mLayoutBindings [ i ];

		for ( u32 dstArrayElement = ZLIndexOp::ZERO; dstArrayElement < binding.descriptorCount; ++dstArrayElement, ++writeIndex ) {

			( *this )[ writeIndex ] = MOAIGfxStructVK::writeDescriptorSet (
				VK_NULL_HANDLE, // unused by signature
				binding.binding,
				dstArrayElement,
				binding.descriptorType
			);
		}
	}
}

//----------------------------------------------------------------//
MOAIDescriptorSetSignatureVK::MOAIDescriptorSetSignatureVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetSignatureVK::~MOAIDescriptorSetSignatureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetSignatureVK::MOAIAbstractSnapshotSignatureVK_CloneFrom ( const MOAIDescriptorSetSignatureVK& other ) {

	this->ZLLeanArray < VkWriteDescriptorSet >::CloneFrom ( other );
}

//----------------------------------------------------------------//
bool MOAIDescriptorSetSignatureVK::MOAIAbstractSnapshotSignatureVK_IsSame ( const MOAIDescriptorSetSignatureVK& other ) const {

	return this->IsIdentical ( other );
}
