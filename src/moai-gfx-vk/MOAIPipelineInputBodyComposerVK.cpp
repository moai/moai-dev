// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIPipelineInputBodyComposerVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkSchemaVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineInputChunkVK.h>
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIPipelineInputBodyComposerVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxStateGPUCacheFrameVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
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
// MOAIGfxScriptTextureCommandVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptTextureCommandVK::MOAIGfxScriptTextureCommandVK ( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit ) :
	MOAIGfxScriptCommandVK ( descriptorSetIndex, bindPoint, arrayItem ),
	mTextureUnit ( textureUnit ) {
}

//----------------------------------------------------------------//
void MOAIGfxScriptTextureCommandVK::MOAIGfxScriptCommandVK_Apply ( MOAIGfxMgrVK& gfxMgr, MOAIPipelineInputChunkVK& descriptorSet, MOAICommandBufferVK& commandBuffer ) {

	MOAITexture2DVK* texture = MOAICast < MOAITexture2DVK >( gfxMgr.GetTexture ( this->mTextureUnit ));
	descriptorSet.SetDescriptor ( this->mBindPoint, this->mArrayItem, *texture->GetSnapshot ( commandBuffer ));
}

//================================================================//
// MOAIPipelineInputBodyComposerVK
//================================================================//

//----------------------------------------------------------------//
void MOAIPipelineInputBodyComposerVK::ComposeAndBind (  MOAIGfxMgrVK& gfxMgr, MOAICommandBufferVK& commandBuffer, VkPipelineBindPoint pipelineBindPoint ) {

	gfxMgr.GfxStateWillChange (); // TODO: need to do this only if graphics state will actually change

	// TODO: pass in listener to detect changes (to invalidate graphics cache)

	for ( ZLIndex i = 0; i < this->mCommandCount; ++i ) {
		MOAIGfxScriptCommandVK* command = this->mCommands [ i ];
		if ( command ) {
			MOAIPipelineInputChunkVK& descriptorSet = this->mDescriptorSets [ command->mDescriptorSetIndex ];
			command->Apply ( gfxMgr, descriptorSet, commandBuffer );
		}
	}
	
	for ( ZLIndex i = 0; i < this->mDescriptorSets.Size (); ++i ) {
		MOAIPipelineInputChunkVK& descriptorSet = this->mDescriptorSets [ i ];
		commandBuffer.BindDescriptorSet ( pipelineBindPoint, *descriptorSet.GetSnapshot ( commandBuffer ), *this->mPipelineLayout, 0 );
	}
}

//----------------------------------------------------------------//
MOAIPipelineInputBodyComposerVK::MOAIPipelineInputBodyComposerVK () :
	mCommandCount ( 0 ) {
}

//----------------------------------------------------------------//
MOAIPipelineInputBodyComposerVK::~MOAIPipelineInputBodyComposerVK () {
}

//----------------------------------------------------------------//
void MOAIPipelineInputBodyComposerVK::PushTextureCommand ( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit ) {

	this->mCommands [ this->mCommandCount++ ] = new MOAIGfxScriptTextureCommandVK ( descriptorSetIndex, bindPoint, arrayItem, textureUnit );
}

//----------------------------------------------------------------//
void MOAIPipelineInputBodyComposerVK::Reserve ( ZLSize size ) {

	this->mCommands.Init ( size );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
