// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetLayoutVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAIOneTriVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAIGfxBufferSnapshotVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

void transitionImageLayout ( VkCommandBuffer& commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout ) {

	VkImageMemoryBarrier barrier =MOAIGfxStructVK::imageMemoryBarrier ( image, oldLayout, newLayout );
	
	vkCmdPipelineBarrier (
		commandBuffer,
		0, // TODO
		0, // TODO
		0,
		0, NULL,
		0, NULL,
		1, &barrier
	);
}

void copyBufferToImage ( VkCommandBuffer& commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height ) {

	VkBufferImageCopy bufferImageCopy = MOAIGfxStructVK::bufferImageCopy (
		MOAIGfxStructVK::offset3D ( 0, 0, 0 ),
		MOAIGfxStructVK::extent3D ( width, height, 1 )
	);

	vkCmdCopyBufferToImage (
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&bufferImageCopy
	);
}

//================================================================//
// MOAIOneTriVK
//================================================================//

//----------------------------------------------------------------//
MOAIOneTriVK::MOAIOneTriVK () {

	this->PrepareVertices ();
	this->PrepareTexture ();
}

//----------------------------------------------------------------//
MOAIOneTriVK::~MOAIOneTriVK () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	vkDestroyPipeline ( logicalDevice, this->mPipeline, NULL );

	this->mPipelineLayout = NULL;

	vkDestroyImageView ( logicalDevice, this->mTextureImageView, NULL );
    vkDestroyImage ( logicalDevice, this->mTextureImage, NULL) ;
    vkFreeMemory ( logicalDevice, this->mTextureImageMemory, NULL );
}

//----------------------------------------------------------------//
void MOAIOneTriVK::PreparePipeline () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	
	MOAIDescriptorSetLayoutVK* descriptorSetLayout = new MOAIDescriptorSetLayoutVK ();
	descriptorSetLayout->Initialize ( logicalDevice, 2 );
	descriptorSetLayout->SetBinding ( ZLIndexCast ( 0 ), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT );
	descriptorSetLayout->SetBinding ( ZLIndexCast ( 1 ), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT );
	
	// create the pipeline layout
	this->mPipelineLayout = new MOAIPipelineLayoutVK ();
	this->mPipelineLayout->Initialize ( logicalDevice, 1 );
	this->mPipelineLayout->SetDescriptorSetLayout ( ZLIndexOp::ZERO, *descriptorSetLayout );
	this->mPipelineLayout->AffirmPipelineLayout ();
	
	this->mDescriptorSet = new MOAIDescriptorSetVK ();
	this->mDescriptorSet->Initialize ( *descriptorSetLayout );

	VkDynamicState dynamicStateEnables [] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState 	= MOAIGfxStructVK::pipelineInputAssemblyStateCreateInfo ( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST );
	VkPipelineRasterizationStateCreateInfo rasterizationState 	= MOAIGfxStructVK::pipelineRasterizationStateCreateInfo ();
	VkPipelineColorBlendAttachmentState blendAttachmentState 	= MOAIGfxStructVK::pipelineColorBlendAttachmentState ();
	VkPipelineColorBlendStateCreateInfo colorBlendState 		= MOAIGfxStructVK::pipelineColorBlendStateCreateInfo ( &blendAttachmentState, 1 ); // one blend attachment state per color attachment (even if blending is not used)
	VkPipelineViewportStateCreateInfo viewportState 			= MOAIGfxStructVK::pipelineViewportStateCreateInfo (); // overridden by dynamic state
	VkPipelineDepthStencilStateCreateInfo depthStencilState 	= MOAIGfxStructVK::pipelineDepthStencilStateCreateInfo ();
	VkPipelineMultisampleStateCreateInfo multisampleState 		= MOAIGfxStructVK::pipelineMultisampleStateCreateInfo ();
	VkPipelineDynamicStateCreateInfo dynamicState				= MOAIGfxStructVK::pipelineDynamicStateCreateInfo ( dynamicStateEnables, 2 );

	VkRenderPass& renderPass = gfxMgr.GetRenderPass ();

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = MOAIGfxStructVK::graphicsPipelineCreateInfo (
		NULL,
		0,
		NULL,
		&inputAssemblyState,
		NULL,
		&viewportState,
		&rasterizationState,
		&multisampleState,
		&depthStencilState,
		&colorBlendState,
		&dynamicState,
		*this->mPipelineLayout,
		renderPass
	);

	gfxMgr.GetVertexFormatPresetVK ( XYZWUVC )->UpdatePipelineCreateInfo ( pipelineCreateInfo ); // gfx state
	gfxMgr.GetShaderPresetVK ( ONETRI_SHADER )->GetProgram ()->UpdatePipelineCreateInfo ( pipelineCreateInfo ); // gfx state

	// Create rendering pipeline using the specified states
	VK_CHECK_RESULT ( vkCreateGraphicsPipelines ( logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->mPipeline ));
}

//----------------------------------------------------------------//
void MOAIOneTriVK::PrepareTexture () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	MOAIPhysicalDeviceVK& physicalDevice = gfxMgr.GetPhysicalDevice ();

	MOAIImage image;
	image.Load ( "../resources/moai.png", ZLImageTransform::TRUECOLOR );
	image.Convert ( image );

	size_t bitmapSize = image.GetBitmapSize ();
	const void* bitmap = image.GetBitmap ();
	u32 width = image.GetWidth ();
	u32 height = image.GetHeight ();

	MOAIGfxBufferSnapshotVK stageImage;
	stageImage.Initialize ( logicalDevice, bitmapSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
	stageImage.MapAndCopy ( bitmap, bitmapSize );
	
	VkImageCreateInfo imageInfo = MOAIGfxStructVK::imageCreateInfo (
		VK_IMAGE_TYPE_2D,
		VK_FORMAT_R8G8B8A8_UNORM,
		MOAIGfxStructVK::extent3D ( width, height, 1 )
	);
	
	this->mTextureImage;
    VK_CHECK_RESULT ( vkCreateImage ( logicalDevice, &imageInfo, NULL, &this->mTextureImage ));

	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements ( logicalDevice, this->mTextureImage, &memReqs );

	u32 memoryTypeIndex;
	bool didFindMemoryTypeIndex = physicalDevice.FindMemoryTypeIndex ( memoryTypeIndex, memReqs.memoryTypeBits );
	assert ( didFindMemoryTypeIndex );

	VkMemoryAllocateInfo memoryAllocateInfo = MOAIGfxStructVK::memoryAllocateInfo ( memReqs.size, memoryTypeIndex );

	this->mTextureImageMemory;
	VK_CHECK_RESULT ( vkAllocateMemory ( logicalDevice, &memoryAllocateInfo, nullptr, &this->mTextureImageMemory ));
	VK_CHECK_RESULT ( vkBindImageMemory ( logicalDevice, this->mTextureImage, this->mTextureImageMemory, 0 ));
	
	// Buffer copies have to be submitted to a queue, so we need a command buffer for them
	// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
	MOAIQueueVK& graphicsQueue = logicalDevice.GetGraphicsQueue ();
	
	MOAICommandBufferVK commandBuffer;
	graphicsQueue.CreateCommandBuffer ( commandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY, true );

	transitionImageLayout ( commandBuffer, this->mTextureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );
	copyBufferToImage ( commandBuffer, stageImage, this->mTextureImage, width, height );
	transitionImageLayout ( commandBuffer, this->mTextureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
	graphicsQueue.FlushAndFreeCommandBuffer ( commandBuffer );
	
	VkSamplerCreateInfo samplerInfo = MOAIGfxStructVK::samplerCreateInfo ();
	VK_CHECK_RESULT ( vkCreateSampler ( logicalDevice, &samplerInfo, nullptr, &this->mTextureSampler ));
	
	VkImageViewCreateInfo viewInfo = MOAIGfxStructVK::imageViewCreateInfo (
		this->mTextureImage,
		VK_IMAGE_VIEW_TYPE_2D,
		VK_FORMAT_R8G8B8A8_UNORM
	);
	VK_CHECK_RESULT ( vkCreateImageView ( logicalDevice, &viewInfo, nullptr, &this->mTextureImageView ));
	
	this->mTextureDescriptor = MOAIGfxStructVK::descriptorImageInfo ( this->mTextureSampler, this->mTextureImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
}

//----------------------------------------------------------------//
// Prepare vertex and index buffers for an indexed triangle
// Also uploads them to device local memory using staging and initializes vertex input and attribute binding to match the vertex shader
void MOAIOneTriVK::PrepareVertices ( bool useStagingBuffers ) {
	
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
void MOAIOneTriVK::UpdateMatrices ( u32 width, u32 height ) {

	if ( !this->mUniforms ) {
		MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
		MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	
		this->mUniforms = new MOAIUniformBufferVK ();
		this->mUniforms->Initialize ( sizeof ( mMatrixUniforms ));
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
void MOAIOneTriVK::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	this->PreparePipeline ();

	MOAIGfxMgrVK& gfxMgr					= MOAIGfxMgrVK::Get ();
	MOAICommandBufferVK& commandBuffer		= gfxMgr.GetCommandBuffer ();
	MOAISwapChainVK& swapChain				= gfxMgr.GetSwapChain ();
	MOAILogicalDeviceVK& logicalDevice		= gfxMgr.GetLogicalDevice ();

	VkRect2D rect = swapChain.GetRect ();
	
	u32 width = ( u32 )rect.extent.width;
	u32 height = ( u32 )rect.extent.height;

	VkViewport viewport = MOAIGfxStructVK::viewport ( 0.0, 0.0, ( float )width, ( float )height, 0.0, 1.0 );
	VkRect2D scissor = MOAIGfxStructVK::rect2D ( width, height );

	vkCmdSetViewport ( commandBuffer, 0, 1, &viewport );
	vkCmdSetScissor ( commandBuffer, 0, 1, &scissor );

	this->UpdateMatrices ( width, height );
	MOAIGfxBufferSnapshotVK* uniforms = new MOAIGfxBufferSnapshotVK ();
	uniforms->Initialize ( logicalDevice, *this->mUniforms );

	// initialize the descriptor set
	MOAIDescriptorSetLayoutVK& descriptorSetLayout = this->mPipelineLayout->GetDescriptorSetLayout ( ZLIndexOp::ZERO );
	
	MOAIDescriptorSetVK* descriptorSetSignature = new MOAIDescriptorSetVK ();
	descriptorSetSignature->Initialize ( descriptorSetLayout );
	descriptorSetSignature->SetDescriptor ( ZLIndexCast ( 0 ), ZLIndexCast ( 0 ), *uniforms );
	descriptorSetSignature->SetDescriptor ( ZLIndexCast ( 1 ), ZLIndexCast ( 0 ), &this->mTextureDescriptor );
	
	MOAIDescriptorSetSnapshotVK* descriptorSet = descriptorSetLayout.ProcureDescriptorSet ( *descriptorSetSignature );
	
	commandBuffer.BindDescriptorSet ( VK_PIPELINE_BIND_POINT_GRAPHICS, *descriptorSet, *this->mPipelineLayout, 0 );
	
	vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->mPipeline );

	VkDeviceSize offsets [] = { 0 };
	vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &this->mVertices->GetBuffer (), offsets );
	vkCmdBindIndexBuffer ( commandBuffer, this->mIndices->GetBuffer (), 0, VK_INDEX_TYPE_UINT32 );
	vkCmdDrawIndexed ( commandBuffer, this->mTotalIndices, 1, 0, 0, 1 );
	
	// snapshot:
	// - uniform buffers
	// - descriptor sets
	// - index buffers
	// - vertex buffers
	// - pipeline layouts
	// - pipelines
	// basically: everything
	// seems like if a resource *can* be mutated, it needs to be copy-on-write
	// moai gfx state changes uniform and shader bindings on draw
	// not all uniform buffers are compose-on-draw
	
	// snapshots should be cached and re-used, even if they need their data updated
	// snapshots contain the actual VK resource
	// snapshots may keep a mem copy for comparison (to check if dirty)
}

//----------------------------------------------------------------//
void MOAIOneTriVK::MOAIDrawable_DrawDebug ( int subPrimID ) {
}
