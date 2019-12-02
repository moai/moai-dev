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
	
	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
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
	this->mDescriptorSetLayouts [ index ].Initialize ( this->GetDependency < MOAILogicalDeviceVK >(), size );
	return this->mDescriptorSetLayouts [ index ];
}

//----------------------------------------------------------------//
void MOAIPipelineLayoutVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize size ) {

	assert ( this->mPipelineLayout == VK_NULL_HANDLE );
	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	this->mDescriptorSetLayouts.Init ( size );
}

//----------------------------------------------------------------//
MOAIPipelineLayoutVK::MOAIPipelineLayoutVK () :
	mPipelineLayout ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIPipelineLayoutVK::~MOAIPipelineLayoutVK () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		if ( this->mPipelineLayout != VK_NULL_HANDLE ) {
			vkDestroyPipelineLayout ( logicalDevice, this->mPipelineLayout, NULL );
		}
	}
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
//	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
//
//	assert ( index < this->mDescriptorSetLayouts.Size ());
//	this->mDescriptorSetLayouts [ index ].Initialize ( logicalDevice, name );
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
