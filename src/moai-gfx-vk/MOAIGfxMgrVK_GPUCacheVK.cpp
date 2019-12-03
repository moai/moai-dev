// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_VertexCacheVK.h>
#include <moai-gfx-vk/MOAIGfxStateGPUCacheFrameVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//#define MOAIGFXSTATECACHE_DEBUG_LOG

#ifdef MOAIGFXSTATECACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIGfxStateGPUCacheFrameVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::ApplyStateChanges () {

	if ( !this->mApplyingStateChanges ) {

		if ( this->mDirtyFlags || this->mTextureDirtyFlags ) {

			this->SuspendChanges ();
			this->mCurrentState = this->mActiveState;

			this->GfxStateWillChange ();

//			u32 dirtyFlags = this->mDirtyFlags;
			this->mDirtyFlags = 0;

//			u32 textureDirtyFlags = this->mTextureDirtyFlags;
			this->mTextureDirtyFlags = 0;

			*this->mActiveState = *this->mPendingState;

			this->mCurrentState = this->mPendingState;
			this->ResumeChanges ();
		}
	}
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgrVK_GPUCacheVK () {

	this->mDefaultFrameBuffer = new MOAIFrameBufferVK ();
	
	this->mActiveState = new MOAIGfxStateGPUCacheFrameVK ();
	this->mPendingState = new MOAIGfxStateGPUCacheFrameVK ();
	
	this->mCurrentState = this->mPendingState;
	
	this->InitTextureUnits ( 16 );
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK::~MOAIGfxMgrVK_GPUCacheVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ClearSurface () {
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_CountTextureUnits () {

	return this->mMaxTextureUnits;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_DrawPrims ( u32 primType, u32 base, u32 count ) {
	UNUSED ( primType );
	UNUSED ( base );
	UNUSED ( count );
	
	VkPrimitiveTopology primTypeVK = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	
	// TODO: move this to a central location; change VK_ consts to MOAI_GFX_ superset of VK and GL
	switch ( primType ) {

		case ZGL_PRIM_LINE_STRIP:
			primTypeVK = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
			
		case ZGL_PRIM_LINES:
			primTypeVK = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
			
		case ZGL_PRIM_POINTS:
			primTypeVK = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			break;
			
		case ZGL_PRIM_TRIANGLE_FAN:
			primTypeVK = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			break;
			
		case ZGL_PRIM_TRIANGLE_STRIP:
			primTypeVK = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			break;
			
		case ZGL_PRIM_TRIANGLES:
			primTypeVK = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
			
		default:
			return;
	};
	
	assert ( primTypeVK != VK_PRIMITIVE_TOPOLOGY_MAX_ENUM );
	
	this->ApplyStateChanges ();
	
	MOAIGfxMgrVK& gfxMgr					= this->GetGfxMgrVK ();
	MOAILogicalDeviceVK& logicalDevice		= gfxMgr.GetLogicalDevice ();
	MOAICommandBufferVK& commandBuffer		= gfxMgr.GetCommandBuffer ();
	
	MOAIShaderVK* shader					= MOAICast < MOAIShaderVK >( this->GetShader ());
	MOAITexture2DVK* texture				= MOAICast < MOAITexture2DVK >( this->GetTexture ()); // TODO: obviously, fix this
	MOAIVertexFormatVK* vertexFormat 		= MOAICast < MOAIVertexFormatVK >( this->GetVertexFormat ());
	
	assert ( shader && texture && vertexFormat );

	// get the pipeline layout
	MOAIPipelineLayoutVK& pipelineLayout = shader->GetProgram ()->GetPipelineLayout ();

	// TODO: all the descriptor set initializion needs to be done from a spec inside
	// the layout object. the spec should bind the uniforms, textures, etc.

	// initialize the descriptor set
	MOAIDescriptorSetLayoutVK& descriptorSetLayout = pipelineLayout.GetDescriptorSetLayout ( ZLIndexOp::ZERO );
	
	MOAIDescriptorSetVK* descriptorSet = new MOAIDescriptorSetVK ();
	descriptorSet->Initialize ( descriptorSetLayout );
	descriptorSet->SetDescriptor ( ZLIndexCast ( 0 ), ZLIndexCast ( 0 ), *texture->GetSnapshot ( commandBuffer ));
	
	commandBuffer.BindDescriptorSet ( VK_PIPELINE_BIND_POINT_GRAPHICS, *descriptorSet->GetSnapshot ( commandBuffer ), pipelineLayout, 0 );
	
	MOAIPipelineSnapshotVK* pipeline = new MOAIPipelineSnapshotVK ();
	pipeline->Initialize (
		logicalDevice,
		gfxMgr.GetRenderPass (),
		primTypeVK,
		vertexFormat,
		shader
	);
	commandBuffer.BindPipeline ( VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline );
	commandBuffer.Pin ( *pipeline );

	// set the viewport and scissor
	VkViewport viewport			= MOAIGfxStructVK::viewport ( this->mActiveState->mViewRect, 0.0, 1.0 );
	VkRect2D scissor			= MOAIGfxStructVK::rect2D ( this->mActiveState->mScissorRect );

	vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );
	vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

	MOAIVertexBufferVK* vtxBuffer = MOAICast < MOAIVertexBufferVK >( this->mActiveState->mVtxBuffer );
	MOAIIndexBufferVK* idxBuffer = MOAICast < MOAIIndexBufferVK >( this->mActiveState->mIdxBuffer );

	assert ( vtxBuffer && idxBuffer );

	VkIndexType indexType = idxBuffer->GetIndexSize () == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;

	MOAIGfxBufferSnapshotVK* vtxBufferSnapshot = vtxBuffer->GetSnapshot ( commandBuffer );
	MOAIGfxBufferSnapshotVK* idxBufferSnapshot = idxBuffer->GetSnapshot ( commandBuffer );

	VkDeviceSize offsets [] = { 0 };
	vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &vtxBufferSnapshot->GetBuffer (), offsets );
	vkCmdBindIndexBuffer ( commandBuffer, idxBufferSnapshot->GetBuffer (), 0, indexType );
	vkCmdDrawIndexed ( commandBuffer, count, 1, base, 0, 1 );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ResetGPUState () {
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_UnbindAll () {
}

