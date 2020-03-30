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
	for ( ZLIndex i = 0; i < total; ++i ) {
		descriptorSetLayouts [ i ] = *this->mDescriptorSetLayouts [ i ];
	}
	
	MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
	VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = MOAIGfxStructVK::pipelineLayoutCreateInfo ( descriptorSetLayouts, ( u32 )total );
	VK_CHECK_RESULT ( vkCreatePipelineLayout ( logicalDevice, &pPipelineLayoutCreateInfo, NULL, &this->mPipelineLayout ));
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutVK& MOAIPipelineLayoutVK::GetDescriptorSetLayout ( ZLIndex index ) {

	assert ( index < this->mDescriptorSetLayouts.Size ());
	assert ( this->mDescriptorSetLayouts [ index ]);
	return *this->mDescriptorSetLayouts [ index ];
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

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIPipelineLayoutVK::SetDescriptorSetLayout ( ZLIndex index, MOAIDescriptorSetLayoutVK& layout ) {

	assert ( this->mPipelineLayout == VK_NULL_HANDLE );
	assert ( index < this->mDescriptorSetLayouts.Size ());
	assert ( !this->mDescriptorSetLayouts [ index ]);
	assert (( const void* )this->GetDependency < MOAILogicalDeviceVK >() == ( const void* )layout.GetDependency < MOAILogicalDeviceVK >());
	
	layout.AffirmDescriptorSetLayout ();
	this->mDescriptorSetLayouts [ index ] = &layout;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineLayoutVK::_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		if ( this->mPipelineLayout != VK_NULL_HANDLE ) {
			vkDestroyPipelineLayout ( logicalDevice, this->mPipelineLayout, NULL );
		}
	}
}
