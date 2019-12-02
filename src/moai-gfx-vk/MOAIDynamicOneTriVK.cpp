// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetSnapshotVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIDynamicOneTriVK.h>
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
// MOAIDynamicOneTriVK
//================================================================//

//----------------------------------------------------------------//
MOAIDynamicOneTriVK::MOAIDynamicOneTriVK () {

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
MOAIDynamicOneTriVK::~MOAIDynamicOneTriVK () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
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

	MOAIGfxBufferSnapshotVK* vertexBuffer = this->mVertices->GetSnapshot ( commandBuffer );
	MOAIGfxBufferSnapshotVK* indexBuffer = this->mIndices->GetSnapshot ( commandBuffer );

	VkDeviceSize offsets [] = { 0 };
	vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &vertexBuffer->GetBuffer (), offsets );
	vkCmdBindIndexBuffer ( commandBuffer, indexBuffer->GetBuffer (), 0, VK_INDEX_TYPE_UINT32 );
	vkCmdDrawIndexed ( commandBuffer, this->mIndices->CountIndices (), 1, 0, 0, 1 );
}

//----------------------------------------------------------------//
void MOAIDynamicOneTriVK::MOAIDrawable_DrawDebug ( int subPrimID ) {
}
