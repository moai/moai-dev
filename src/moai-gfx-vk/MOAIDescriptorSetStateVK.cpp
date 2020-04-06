// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDescriptorElementStateVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetStateVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
	
//================================================================//
// MOAIDescriptorSetStateVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetStateVK::Initialize ( MOAIDescriptorSetLayoutVK& descriptorSetLayout ) {

	this->SetDependency < MOAIDescriptorSetLayoutVK >( descriptorSetLayout );
	this->mSignature.Resize ( descriptorSetLayout.mSignatureSize, NULL );
	
	ZLSize nBindings = descriptorSetLayout.GetSize ();

	this->mDescriptors.Init ( nBindings );
	this->mWriteDescriptors.Init ( nBindings );
	this->mMutableWriteDescriptors.Init ( nBindings );

	ZLIndex writeIndex = 0;
	for ( ZLIndex i = 0; i < nBindings; ++i ) {

		const VkDescriptorSetLayoutBinding& binding = descriptorSetLayout.mLayoutBindings [ i ];
		MOAIDescriptorStateVK& descriptor = this->mDescriptors [ i ];

		descriptor.mElements.Init ( binding.descriptorCount );
		
		VkWriteDescriptorSet& writeDescriptorSet = this->mWriteDescriptors [ i ];
		MOAIMutableWriteDescriptorSetVK& mutableWriteDescriptorSet = this->mMutableWriteDescriptors [ i ];
		
		writeDescriptorSet.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.pNext				= NULL;
		writeDescriptorSet.dstSet				= VK_NULL_HANDLE;
		writeDescriptorSet.descriptorType		= binding.descriptorType;
		writeDescriptorSet.dstBinding			= binding.binding;
		writeDescriptorSet.dstArrayElement		= 0;
		writeDescriptorSet.descriptorCount		= binding.descriptorCount;
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
				mutableWriteDescriptorSet.mImageInfo = ( VkDescriptorImageInfo* )descriptor.mInfoArray.GetBuffer ();
				writeDescriptorSet.pImageInfo = mutableWriteDescriptorSet.mImageInfo;
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
	}
}

//----------------------------------------------------------------//
MOAIDescriptorSetStateVK::MOAIDescriptorSetStateVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetStateVK::~MOAIDescriptorSetStateVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIDescriptorSetStateVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementStateVK* descriptor ) {

	this->mDescriptors [ binding ].mElements [ arrayElement ] = descriptor;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetVK* MOAIDescriptorSetStateVK::MOAIAbstractSnapshotFactoryVK_GetSnapshot () {

	if ( !this->HasDependency < MOAIDescriptorSetLayoutVK >()) return NULL;

	// TODO: can skip a lot of this if no pending change
	ZLSize sigTop = 0;
	ZLSize totalSets = this->mDescriptors.Size ();
	for ( ZLIndex i = 0; i < totalSets; ++i ) {
	
		MOAIMutableWriteDescriptorSetVK& mutableWriteDescriptorSet = this->mMutableWriteDescriptors [ i ];
		MOAIDescriptorStateVK& descriptorState = this->mDescriptors [ i ];
		
		for ( ZLIndex j = 0; j < descriptorState.mElements.Size (); ++j ) {
			MOAIAbstractDescriptorElementStateVK* elementState = descriptorState.mElements [ i ];
			assert ( elementState );
						
			MOAIAbstractDescriptorElementVK* element = descriptorState.mElements [ i ]->GetElement ( mutableWriteDescriptorSet, j );
			this->mSignature [ sigTop++ ] = element;
		}
	}
	
	return this->GetDependency < MOAIDescriptorSetLayoutVK >().ProcureDescriptorSet ( *this );
}
