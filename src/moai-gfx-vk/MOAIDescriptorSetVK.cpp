// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorSetVK
//================================================================//

//----------------------------------------------------------------//
VkWriteDescriptorSet* MOAIDescriptorSetVK::GetWriteDescriptorSet ( ZLIndex binding, ZLIndex arrayElement ) {

	MOAIDescriptorSetLayoutVK& layout = this->GetProvider < MOAIDescriptorSetLayoutVK >();

	if ( binding >= layout.GetSize ()) return NULL;

	const VkDescriptorSetLayoutBinding& layoutBinding = layout.mLayoutBindings [ ZLIndexCast ( binding )];

	if ( arrayElement >= layoutBinding.descriptorCount ) return NULL;

	// TODO: add type check as well?
	
	ZLIndex index = layout.mSignatureOffsets [ binding ];

	return &( *this )[ index + arrayElement ];
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::Initialize ( MOAIDescriptorSetLayoutVK& descriptorSetLayout ) {

	this->SetProvider < MOAIDescriptorSetLayoutVK >( descriptorSetLayout );
	
	// set up the pool
	ZLSize nBindings		= descriptorSetLayout.GetSize ();
	ZLSize signatureSize	= descriptorSetLayout.GetSignatureSize ();

	// set up the write array
	this->Init ( signatureSize );

	ZLIndex writeIndex = ZLIndexOp::ZERO;
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nBindings; ++i ) {

		const VkDescriptorSetLayoutBinding& binding = descriptorSetLayout.mLayoutBindings [ i ];

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
MOAIDescriptorSetVK::MOAIDescriptorSetVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::~MOAIDescriptorSetVK () {
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pTexelBufferView = texelBufferView;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pBufferInfo = bufferInfo;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pImageInfo = imageInfo;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIDescriptorSetVK::MOAIAbstractSnapshotSubjectVK_MakeSnapshot () {

	return this->GetProvider < MOAIDescriptorSetLayoutVK >().ProcureDescriptorSetSnapshot ( *this );
}
