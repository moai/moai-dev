// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDescriptorElementVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorVK
//================================================================//
	
//----------------------------------------------------------------//
bool MOAIDescriptorVK::UpdateAndPin ( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer ) {

	for ( ZLIndex i = 0; i < this->mElements.Size (); ++i ) {
		MOAIAbstractDescriptorElementVK* element = this->mElements [ i ];
		if ( !element ) return false;
		this->mElements [ i ]->GetPinnedData ( writeDescriptorSet, index, commandBuffer );
	}
	return true;
}

//================================================================//
// MOAIDescriptorSetVK
//================================================================//

////----------------------------------------------------------------//
//VkWriteDescriptorSet* MOAIDescriptorSetVK::GetWriteDescriptorSet ( ZLIndex binding, ZLIndex arrayElement ) {
//
//	MOAIDescriptorSetLayoutVK& layout = this->GetDependency < MOAIDescriptorSetLayoutVK >();
//
//	if ( binding >= layout.GetSize ()) return NULL;
//
//	const VkDescriptorSetLayoutBinding& layoutBinding = layout.mLayoutBindings [ binding ];
//
//	if ( arrayElement >= layoutBinding.descriptorCount ) return NULL;
//
//	// TODO: add type check as well?
//
//	ZLIndex index = layout.mSignatureOffsets [ binding ];
//
//	return &( *this )[ index + arrayElement ];
//}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::Initialize ( MOAIDescriptorSetLayoutVK& descriptorSetLayout ) {

	this->SetDependency < MOAIDescriptorSetLayoutVK >( descriptorSetLayout );
	
	ZLSize nBindings = descriptorSetLayout.GetSize ();

	this->mDescriptors.Init ( nBindings );
	this->mWriteDescriptors.Init ( nBindings );

	ZLIndex writeIndex = 0;
	for ( ZLIndex i = 0; i < nBindings; ++i ) {

		const VkDescriptorSetLayoutBinding& binding = descriptorSetLayout.mLayoutBindings [ i ];
		MOAIDescriptorVK& descriptor = this->mDescriptors [ i ];

		descriptor.mElements.Init ( binding.descriptorCount );
		
		
		VkWriteDescriptorSet writeDescriptorSet;
		
		writeDescriptorSet.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.pNext				= NULL;
		writeDescriptorSet.dstSet				= VK_NULL_HANDLE;
		writeDescriptorSet.descriptorType		= binding.descriptorType;
		writeDescriptorSet.dstBinding			= binding.binding;
		writeDescriptorSet.dstArrayElement		= 0;
		writeDescriptorSet.pImageInfo			= NULL;
		writeDescriptorSet.pBufferInfo			= NULL;
		writeDescriptorSet.pTexelBufferView		= NULL;
		
		switch ( binding.descriptorType ) {
			case VK_DESCRIPTOR_TYPE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
			case VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT:
			case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV:
			
				// TODO: obviously, this is wrong - all types are treated as VkDescriptorImageInfo (for now)
				descriptor.mInfoArray.Init ( binding.descriptorCount * sizeof ( VkDescriptorImageInfo ));
				writeDescriptorSet.pImageInfo = ( const VkDescriptorImageInfo* )descriptor.mInfoArray.GetBuffer ();
//				VkDescriptorImageInfo
//				VkDescriptorBufferInfo
//				VkBufferView
				break;
				
			default:
				assert ( false );
		}
		
		const VkDescriptorImageInfo*     pImageInfo;
		const VkDescriptorBufferInfo*    pBufferInfo;
		const VkBufferView*              pTexelBufferView;
		
		this->mWriteDescriptors [ i ] = writeDescriptorSet;
		
//		descriptor.mWriter = MOAIGfxStructVK::writeDescriptorSet (
//			VK_NULL_HANDLE, // unused by signature
//			binding.binding,
//			0,
//			binding.descriptorType,
//			binding.descriptorCount
//		);
	}
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::MOAIDescriptorSetVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::~MOAIDescriptorSetVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementVK* descriptor ) {

	this->mDescriptors [ binding ].mElements [ arrayElement ] = descriptor;
}

////----------------------------------------------------------------//
//void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkBufferView* texelBufferView ) {
//
//	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
//	assert ( writeDescriptorSet );
//	writeDescriptorSet->pTexelBufferView = texelBufferView;
//}
//
////----------------------------------------------------------------//
//void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkDescriptorBufferInfo* bufferInfo ) {
//
//	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
//	assert ( writeDescriptorSet );
//	writeDescriptorSet->pBufferInfo = bufferInfo;
//}
//
////----------------------------------------------------------------//
//void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, VkDescriptorImageInfo* imageInfo ) {
//
//	VkWriteDescriptorSet* writeDescriptorSet = this->GetWriteDescriptorSet ( binding, arrayElement );
//	assert ( writeDescriptorSet );
//	writeDescriptorSet->pImageInfo = imageInfo;
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIDescriptorSetVK::MOAIAbstractSnapshotFactoryVK_GetSnapshot ( MOAICommandBufferVK& commandBuffer ) {
	UNUSED ( commandBuffer );

	if ( !this->HasDependency < MOAIDescriptorSetLayoutVK >()) return NULL;
	
	MOAIDescriptorSetLayoutVK& descriptorSetLayout = this->GetDependency < MOAIDescriptorSetLayoutVK >();
	
	MOAIDescriptorSetSnapshotVK* snapshot = descriptorSetLayout.ProcureDescriptorSet ( *this );
	if ( !snapshot ) return NULL;

	MOAILogicalDeviceVK& logicalDevice = descriptorSetLayout.GetDependency < MOAILogicalDeviceVK >();

	ZLSize totalSets = this->mDescriptors.Size ();
	for ( ZLIndex i = 0; i < totalSets; ++i ) {
		VkWriteDescriptorSet& writeDescriptorSet = this->mWriteDescriptors [ i ];
		bool result = this->mDescriptors [ i ].UpdateAndPin ( writeDescriptorSet, i, commandBuffer );
		assert ( result );
		writeDescriptorSet.dstSet = *snapshot;
	}
	vkUpdateDescriptorSets ( logicalDevice, ( u32 )totalSets, this->mWriteDescriptors.GetBuffer (), 0, NULL );
	
	return snapshot;
}
