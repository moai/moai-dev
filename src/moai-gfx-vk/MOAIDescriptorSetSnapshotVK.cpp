// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>

//================================================================//
// MOAIDescriptorSetSnapshotVK
//================================================================//

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK::MOAIDescriptorSetSnapshotVK () :
	mDescriptorSet ( VK_NULL_HANDLE ) {
}

//----------------------------------------------------------------//
MOAIDescriptorSetSnapshotVK::~MOAIDescriptorSetSnapshotVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDescriptorSetSnapshotVK::MOAIAbstractSnapshotVK_OnUnpin () {

	this->GetDependency < MOAIDescriptorSetLayoutVK >().RetireDescriptorSet ( *this );
}
