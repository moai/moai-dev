// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAIDynamicOneTriVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIDynamicOneTriVK
//================================================================//

//----------------------------------------------------------------//
MOAIDynamicOneTriVK::MOAIDynamicOneTriVK () {

	MOAIImage image;
	image.Load ( "../resources/moai.png", ZLImageTransform::TRUECOLOR );
	image.Convert ( image );

	this->mTexture = new MOAITexture2DVK ();
	this->mTexture->Init ( image, "" );

	this->PrepareVertices ();
}

//----------------------------------------------------------------//
MOAIDynamicOneTriVK::~MOAIDynamicOneTriVK () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
}

//----------------------------------------------------------------//
// Prepare vertex and index buffers for an indexed triangle
// Also uploads them to device local memory using staging and initializes vertex input and attribute binding to match the vertex shader
void MOAIDynamicOneTriVK::PrepareVertices ( bool useStagingBuffers ) {
	
	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAIPhysicalDeviceVK& physicalDevice = gfxMgr.GetPhysicalDevice ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	
	this->mVertices		= new MOAIGfxBufferSnapshotVK ();
	this->mIndices		= new MOAIGfxBufferSnapshotVK ();
	
	// Setup vertices
	std::vector < Vertex > vertexBuffer;
	vertexBuffer.push_back ( Vertex (  1.0f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f,	1.0, 0.0 ));
	vertexBuffer.push_back ( Vertex ( -1.0f,  1.0f, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0, 0.0 ));
	vertexBuffer.push_back ( Vertex (  0.0f, -1.0f, 0.0f, 	0.0f, 0.0f, 1.0f,	0.5, 1.0 ));
	uint32_t vertexBufferSize = static_cast < uint32_t >( vertexBuffer.size ()) * sizeof ( Vertex );

	// Setup indices
	std::vector < uint32_t > indexBuffer;
	indexBuffer.push_back ( 0 );
	indexBuffer.push_back ( 1 );
	indexBuffer.push_back ( 2 );
	this->mTotalIndices = static_cast < uint32_t >( indexBuffer.size ());
	uint32_t indexBufferSize = this->mTotalIndices * sizeof ( uint32_t );

	if ( useStagingBuffers ) {
		
		// Static data like vertex and index buffer should be stored on the device memory
		// for optimal (and fastest) access by the GPU

		MOAIGfxBufferSnapshotVK stageVerts;
		stageVerts.Initialize ( logicalDevice, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
		stageVerts.MapAndCopy ( vertexBuffer.data (), vertexBufferSize );

		this->mVertices->Initialize ( logicalDevice, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, MOAIGfxBufferSnapshotVK::DEVICE_BUFFER_PROPS );
		
		MOAIGfxBufferSnapshotVK stageIndices;
		stageIndices.Initialize ( logicalDevice, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
		stageIndices.MapAndCopy ( indexBuffer.data (), indexBufferSize );

		this->mIndices->Initialize ( logicalDevice, indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, MOAIGfxBufferSnapshotVK::DEVICE_BUFFER_PROPS );

		// Buffer copies have to be submitted to a queue, so we need a command buffer for them
		// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
		MOAIQueueVK& graphicsQueue = logicalDevice.GetGraphicsQueue ();
		
		MOAICommandBufferVK commandBuffer;
		graphicsQueue.CreateCommandBuffer ( commandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY, true );

		// Put buffer region copies into command buffer
		VkBufferCopy copyRegion = {};

		// Vertex buffer
		copyRegion.size = vertexBufferSize;
		vkCmdCopyBuffer ( commandBuffer, stageVerts, *this->mVertices, 1, &copyRegion );
		
		// Index buffer
		copyRegion.size = indexBufferSize;
		vkCmdCopyBuffer ( commandBuffer, stageIndices, *this->mIndices, 1, &copyRegion );

		// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
		graphicsQueue.FlushAndFreeCommandBuffer ( commandBuffer );
	}
	else {
		
		// Don't use staging
		// Create host-visible buffers only and use these for rendering. This is not advised and will usually result in lower rendering performance

		// Vertex buffer
		this->mVertices->Initialize ( logicalDevice, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT );
		this->mVertices->MapAndCopy ( vertexBuffer.data (), vertexBufferSize );
		
		this->mIndices->Initialize ( logicalDevice, indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT );
		this->mIndices->MapAndCopy ( indexBuffer.data (), indexBufferSize );
	}
}

//----------------------------------------------------------------//
void MOAIDynamicOneTriVK::UpdateMatrices ( u32 width, u32 height ) {

	if ( !this->mUniforms ) {
		MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
		MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	
		this->mUniforms = new MOAIUniformBufferVK ();
		this->mUniforms->Initialize ( logicalDevice, sizeof ( mMatrixUniforms ));
	}

	float aspect		= ( float )width / ( float )height;
	float fovy			= 60.0 * 0.01745329251994329576923690768489; // D2R
	float tanHalfFovy	= tan ( fovy / 2 );

	float xs = 1 / ( aspect * tanHalfFovy );
	float ys = 1 / tanHalfFovy;
	float zn = 0.1;
	float zf = 256;

	ZLMatrix4x4 projectionMatrix;
	projectionMatrix.Perspective ( xs, ys, zn, zf );

	ZLMatrix4x4 viewMatrix;
	viewMatrix.Ident ();

	ZLMatrix4x4 modelMatrix;
	modelMatrix.Translate ( 0, 0, -2.5 );

	memcpy ( mMatrixUniforms.projectionMatrix, &projectionMatrix, sizeof ( projectionMatrix ));
	memcpy ( mMatrixUniforms.viewMatrix, &viewMatrix, sizeof ( viewMatrix ));
	memcpy ( mMatrixUniforms.modelMatrix, &modelMatrix, sizeof ( modelMatrix ));
	
	this->mUniforms->Seek ( 0 );
	this->mUniforms->WriteBytes ( &this->mMatrixUniforms, sizeof ( this->mMatrixUniforms ));
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDynamicOneTriVK::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxMgrVK& gfxMgr					= MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice		= gfxMgr.GetLogicalDevice ();
	MOAICommandBufferVK& commandBuffer		= gfxMgr.GetCommandBuffer ();
	MOAISwapChainVK& swapChain				= gfxMgr.GetSwapChain ();

	VkRect2D rect = swapChain.GetRect ();
	
	u32 width = ( u32 )rect.extent.width;
	u32 height = ( u32 )rect.extent.height;

	this->UpdateMatrices ( width, height );

	VkViewport viewport = MOAIGfxStructVK::viewport (( float )width, ( float )height, 0.0, 1.0 );
	VkRect2D scissor = MOAIGfxStructVK::rect2D ( width, height );

	vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );
	vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

	// get the pipeline layout (should be moved to pipeline object)
	MOAIPipelineLayoutVK* pipelineLayout = gfxMgr.GetShaderPresetVK ( ONETRI_SHADER )->GetProgram ()->GetPipelineLayout ();

	// initialize the descriptor set
	MOAIDescriptorSetLayoutVK& descriptorSetLayout = pipelineLayout->GetDescriptorSetLayout ( ZLIndexOp::ZERO );
	
	MOAIDescriptorSetVK* descriptorSet = new MOAIDescriptorSetVK ();
	descriptorSet->Initialize ( descriptorSetLayout );
	descriptorSet->SetDescriptor ( ZLIndexCast ( 0 ), ZLIndexCast ( 0 ), *this->mUniforms->GetSnapshot ( commandBuffer ));
	descriptorSet->SetDescriptor ( ZLIndexCast ( 1 ), ZLIndexCast ( 0 ), *this->mTexture->GetSnapshot ( commandBuffer ));
	
	commandBuffer.BindDescriptorSet ( VK_PIPELINE_BIND_POINT_GRAPHICS, *descriptorSet->GetSnapshot ( commandBuffer ), *pipelineLayout, 0 );
	
	MOAIPipelineSnapshotVK* pipeline = new MOAIPipelineSnapshotVK ();
	pipeline->Initialize (
		logicalDevice,
		gfxMgr.GetRenderPass (),
		gfxMgr.GetVertexFormatPresetVK ( XYZWUVC ),
		gfxMgr.GetShaderPresetVK ( ONETRI_SHADER )
	);
	commandBuffer.BindPipeline ( VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline );

	commandBuffer.Pin ( *pipeline );

	VkDeviceSize offsets [] = { 0 };
	vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &this->mVertices->GetBuffer (), offsets );
	vkCmdBindIndexBuffer ( commandBuffer, this->mIndices->GetBuffer (), 0, VK_INDEX_TYPE_UINT32 );
	vkCmdDrawIndexed ( commandBuffer, this->mTotalIndices, 1, 0, 0, 1 );
}

//----------------------------------------------------------------//
void MOAIDynamicOneTriVK::MOAIDrawable_DrawDebug ( int subPrimID ) {
}
