// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDeckShaderOneTriVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIPipelineSnapshotVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAITextureSnapshot2DVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIDeckShaderOneTriVK
//================================================================//

//----------------------------------------------------------------//
MOAIDeckShaderOneTriVK::MOAIDeckShaderOneTriVK () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAIPhysicalDeviceVK& physicalDevice = gfxMgr.GetPhysicalDevice ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	MOAIImage image;
	image.Load ( "../resources/moai.png", ZLImageTransform::TRUECOLOR );
	image.Convert ( image );

	this->mTexture = new MOAITexture2DVK ();
	this->mTexture->Init ( image, "" );

	this->mVertices = new MOAIVertexBufferVK ();
	this->mVertices->Initialize ( logicalDevice, sizeof ( Vertex ) * 3 );

	this->mVertices->Write < Vertex >( Vertex (  1.0f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f,	1.0, 0.0 ));
	this->mVertices->Write < Vertex >( Vertex ( -1.0f,  1.0f, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0, 0.0 ));
	this->mVertices->Write < Vertex >( Vertex (  0.0f, -1.0f, 0.0f, 	0.0f, 0.0f, 1.0f,	0.5, 1.0 ));

	this->mIndices = new MOAIIndexBufferVK ();
	this->mIndices->Initialize ( logicalDevice, sizeof ( u32 ) * 3 );
	this->mIndices->SetIndexSize ( 4 );

	this->mIndices->Write < u32 >( 0 );
	this->mIndices->Write < u32 >( 1 );
	this->mIndices->Write < u32 >( 2 );
}

//----------------------------------------------------------------//
MOAIDeckShaderOneTriVK::~MOAIDeckShaderOneTriVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDeckShaderOneTriVK::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxMgrVK& gfxMgr					= MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice		= gfxMgr.GetLogicalDevice ();
	MOAICommandBufferVK& commandBuffer		= gfxMgr.GetCommandBuffer ();
	MOAISwapChainVK& swapChain				= gfxMgr.GetSwapChain ();

	VkRect2D rect = swapChain.GetRect ();
	
	u32 width = ( u32 )rect.extent.width;
	u32 height = ( u32 )rect.extent.height;

	VkViewport viewport = MOAIGfxStructVK::viewport ( 0.0, 0.0, ( float )width, ( float )height, 0.0, 1.0 );
	VkRect2D scissor = MOAIGfxStructVK::rect2D ( width, height );

	vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );
	vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

	// get the pipeline layout (should be moved to pipeline object)
	MOAIPipelineLayoutVK& pipelineLayout = gfxMgr.GetShaderPresetVK ( DECK2D_SHADER )->GetProgram ()->GetPipelineLayout ();

	// initialize the descriptor set
	MOAIDescriptorSetLayoutVK& descriptorSetLayout = pipelineLayout.GetDescriptorSetLayout ( 0 );
	
	MOAIDescriptorSetVK* descriptorSet = new MOAIDescriptorSetVK ();
	descriptorSet->Initialize ( descriptorSetLayout );
	descriptorSet->SetDescriptor ( 0, 0, *this->mTexture->GetSnapshot ( commandBuffer ));
	
	commandBuffer.BindDescriptorSet ( VK_PIPELINE_BIND_POINT_GRAPHICS, *descriptorSet->GetSnapshot ( commandBuffer ), pipelineLayout, 0 );
	
	MOAIPipelineSnapshotVK* pipeline = new MOAIPipelineSnapshotVK ();
	pipeline->Initialize (
		logicalDevice,
		gfxMgr.GetRenderPass (),
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		gfxMgr.GetVertexFormatPresetVK ( XYZWUVC ),
		gfxMgr.GetShaderPresetVK ( DECK2D_SHADER )
	);
	commandBuffer.BindPipeline ( VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline );
	commandBuffer.Pin ( *pipeline );

	MOAIGfxBufferSnapshotVK* vertexBuffer = this->mVertices->GetSnapshot ( commandBuffer );
	MOAIGfxBufferSnapshotVK* indexBuffer = this->mIndices->GetSnapshot ( commandBuffer );

	VkDeviceSize offsets [] = { 0 };
	vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &vertexBuffer->GetBuffer (), offsets );
	vkCmdBindIndexBuffer ( commandBuffer, indexBuffer->GetBuffer (), 0, VK_INDEX_TYPE_UINT32 );
	vkCmdDrawIndexed ( commandBuffer, this->mIndices->CountIndices (), 1, 0, 0, 1 );
}

//----------------------------------------------------------------//
void MOAIDeckShaderOneTriVK::MOAIDrawable_DrawDebug ( int subPrimID ) {
}
