// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAIOneTriVK.h>
#include <moai-gfx-vk/MOAIOneTriNativeVK_spirv.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIOneTriVK
//================================================================//

//----------------------------------------------------------------//
void MOAIOneTriVK::Draw ( VkCommandBuffer& commandBuffer, u32 width, u32 height ) {

	this->UpdateUniformBuffers ( width, height );

	vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &this->mDescriptorSet, 0, nullptr );
	vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline );

	VkDeviceSize offsets [] = { 0 };
	vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &this->mVertices->GetBuffer (), offsets );
	vkCmdBindIndexBuffer ( commandBuffer, this->mIndices->GetBuffer (), 0, VK_INDEX_TYPE_UINT32 );
	vkCmdDrawIndexed ( commandBuffer, this->mTotalIndices, 1, 0, 0, 1 );
}

//----------------------------------------------------------------//
void MOAIOneTriVK::PreparePipeline () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	VkRenderPass& renderPass = gfxMgr.GetRenderPass ();

	VkDynamicState dynamicStateEnables [] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState 	= MOAIGfxStructVK::pipelineInputAssemblyStateCreateInfo ( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE, 0 );
	VkPipelineRasterizationStateCreateInfo rasterizationState 	= MOAIGfxStructVK::pipelineRasterizationStateCreateInfo ();
	VkPipelineColorBlendAttachmentState blendAttachmentState 	= MOAIGfxStructVK::pipelineColorBlendAttachmentState ();
	VkPipelineColorBlendStateCreateInfo colorBlendState 		= MOAIGfxStructVK::pipelineColorBlendStateCreateInfo ( &blendAttachmentState, 1 ); // one blend attachment state per color attachment (even if blending is not used
	VkPipelineViewportStateCreateInfo viewportState 			= MOAIGfxStructVK::pipelineViewportStateCreateInfo ( NULL, 1, NULL, 1 ); // overridden by dynamic state
	VkPipelineDepthStencilStateCreateInfo depthStencilState 	= MOAIGfxStructVK::pipelineDepthStencilStateCreateInfo ();
	VkPipelineMultisampleStateCreateInfo multisampleState 		= MOAIGfxStructVK::pipelineMultisampleStateCreateInfo ();
	VkPipelineDynamicStateCreateInfo dynamicState				= MOAIGfxStructVK::pipelineDynamicStateCreateInfo ( dynamicStateEnables, 2 );

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
		this->mPipelineLayout,
		renderPass
	);

	MOAIVertexFormatVK* vertexFormat = MOAICast < MOAIVertexFormatVK >( gfxMgr.GetVertexFormatPreset ( MOAIVertexFormatPresetEnum::XYZC ));
	vertexFormat->UpdatePipelineCreateInfo ( pipelineCreateInfo );

	this->mShaderProgram = new MOAIShaderProgramVK ();
	this->mShaderProgram->LoadModule ( MOAIShaderProgramVK::VERTEX_MODULE, triangleVertSPIRV, sizeof ( triangleVertSPIRV ));
	this->mShaderProgram->LoadModule ( MOAIShaderProgramVK::FRAGMENT_MODULE, triangleFragSPIRV, sizeof ( triangleFragSPIRV ));
	this->mShaderProgram->UpdatePipelineCreateInfo ( pipelineCreateInfo );

	// Create rendering pipeline using the specified states
	VK_CHECK_RESULT ( vkCreateGraphicsPipelines ( logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->mPipeline ));
}

//----------------------------------------------------------------//
void MOAIOneTriVK::PrepareUniformBuffers () {

	this->mUniforms = new MOAIUniformBufferVK ();

	this->mUniforms->Init ( sizeof ( mMatrixUniforms ));
	this->mUniforms->Bind ();

	// Store information in the uniform's descriptor that is used by the descriptor set
	this->mUniformsDescriptor.buffer = *this->mUniforms;
	this->mUniformsDescriptor.offset = 0;
	this->mUniformsDescriptor.range = sizeof ( mMatrixUniforms );
}

//----------------------------------------------------------------//
// Prepare vertex and index buffers for an indexed triangle
// Also uploads them to device local memory using staging and initializes vertex input and attribute binding to match the vertex shader
void MOAIOneTriVK::PrepareVertices ( bool useStagingBuffers ) {
	
	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAIPhysicalDeviceVK& physicalDevice = gfxMgr.GetPhysicalDevice ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();
	
	this->mVertices		= new MOAIVertexBufferVK ();
	this->mIndices		= new MOAIIndexBufferVK ();
	
	// Setup vertices
	std::vector < Vertex > vertexBuffer;
	vertexBuffer.push_back ( Vertex (  1.0f,  1.0f, 0.0f, 	1.0f, 0.0f, 0.0f ));
	vertexBuffer.push_back ( Vertex ( -1.0f,  1.0f, 0.0f, 	0.0f, 1.0f, 0.0f ));
	vertexBuffer.push_back ( Vertex (  0.0f, -1.0f, 0.0f, 	0.0f, 0.0f, 1.0f ));
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

		MOAIVertexBufferVK stageVerts;
		stageVerts.Init ( vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
		stageVerts.MapAndCopy ( vertexBuffer.data (), vertexBufferSize );
		stageVerts.Bind ();

		this->mVertices->Init ( vertexBufferSize,  VK_BUFFER_USAGE_TRANSFER_DST_BIT, MOAIGfxBufferVK::DEVICE_BUFFER_PROPS );
		this->mVertices->Bind ();
		
		MOAIIndexBufferVK stageIndices;
		stageIndices.Init ( indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT );
		stageIndices.MapAndCopy ( indexBuffer.data (), indexBufferSize );
		stageIndices.Bind ();

		this->mIndices->Init ( indexBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT, MOAIGfxBufferVK::DEVICE_BUFFER_PROPS );
		this->mIndices->Bind ();

		// Buffer copies have to be submitted to a queue, so we need a command buffer for them
		// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
		MOAIQueueVK& graphicsQueue = logicalDevice.GetGraphicsQueue ();
		VkCommandBuffer commandBuffer = graphicsQueue.CreateCommandBuffer ( logicalDevice );

		// Put buffer region copies into command buffer
		VkBufferCopy copyRegion = {};

		// Vertex buffer
		copyRegion.size = vertexBufferSize;
		vkCmdCopyBuffer ( commandBuffer, stageVerts, *this->mVertices, 1, &copyRegion );
		
		// Index buffer
		copyRegion.size = indexBufferSize;
		vkCmdCopyBuffer ( commandBuffer, stageIndices, *this->mIndices, 1, &copyRegion );

		// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
		graphicsQueue.FlushAndFreeCommandBuffer ( logicalDevice, commandBuffer );

		// Destroy staging buffers
		// Note: Staging buffer must not be deleted before the copies have been submitted and executed
		stageVerts.Cleanup ();
		stageIndices.Cleanup ();
	}
	else {
		
		// Don't use staging
		// Create host-visible buffers only and use these for rendering. This is not advised and will usually result in lower rendering performance

		// Vertex buffer
		this->mVertices->Init ( vertexBufferSize );
		this->mVertices->MapAndCopy ( vertexBuffer.data (), vertexBufferSize );
		this->mVertices->Bind ();
		
		this->mIndices->Init ( indexBufferSize );
		this->mIndices->MapAndCopy ( indexBuffer.data (), indexBufferSize );
		this->mIndices->Bind ();
	}
}

//----------------------------------------------------------------//
void MOAIOneTriVK::SetupDescriptorPool () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	// We need to tell the API the number of max. requested descriptors per type
	VkDescriptorPoolSize typeCounts [ 1 ];
	// This example only uses one descriptor type (uniform buffer) and only requests one descriptor of this type
	typeCounts [ 0 ].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	typeCounts [ 0 ].descriptorCount = 1;
	// For additional types you need to add new entries in the type count list
	// E.g. for two combined image samplers :
	// typeCounts [ 1 ].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// typeCounts [ 1 ].descriptorCount = 2;

	// Create the global descriptor pool
	// All descriptors used in this example are allocated from this pool
	VkDescriptorPoolCreateInfo descriptorPoolInfo = MOAIGfxStructVK::descriptorPoolCreateInfo ( typeCounts, 1, 1 );
	VK_CHECK_RESULT ( vkCreateDescriptorPool ( logicalDevice, &descriptorPoolInfo, NULL, &this->mDescriptorPool ));
}

//----------------------------------------------------------------//
void MOAIOneTriVK::SetupDescriptorSet () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	// Allocate a new descriptor set from the global descriptor pool
	VkDescriptorSetAllocateInfo allocInfo = MOAIGfxStructVK::descriptorSetAllocateInfo ( this->mDescriptorPool, &this->mDescriptorSetLayout );
	VK_CHECK_RESULT ( vkAllocateDescriptorSets ( logicalDevice, &allocInfo, &this->mDescriptorSet ));

	// Update the descriptor set determining the shader binding points
	// For every binding point used in a shader there needs to be one
	// descriptor set matching that binding point

	VkWriteDescriptorSet writeDescriptorSet = MOAIGfxStructVK::writeDescriptorSet ( this->mDescriptorSet, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &this->mUniformsDescriptor );

	vkUpdateDescriptorSets ( logicalDevice, 1, &writeDescriptorSet, 0, NULL );
}

//----------------------------------------------------------------//
void MOAIOneTriVK::SetupDescriptorSetLayout () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	// Setup layout of descriptors used in this example
	// Basically connects the different shader stages to descriptors for binding uniform buffers, image samplers, etc.
	// So every shader binding should map to one descriptor set layout binding

	// Binding 0: Uniform buffer (Vertex shader)
	VkDescriptorSetLayoutBinding layoutBinding = MOAIGfxStructVK::descriptorSetLayoutBinding ( 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT );

	VkDescriptorSetLayoutCreateInfo descriptorLayout = MOAIGfxStructVK::descriptorSetLayoutCreateInfo ( &layoutBinding );
	VK_CHECK_RESULT ( vkCreateDescriptorSetLayout ( logicalDevice, &descriptorLayout, NULL, &this->mDescriptorSetLayout ));

	// Create the pipeline layout that is used to generate the rendering pipelines that are based on this descriptor set layout
	// In a more complex scenario you would have different pipeline layouts for different descriptor set layouts that could be reused
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = MOAIGfxStructVK::pipelineLayoutCreateInfo ( &this->mDescriptorSetLayout );
	VK_CHECK_RESULT ( vkCreatePipelineLayout ( logicalDevice, &pPipelineLayoutCreateInfo, NULL, &this->mPipelineLayout ));
}

//----------------------------------------------------------------//
void MOAIOneTriVK::UpdateUniformBuffers ( u32 width, u32 height ) {

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

	this->mUniforms->MapAndCopy ( &this->mMatrixUniforms, sizeof ( this->mMatrixUniforms ));
}

//----------------------------------------------------------------//
MOAIOneTriVK::MOAIOneTriVK () {

	this->PrepareVertices ();
	this->PrepareUniformBuffers ();
	this->SetupDescriptorSetLayout ();
	this->PreparePipeline ();
	this->SetupDescriptorPool ();
	this->SetupDescriptorSet ();
}

//----------------------------------------------------------------//
MOAIOneTriVK::~MOAIOneTriVK () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	vkDestroyPipeline ( logicalDevice, this->mPipeline, NULL );
	vkDestroyPipelineLayout ( logicalDevice, this->mPipelineLayout, NULL );
	vkDestroyDescriptorSetLayout ( logicalDevice, this->mDescriptorSetLayout, NULL );
	vkDestroyDescriptorPool ( logicalDevice, mDescriptorPool, NULL );

	this->mVertices->Cleanup ();
	this->mIndices->Cleanup ();
	this->mUniforms->Cleanup ();
}
