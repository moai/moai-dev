// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutNameVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

//================================================================//
// MOAIDescriptorSetVK
//================================================================//

//----------------------------------------------------------------//
VkWriteDescriptorSet* MOAIDescriptorSetVK::GetWriteDescriptorSet ( u32 binding, u32 arrayElement ) {

	const MOAIDescriptorSetLayoutNameVK& name = this->GetLayout ().GetName ();
	
	if ( binding >= name.Size () ) return NULL;
	
	const VkDescriptorSetLayoutBinding& layoutBinding = name [ ZLIndexCast ( binding )];
	
	if ( arrayElement >= layoutBinding.descriptorCount ) return NULL;
	
	// TODO: add type check?
	
	return &this->mWriteArrayBindings [ ZLIndexCast ( binding )][ arrayElement ];
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::InitWriteArray ( const MOAIDescriptorSetLayoutNameVK& name ) {

	// set up the pool
	ZLSize nBindings = name.Size ();
	ZLSize writeArraySize = 0;
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nBindings; ++i ) {
		const VkDescriptorSetLayoutBinding& binding = name [ i ];
		writeArraySize += binding.descriptorCount;
	}
	
	// set up the write array
	this->mWriteArray.Init ( writeArraySize );
	this->mWriteArrayBindings.Init ( nBindings );

	ZLIndex writeIndex = ZLIndexOp::ZERO;
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nBindings; ++i ) {
	
		const VkDescriptorSetLayoutBinding& binding = name [ i ];
		this->mWriteArrayBindings [ i ] = &this->mWriteArray [ writeIndex ];
		
		for ( u32 dstArrayElement = ZLIndexOp::ZERO; dstArrayElement < binding.descriptorCount; ++dstArrayElement, ++writeIndex ) {
			
			this->mWriteArray [ writeIndex ] = MOAIGfxStructVK::writeDescriptorSet (
				this->mDescriptorSet,
				binding.binding,
				dstArrayElement,
				binding.descriptorType
			);
		}
	}
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::MOAIDescriptorSetVK () :
	mDescriptorSet ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::~MOAIDescriptorSetVK () {

	this->Finalize ();
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( u32 binding, u32 arrayElement, VkBufferView* texelBufferView ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pTexelBufferView = texelBufferView;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( u32 binding, u32 arrayElement, VkDescriptorBufferInfo* bufferInfo ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pBufferInfo = bufferInfo;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( u32 binding, u32 arrayElement, VkDescriptorImageInfo* imageInfo ) {

	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
	assert ( writeDescriptorSet );
	writeDescriptorSet->pImageInfo = imageInfo;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::Update () {

	MOAILogicalDeviceVK& logicalDevice = this->GetLayout ().GetLogicalDevice ();
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )this->mWriteArray.Size (), this->mWriteArray.GetBuffer (), 0, NULL );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::MOAIAbstractCommandBufferMemberVK_Discard () {

	this->GetLayout ().DiscardDescriptorSet ( *this );
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::MOAIAbstractLifecycleClientVK_Finalize () {

	this->GetLayout ().InvalidateDescriptorSet ( *this );
}
