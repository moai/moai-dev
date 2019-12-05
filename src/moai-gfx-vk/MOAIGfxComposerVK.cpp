// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxComposerVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxComposerVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxStateGPUCacheFrameVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIGfxComposerTextureCommandVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxComposerTextureCommandVK::MOAIGfxComposerTextureCommandVK ( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit ) :
	MOAIGfxComposerCommandVK ( descriptorSetIndex, bindPoint, arrayItem ),
	mTextureUnit ( textureUnit ) {
}

//----------------------------------------------------------------//
void MOAIGfxComposerTextureCommandVK::MOAIGfxComposerCommandVK_Apply ( MOAIDescriptorSetVK& descriptorSet, MOAICommandBufferVK& commandBuffer ) {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAITexture2DVK* texture = MOAICast < MOAITexture2DVK >( gfxMgr.GetTexture ( this->mTextureUnit ));
	
	descriptorSet.SetDescriptor ( this->mBindPoint, this->mArrayItem, *texture->GetSnapshot ( commandBuffer ));
}

//================================================================//
// MOAIGfxComposerVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposerVK::ApplyAndBind ( MOAICommandBufferVK& commandBuffer, VkPipelineBindPoint pipelineBindPoint ) {

	// TODO: pass in listener to detect changes (to invalidate graphics cache)

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mCommandCount; ++i ) {
		MOAIGfxComposerCommandVK* command = this->mCommands [ i ];
		if ( command ) {
			MOAIDescriptorSetVK& descriptorSet = this->mDescriptorSets [ command->mDescriptorSetIndex ];
			command->Apply ( descriptorSet, commandBuffer );
			commandBuffer.BindDescriptorSet ( pipelineBindPoint, *descriptorSet.GetSnapshot ( commandBuffer ), *this->mPipelineLayout, 0 );
		}
	}
}

//----------------------------------------------------------------//
MOAIDescriptorSetVK& MOAIGfxComposerVK::GetDescriptorSet ( ZLIndex index ) {

	return this->mDescriptorSets [ index ];
}

//----------------------------------------------------------------//
MOAIPipelineLayoutVK& MOAIGfxComposerVK::GetPipelineLayout () {

	assert ( this->mPipelineLayout );
	return *this->mPipelineLayout;
}

//----------------------------------------------------------------//
MOAIGfxComposerVK::MOAIGfxComposerVK () :
	mCommandCount ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGfxComposerVK::~MOAIGfxComposerVK () {
}

//----------------------------------------------------------------//
void MOAIGfxComposerVK::PushTextureCommand ( ZLIndex descriptorSetIndex, ZLIndex bindPoint, ZLIndex arrayItem, ZLIndex textureUnit ) {

	this->mCommands [ ZLIndexCast ( this->mCommandCount++ )] = new MOAIGfxComposerTextureCommandVK ( descriptorSetIndex, bindPoint, arrayItem, textureUnit );
}

//----------------------------------------------------------------//
void MOAIGfxComposerVK::Reserve ( ZLSize size ) {

	this->mCommands.Init ( size );
}

//----------------------------------------------------------------//
void MOAIGfxComposerVK::SetPipelineLayout ( MOAIPipelineLayoutVK& pipelineLayout ) {

	this->Finalize ();
	
	pipelineLayout.AffirmPipelineLayout ();
	this->mPipelineLayout = &pipelineLayout;
	
	ZLSize nLayouts = this->mPipelineLayout->mDescriptorSetLayouts.Size ();
	this->mDescriptorSets.Init ( nLayouts );
	
	for ( ZLIndex i = ZLIndexOp::ZERO; i < nLayouts; ++i ) {
		MOAIDescriptorSetVK& descriptorSet = this->mDescriptorSets [ i ];
		descriptorSet.Initialize ( this->mPipelineLayout->GetDescriptorSetLayout ( i ));
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
