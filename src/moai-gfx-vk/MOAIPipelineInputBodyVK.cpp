// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIPipelineInputBodyVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkSchemaVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkVK.h>
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineInputBodyVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxStateGPUCacheFrameVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPipelineInputBodyVK.h>
#include <moai-gfx-vk/MOAIPipelineInputBodySchemaVK.h>
#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIPipelineInputBodyVK
//================================================================//

//----------------------------------------------------------------//
MOAIPipelineInputChunkVK& MOAIPipelineInputBodyVK::GetDescriptorSet ( ZLIndex index ) {

	return this->mDescriptorSets [ index ];
}

//----------------------------------------------------------------//
MOAIPipelineInputBodySchemaVK& MOAIPipelineInputBodyVK::GetPipelineLayout () {

	assert ( this->mPipelineLayout );
	return *this->mPipelineLayout;
}

//----------------------------------------------------------------//
MOAIPipelineInputBodyVK::MOAIPipelineInputBodyVK () {
}

//----------------------------------------------------------------//
MOAIPipelineInputBodyVK::~MOAIPipelineInputBodyVK () {
}

//----------------------------------------------------------------//
void MOAIPipelineInputBodyVK::SetPipelineLayout ( MOAIPipelineInputBodySchemaVK& pipelineLayout ) {

	this->Finalize ();
	
	pipelineLayout.AffirmPipelineLayout ();
	this->mPipelineLayout = &pipelineLayout;
	
	ZLSize nLayouts = this->mPipelineLayout->mDescriptorSetLayouts.Size ();
	this->mDescriptorSets.Init ( nLayouts );
	
	for ( ZLIndex i = 0; i < nLayouts; ++i ) {
		MOAIPipelineInputChunkVK& descriptorSet = this->mDescriptorSets [ i ];
		descriptorSet.Initialize ( this->mPipelineLayout->GetDescriptorSetLayout ( i ));
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
