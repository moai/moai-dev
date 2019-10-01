// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>

//================================================================//
// MOAIPipelineLayoutVK
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineLayoutVK::AffirmPipelineLayout () {

	if ( this->mPipelineLayout != VK_NULL_HANDLE ) return;
	
	ZLSize total = this->mDescriptorSetLayouts.Size ();
	VkDescriptorSetLayout* descriptorSetLayouts = ( VkDescriptorSetLayout* )alloca ( total * sizeof ( VkDescriptorSetLayout ));
	for ( ZLIndex i = ZLIndexOp::ZERO; i < total; ++i ) {
		descriptorSetLayouts [ i ] = this->mDescriptorSetLayouts [ i ];
	}
	
	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
	VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = MOAIGfxStructVK::pipelineLayoutCreateInfo ( descriptorSetLayouts, ( u32 )total );
	VK_CHECK_RESULT ( vkCreatePipelineLayout ( logicalDevice, &pPipelineLayoutCreateInfo, NULL, &this->mPipelineLayout ));
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK& MOAIPipelineLayoutVK::GetDescriptorSetLayout ( ZLIndex index ) {

	assert ( index < this->mDescriptorSetLayouts.Size ());
	return this->mDescriptorSetLayouts [ index ];
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK& MOAIPipelineLayoutVK::InitializeDescriptorSetLayout ( ZLIndex index, ZLSize size ) {

	assert ( index < this->mDescriptorSetLayouts.Size ());
	this->mDescriptorSetLayouts [ index ].Initialize ( this->GetLogicalDevice (), size );
	return this->mDescriptorSetLayouts [ index ];
}

//----------------------------------------------------------------//
void MOAIPipelineLayoutVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize size ) {

	assert ( this->mPipelineLayout == VK_NULL_HANDLE );
	logicalDevice.AddClient ( logicalDevice, *this );
	this->mDescriptorSetLayouts.Init ( size );
}

//----------------------------------------------------------------//
MOAIPipelineLayoutVK::MOAIPipelineLayoutVK () :
	mPipelineLayout ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIPipelineLayoutVK::~MOAIPipelineLayoutVK () {

	this->Finalize ();
}

////----------------------------------------------------------------//
//MOAIDescriptorSetVK* MOAIPipelineLayoutVK::ProcureDescriptorSet ( ZLIndex index ) {
//
//	assert ( index < this->mDescriptorSetLayouts.Size ());
//	return this->mDescriptorSetLayouts [ index ].ProcureDescriptorSet ();
//}
//
////----------------------------------------------------------------//
//void MOAIPipelineLayoutVK::SetDescriptorSetLayout ( ZLIndex index, MOAIDescriptorSetLayoutNameVK& name ) {
//
//	assert ( this->mPipelineLayout == VK_NULL_HANDLE );
//
//	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
//
//	assert ( index < this->mDescriptorSetLayouts.Size ());
//	this->mDescriptorSetLayouts [ index ].Initialize ( logicalDevice, name );
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineLayoutVK::MOAIAbstractLifecycleClientVK_Finalize () {
	
	MOAILogicalDeviceVK& logicalDevice = this->GetLogicalDevice ();
	if ( this->mPipelineLayout != VK_NULL_HANDLE ) {
		vkDestroyPipelineLayout ( logicalDevice, this->mPipelineLayout, NULL );
	}
	logicalDevice.RemoveClient ( *this );
}
