// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetArrayVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetStateVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>

//================================================================//
// MOAIDescriptorSetArrayVK
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetArrayVK::Bind ( MOAICommandBufferVK& commandBuffer, VkPipelineBindPoint pipelineBindPoint ) {

	for ( ZLIndex i = 0; i < this->mDescriptorSets.Size (); ++i ) {
		MOAIDescriptorSetStateVK& descriptorSet = this->mDescriptorSets [ i ];
		MOAIDescriptorSetVK& snapshot = *descriptorSet.GetDescriptorSet ();
		commandBuffer.BindDescriptorSet ( pipelineBindPoint, snapshot, *this->mPipelineLayout, 0 );
	}
}

//----------------------------------------------------------------//
MOAIDescriptorSetStateVK& MOAIDescriptorSetArrayVK::GetDescriptorSet ( ZLIndex index ) {

	return this->mDescriptorSets [ index ];
}

//----------------------------------------------------------------//
MOAIPipelineLayoutVK& MOAIDescriptorSetArrayVK::GetPipelineLayout () {

	assert ( this->mPipelineLayout );
	return *this->mPipelineLayout;
}

//----------------------------------------------------------------//
MOAIDescriptorSetArrayVK::MOAIDescriptorSetArrayVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetArrayVK::~MOAIDescriptorSetArrayVK () {
}

//----------------------------------------------------------------//
void MOAIDescriptorSetArrayVK::SetPipelineLayout ( MOAIPipelineLayoutVK& pipelineLayout ) {

	this->Finalize ();
	
	pipelineLayout.AffirmPipelineLayout ();
	this->mPipelineLayout = &pipelineLayout;
	
	ZLSize nLayouts = this->mPipelineLayout->mDescriptorSetLayouts.Size ();
	this->mDescriptorSets.Init ( nLayouts );
	
	for ( ZLIndex i = 0; i < nLayouts; ++i ) {
		MOAIDescriptorSetStateVK& descriptorSet = this->mDescriptorSets [ i ];
		descriptorSet.Initialize ( this->mPipelineLayout->GetDescriptorSetLayout ( i ));
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
