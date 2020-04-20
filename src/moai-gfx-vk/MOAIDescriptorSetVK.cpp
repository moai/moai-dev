// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIAbstractDescriptorElementStateVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
	
//================================================================//
// MOAIDescriptorSetVK
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK* MOAIDescriptorSetVK::GetDescriptorSet () {

	if ( !this->HasDependency < MOAIDescriptorSetLayoutVK >()) return NULL;

	ZLSize sigTop = 0;
	bool didChange = !this->mDescriptorSet;
	ZLSize totalSets = this->mDescriptors.Size ();
	for ( ZLIndex i = 0; i < totalSets; ++i ) {
	
		MOAIMutableWriteDescriptorSetVK& mutableWriteDescriptorSet = this->mMutableWriteDescriptors [ i ];
		MOAIDescriptorVK& descriptorState = this->mDescriptors [ i ];
		
		for ( ZLIndex j = 0; j < descriptorState.mElements.Size (); ++j ) {
			MOAIAbstractDescriptorElementStateVK* elementState = descriptorState.mElements [ i ];
			assert ( elementState );
						
			MOAIAbstractDescriptorElementVK* element = descriptorState.mElements [ i ]->GetElement ( mutableWriteDescriptorSet, j );
			if ( this->mSignature [ sigTop ] != element ) {
				this->mSignature [ sigTop ] = element;
				didChange = true;
			}
			++sigTop;
		}
	}
	
	if ( didChange ) {
		this->mDescriptorSet = this->GetDependency < MOAIDescriptorSetLayoutVK >().ProcureDescriptorSet ( *this );
	}
	return this->mDescriptorSet;
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::Initialize ( MOAIDescriptorSetLayoutVK& descriptorSetLayout ) {

	this->SetDependency < MOAIDescriptorSetLayoutVK >( descriptorSetLayout );
	this->mSignature.Resize ( descriptorSetLayout.GetSignatureSize (), NULL );
	
	ZLSize nBindings = descriptorSetLayout.GetSize ();

	this->mDescriptors.Init ( nBindings );
	this->mWriteDescriptors.Init ( nBindings );
	this->mMutableWriteDescriptors.Init ( nBindings );

	ZLIndex writeIndex = 0;
	for ( ZLIndex i = 0; i < nBindings; ++i ) {

		const VkDescriptorSetLayoutBinding& binding = ( *descriptorSetLayout.mLayoutBindings )[ i ];
		MOAIDescriptorVK& descriptor = this->mDescriptors [ i ];

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
MOAIDescriptorSetVK::MOAIDescriptorSetVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK::~MOAIDescriptorSetVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIDescriptorSetVK::SetDescriptor ( ZLIndex binding, ZLIndex arrayElement, MOAIAbstractDescriptorElementStateVK* descriptor ) {

	this->mDescriptors [ binding ].mElements [ arrayElement ] = descriptor;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
