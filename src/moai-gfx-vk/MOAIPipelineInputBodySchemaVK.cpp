// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIPipelineInputBodySchemaVK.h>

//================================================================//
// MOAIPipelineInputBodySchemaVK
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineInputBodySchemaVK::AffirmPipelineLayout () {

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
MOAIPipelineInputChunkSchemaVK& MOAIPipelineInputBodySchemaVK::GetDescriptorSetLayout ( ZLIndex index ) {

	assert ( index < this->mDescriptorSetLayouts.Size ());
	assert ( this->mDescriptorSetLayouts [ index ]);
	return *this->mDescriptorSetLayouts [ index ];
}

//----------------------------------------------------------------//
void MOAIPipelineInputBodySchemaVK::Initialize ( MOAILogicalDeviceVK& logicalDevice, ZLSize size ) {

	assert ( this->mPipelineLayout == VK_NULL_HANDLE );
	this->SetDependency < MOAILogicalDeviceVK >( logicalDevice );
	this->mDescriptorSetLayouts.Init ( size );
}

//----------------------------------------------------------------//
MOAIPipelineInputBodySchemaVK::MOAIPipelineInputBodySchemaVK () :
	mPipelineLayout ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIPipelineInputBodySchemaVK::~MOAIPipelineInputBodySchemaVK () {

	this->Destruct ();
}

//----------------------------------------------------------------//
void MOAIPipelineInputBodySchemaVK::SetDescriptorSetLayout ( ZLIndex index, MOAIPipelineInputChunkSchemaVK& layout ) {

	assert ( this->mPipelineLayout == VK_NULL_HANDLE );
	assert ( index < this->mDescriptorSetLayouts.Size ());
	assert ( !this->mDescriptorSetLayouts [ index ]);
	assert (( const void* )this->GetDependency < MOAILogicalDeviceVK >() == ( const void* )layout.GetDependency < MOAILogicalDeviceVK >());
	
	layout.AffirmDescritorSetLayout ();
	this->mDescriptorSetLayouts [ index ] = &layout;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineInputBodySchemaVK::_Finalize () {

	if ( this->HasDependency < MOAILogicalDeviceVK >()) {
		MOAILogicalDeviceVK& logicalDevice = this->GetDependency < MOAILogicalDeviceVK >();
		if ( this->mPipelineLayout != VK_NULL_HANDLE ) {
			vkDestroyPipelineLayout ( logicalDevice, this->mPipelineLayout, NULL );
		}
	}
}
