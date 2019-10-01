// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSignatureVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorSetVK
//================================================================//

//----------------------------------------------------------------//
VkWriteDescriptorSet* MOAIDescriptorSetVK::GetWriteDescriptorSet ( ZLIndex binding, ZLIndex arrayElement ) {

	MOAIDescriptorSetLayoutVK& layout = this->GetLayout ();

	if ( binding >= layout.GetSize ()) return NULL;

	const VkDescriptorSetLayoutBinding& layoutBinding = layout.mLayoutBindings [ ZLIndexCast ( binding )];

	if ( arrayElement >= layoutBinding.descriptorCount ) return NULL;

	// TODO: add type check as well?
	
	ZLIndex index = layout.mSignatureOffsets [ binding ];

	return &( *this )[ index + arrayElement ];
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::Initialize ( MOAIDescriptorSetLayoutVK& descriptorSetLayout ) {

	descriptorSetLayout.AddClient ( descriptorSetLayout, *this );
	this->MOAIDescriptorSetSignatureVK::Initialize ( descriptorSetLayout );
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::MOAIDescriptorSetVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::~MOAIDescriptorSetVK () {

	this->Finalize ();
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
void MOAIDescriptorSetVK::MOAIAbstractLifecycleClientVK_Finalize () {

	this->GetLayout ().RemoveClient ( *this );
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::SnapshotCache* MOAIDescriptorSetVK::MOAIAbstractSnapshotSubjectVK_GetCache () {

	return &this->GetLayout ();
}

//----------------------------------------------------------------//
const MOAIDescriptorSetSignatureVK* MOAIDescriptorSetVK::MOAIAbstractSnapshotSubjectVK_GetSignature () const {

	return this;
}

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIDescriptorSetVK::MOAIAbstractSnapshotSubjectVK_MakeSnapshot () {

	return this->GetLayout ().ProcureDescriptorSetSnapshot ( *this );
}
