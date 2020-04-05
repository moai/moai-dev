// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIDescriptorSetArrayVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetStateVK.h>
#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxConstsVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_VertexCacheVK.h>
#include <moai-gfx-vk/MOAIGfxStateGPUCacheFrameVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIPipelineVK.h>
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
MOAIPipelineVK* MOAIGfxMgrVK_GPUCacheVK::AffirmPipeline ( MOAIPipelineParamsVK& params ) {

	ZLStrongPtr < MOAIPipelineVK > pipeline = this->mPipelinePool [ params ];
	if ( !pipeline ) {
		pipeline = new MOAIPipelineVK ();
		pipeline->Initialize ( params );
		this->mPipelinePool [ params ] = pipeline;
	}
	return pipeline;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ApplyStateChanges () {

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
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ClearSurface () {

//	MOAIGfxMgr_CPUCache& cpuCache = this->GetCPUCache ();
//	
//	u32 clearFlags = cpuCache.GetClearFlags ();
//
//	if ( clearFlags ) {
//		
//		// discard this if all buffers are to be cleared?
//		// (may still need to draw if depth or color only)
//		this->GetVertexCache ().FlushToGPU ();
//		
//		this->ApplyStateChanges ();
//	
//		if ( clearFlags & ZLGfxClearFlags::COLOR_BUFFER_BIT ) {
//		
//			const ZLColorVec& clearColor = cpuCache.GetClearColor ();
//		
//			gfx.ClearColor (
//				clearColor.mR,
//				clearColor.mG,
//				clearColor.mB,
//				clearColor.mA
//			);
//		}
//	
//		if (( clearFlags & ZLGfxClearFlags::DEPTH_BUFFER_BIT ) && !this->GetDepthMask ()) {
//			gfx.DepthMask ( true );
//			gfx.Clear ( clearFlags );
//			gfx.DepthMask ( false );
//		}
//		else {
//			gfx.Clear ( clearFlags );
//		}
//	}
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_CountTextureUnits () {

	return this->mMaxTextureUnits;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_DrawPrims ( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) {
	UNUSED ( primType );
	UNUSED ( base );
	UNUSED ( count );
	
	VkPrimitiveTopology primTypeVK = MOAIGfxConstsVK::Remap ( primType );
	
	this->ApplyStateChanges ();
	
	MOAIGfxMgrVK& gfxMgr					= this->GetGfxMgrVK ();
	MOAILogicalDeviceVK& logicalDevice		= gfxMgr.GetLogicalDevice ();
	MOAICommandBufferVK& commandBuffer		= gfxMgr.GetCommandBuffer ();
	
	MOAIShaderVK* shader					= MOAICast < MOAIShaderVK >( this->GetShader ());
	MOAITexture2DVK* texture				= MOAICast < MOAITexture2DVK >( this->GetTexture ()); // TODO: obviously, fix this
	MOAIVertexFormatVK* vertexFormat 		= MOAICast < MOAIVertexFormatVK >( this->GetVertexFormat ());

	assert ( shader && texture && vertexFormat );
	
	MOAIGfxStateGPUCacheFrame& activeState	= *this->mActiveState;
	
	MOAIPipelineParamsVK pipelinesParams (
		logicalDevice,
		gfxMgr.GetRenderPass (),
		primTypeVK,
		vertexFormat,
		shader,
		activeState.mBlendEnabled ? &activeState.mBlendMode : NULL
	);
	
	MOAIPipelineVK* pipeline = this->AffirmPipeline ( pipelinesParams );
	commandBuffer.BindPipeline ( VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline );
	commandBuffer.Pin ( *pipeline );

	// compose
	// TODO: need to detect changes and signal gfx state change
	gfxMgr.GfxStateWillChange ();
	MOAIDescriptorSetArrayVK& descriptorSetArray = *shader->GetDescriptorSetArray ();
	descriptorSetArray.Bind ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS );

	// set the viewport and scissor
	VkViewport viewport			= MOAIGfxStructVK::viewport ( activeState.mViewRect, 0.0, 1.0 );
	VkRect2D scissor			= MOAIGfxStructVK::rect2D ( activeState.mScissorRect );

	vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );
	vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

	MOAIVertexBufferVK* vtxBuffer = MOAICast < MOAIVertexBufferVK >( activeState.mVtxBuffer );
	MOAIIndexBufferVK* idxBuffer = MOAICast < MOAIIndexBufferVK >( activeState.mIdxBuffer );

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

