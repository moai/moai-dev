// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetLayoutNameVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>

//================================================================//
// MOAIDescriptorSetLayoutNameVK
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutNameVK::MOAIDescriptorSetLayoutNameVK () {
}

//----------------------------------------------------------------//
MOAIDescriptorSetLayoutNameVK::~MOAIDescriptorSetLayoutNameVK () {
}

//----------------------------------------------------------------//
void MOAIDescriptorSetLayoutNameVK::SetBinding ( u32 binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, u32 descriptorCount ) {

	assert ( binding < this->Size ());
	( *this )[ ZLIndexCast ( binding )] = MOAIGfxStructVK::descriptorSetLayoutBinding (
		binding,
		descriptorType,
		stageFlags,
		descriptorCount,
		NULL
	);
}
