// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIPipelineInputChunkSchemaVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIPipelineInputChunkVK
//================================================================//

//----------------------------------------------------------------//
VkWriteDescriptorSet* MOAIPipelineInputChunkVK::GetWriteDescriptorSet ( ZLIndex binding, ZLIndex arrayElement ) {

	MOAIPipelineInputChunkSchemaVK& layout = this->GetDependency < MOAIPipelineInputChunkSchemaVK >();

	if ( binding >= layout.GetSize ()) return NULL;

	const VkDescriptorSetLayoutBinding& layoutBinding = layout.mLayoutBindings [ binding ];

	if ( arrayElement >= layoutBinding.descriptorCount ) return NULL;

	// TODO: add type check as well?
	
	ZLIndex index = layout.mSignatureOffsets [ binding ];

	return &( *this )[ index + arrayElement ];
}

//----------------------------------------------------------------//
void MOAIPipelineInputChunkVK::Initialize ( MOAIPipelineInputChunkSchemaVK& descriptorSetLayout ) {

	this->SetDependency < MOAIPipelineInputChunkSchemaVK >( descriptorSetLayout );
	
	// set up the pool
	ZLSize nBindings		= descriptorSetLayout.GetSize ();
	ZLSize signatureSize	= descriptorSetLayout.GetSignatureSize ();

	// set up the write array
	this->Init ( signatureSize );

	ZLIndex writeIndex = 0;
	for ( ZLIndex i = 0; i < nBindings; ++i ) {

		const VkDescriptorSetLayoutBinding& binding = descriptorSetLayout.mLayoutBindings [ i ];

		for ( u32 dstArrayElement = 0; dstArrayElement < binding.descriptorCount; ++dstArrayElement, ++writeIndex ) {

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
MOAIPipelineInputChunkVK::MOAIPipelineInputChunkVK () {
}

//----------------------------------------------------------------//
MOAIPipelineInputChunkVK::~MOAIPipelineInputChunkVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIPipelineInputChunkVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pTexelBufferView = texelBufferView;
}

//----------------------------------------------------------------//
void MOAIPipelineInputChunkVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pBufferInfo = bufferInfo;
}

//----------------------------------------------------------------//
void MOAIPipelineInputChunkVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pImageInfo = imageInfo;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIPipelineInputChunkVK::MOAIAbstractSnapshotFactoryVK_GetSnapshot ( MOAICommandBufferVK& commandBuffer ) {
	UNUSED ( commandBuffer );

	if ( this->HasDependency < MOAIPipelineInputChunkSchemaVK >()) {
		return this->GetDependency < MOAIPipelineInputChunkSchemaVK >().ProcureDescriptorSet ( *this );
	}
	return NULL;
}
