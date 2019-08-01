// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIGfxUtilVK.h>
#include <moai-gfx-vk/MOAIOneTriVK.h>
#include <moai-gfx-vk/MOAIOneTriVK_spirv.h>

//================================================================//
// MOAIOneTriBufferVK
//================================================================//
class MOAIOneTriBufferVK :
	public ZLRefCountedObject {
public:

	static const VkMemoryPropertyFlags STAGING_BUFFER_PROPS = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

	VkBuffer		mBuffer;
	VkDeviceMemory	mMemory;
	VkDeviceSize	mAllocationSize;
	
	//----------------------------------------------------------------//
	operator VkBuffer () {
		return this->mBuffer;
	}
	
	//----------------------------------------------------------------//
	void bind ( VkDevice device ) {
	
		VK_CHECK_RESULT ( vkBindBufferMemory ( device, this->mBuffer, this->mMemory, 0 ));
	}
	
	//----------------------------------------------------------------//
	void cleanup ( VkDevice device ) {
	
		vkDestroyBuffer ( device, this->mBuffer, NULL );
		vkFreeMemory ( device, this->mMemory, NULL );
	}
	
	//----------------------------------------------------------------//
	void init ( VkDevice device, VkPhysicalDeviceMemoryProperties memProps, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memPropFlags = STAGING_BUFFER_PROPS ) {
	
		VkBufferCreateInfo vertexBufferInfo = {};
		vertexBufferInfo.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferInfo.size	= size;
		vertexBufferInfo.usage	= usage;
		VK_CHECK_RESULT ( vkCreateBuffer ( device, &vertexBufferInfo, NULL, &this->mBuffer ));
		
		VkMemoryRequirements memReqs;
		vkGetBufferMemoryRequirements ( device, this->mBuffer, &memReqs );
		
		this->mAllocationSize = memReqs.size;
		
		VkMemoryAllocateInfo memAlloc = {};
		memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = MOAIGfxUtilVK::GetMemoryTypeIndex ( memReqs.memoryTypeBits, memProps, memPropFlags );
		VK_CHECK_RESULT ( vkAllocateMemory ( device, &memAlloc, NULL, &this->mMemory ));
	}
	
	//----------------------------------------------------------------//
	void mapAndCopy ( VkDevice device, const void* data, size_t size ) {
	
		void* mappedAddr;
		VK_CHECK_RESULT ( vkMapMemory ( device, this->mMemory, 0, this->mAllocationSize, 0, &mappedAddr ));
		memcpy ( mappedAddr, data, size );
		vkUnmapMemory ( device, this->mMemory );
	}
};

//================================================================//
// MOAIOneTriVK
//================================================================//

//----------------------------------------------------------------//
void MOAIOneTriVK::Draw ( VkCommandBuffer& commandBuffer, u32 width, u32 height ) {

	this->UpdateUniformBuffers ( width, height );

	vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0, 1, &mDescriptorSet, 0, nullptr );
	vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline );

	VkDeviceSize offsets [] = { 0 };
	vkCmdBindVertexBuffers ( commandBuffer, 0, 1, &this->mVertices->mBuffer, offsets );
	vkCmdBindIndexBuffer ( commandBuffer, this->mIndices->mBuffer, 0, VK_INDEX_TYPE_UINT32 );
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

	VkVertexInputAttributeDescription vertexInputAttributes [] = {
		MOAIGfxStructVK::vertexInputAttributeDescription ( 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof ( Vertex, position )),
		MOAIGfxStructVK::vertexInputAttributeDescription ( 0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof ( Vertex, color )),
	};

	VkPipelineShaderStageCreateInfo shaderStages [] = {
		MOAIGfxStructVK::pipelineShaderStageCreateInfo (
			VK_SHADER_STAGE_VERTEX_BIT,
			MOAIGfxUtilVK::LoadShaderSPIRV ( triangleVertSPIRV, sizeof ( triangleVertSPIRV ), logicalDevice ),
			"main"
		),
		MOAIGfxStructVK::pipelineShaderStageCreateInfo (
			VK_SHADER_STAGE_FRAGMENT_BIT,
			MOAIGfxUtilVK::LoadShaderSPIRV ( triangleFragSPIRV, sizeof ( triangleFragSPIRV ), logicalDevice ),
			"main"
		),
	};

	assert ( shaderStages [ 0 ].module != VK_NULL_HANDLE );
	assert ( shaderStages [ 1 ].module != VK_NULL_HANDLE );

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState 	= MOAIGfxStructVK::pipelineInputAssemblyStateCreateInfo ( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE, 0 );
	VkPipelineRasterizationStateCreateInfo rasterizationState 	= MOAIGfxStructVK::pipelineRasterizationStateCreateInfo ();
	VkPipelineColorBlendAttachmentState blendAttachmentState 	= MOAIGfxStructVK::pipelineColorBlendAttachmentState ();
	VkPipelineColorBlendStateCreateInfo colorBlendState 		= MOAIGfxStructVK::pipelineColorBlendStateCreateInfo ( &blendAttachmentState, 1 ); // one blend attachment state per color attachment (even if blending is not used
	VkPipelineViewportStateCreateInfo viewportState 			= MOAIGfxStructVK::pipelineViewportStateCreateInfo ( NULL, 1, NULL, 1 ); // overridden by dynamic state
	VkPipelineDepthStencilStateCreateInfo depthStencilState 	= MOAIGfxStructVK::pipelineDepthStencilStateCreateInfo ();
	VkPipelineMultisampleStateCreateInfo multisampleState 		= MOAIGfxStructVK::pipelineMultisampleStateCreateInfo ();
	VkVertexInputBindingDescription vertexInputBinding 			= MOAIGfxStructVK::vertexInputBindingDescription ( 0, sizeof ( Vertex ), VK_VERTEX_INPUT_RATE_VERTEX );
	VkPipelineDynamicStateCreateInfo dynamicState				= MOAIGfxStructVK::pipelineDynamicStateCreateInfo ( dynamicStateEnables, 2 );
	VkPipelineVertexInputStateCreateInfo vertexInputState 		= MOAIGfxStructVK::pipelineVertexInputStateCreateInfo ( &vertexInputBinding, 1, vertexInputAttributes, 2 );

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = MOAIGfxStructVK::graphicsPipelineCreateInfo (
		shaderStages,
		2,
		&vertexInputState,
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

	// Create rendering pipeline using the specified states
	VK_CHECK_RESULT ( vkCreateGraphicsPipelines ( logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->mPipeline ));

	// Shader modules are no longer needed once the graphics pipeline has been created
	vkDestroyShaderModule ( logicalDevice, shaderStages [ 0 ].module, nullptr );
	vkDestroyShaderModule ( logicalDevice, shaderStages [ 1 ].module, nullptr );
}

//----------------------------------------------------------------//
void MOAIOneTriVK::PrepareUniformBuffers () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAIPhysicalDeviceVK& physicalDevice = gfxMgr.GetPhysicalDevice ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	this->mUniforms		= new MOAIOneTriBufferVK;

	this->mUniforms->init ( logicalDevice, physicalDevice.mMemoryProperties, sizeof ( mMatrixUniforms ), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT );
	this->mUniforms->bind ( logicalDevice );

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
	
	this->mIndices		= new MOAIOneTriBufferVK;
	this->mVertices		= new MOAIOneTriBufferVK;
	
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
		
//		// Static data like vertex and index buffer should be stored on the device memory
//		// for optimal (and fastest) access by the GPU
//
//		MOAIOneTriBufferVK stageVerts;
//		stageVerts.init ( logicalDevice, physicalDevice.mMemoryProperties, vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
//		stageVerts.mapAndCopy ( logicalDevice, vertexBuffer.data (), vertexBufferSize );
//		stageVerts.bind ( logicalDevice );
//
//		this->mVertices->init ( logicalDevice, physicalDevice.mMemoryProperties, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
//		this->mVertices->bind ( logicalDevice );
//		
//		MOAIOneTriBufferVK stageIndices;
//		stageIndices.init ( logicalDevice, physicalDevice.mMemoryProperties, indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
//		stageIndices.mapAndCopy ( logicalDevice, indexBuffer.data (), indexBufferSize );
//		stageIndices.bind ( logicalDevice );
//
//		this->mIndices->init ( logicalDevice, physicalDevice.mMemoryProperties, indexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
//		this->mIndices->bind ( logicalDevice );
//
//		// Buffer copies have to be submitted to a queue, so we need a command buffer for them
//		// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
//		VkCommandBuffer copyCmd = MOAIGfxUtilVK::createCommandBuffer ( logicalDevice, this->mSwapChainQueueCommandPool );
//
//		// Put buffer region copies into command buffer
//		VkBufferCopy copyRegion = {};
//
//		// Vertex buffer
//		copyRegion.size = vertexBufferSize;
//		vkCmdCopyBuffer ( copyCmd, stageVerts, this->mVertices, 1, &copyRegion );
//		
//		// Index buffer
//		copyRegion.size = indexBufferSize;
//		vkCmdCopyBuffer ( copyCmd, stageIndices, this->mIndices, 1, &copyRegion );
//
//		// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
//		MOAIGfxUtilVK::flushAndFreeCommandBuffer ( logicalDevice, this->mQueue, copyCmd, this->mSwapChainQueueCommandPool );
//
//		// Destroy staging buffers
//		// Note: Staging buffer must not be deleted before the copies have been submitted and executed
//		stageVerts.cleanup ( logicalDevice );
//		stageIndices.cleanup ( logicalDevice );
	}
	else {
		
		// Don't use staging
		// Create host-visible buffers only and use these for rendering. This is not advised and will usually result in lower rendering performance

		// Vertex buffer
		this->mVertices->init ( logicalDevice, physicalDevice.mMemoryProperties, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
		this->mVertices->mapAndCopy ( logicalDevice, vertexBuffer.data (), vertexBufferSize );
		this->mVertices->bind ( logicalDevice );
		
		this->mIndices->init ( logicalDevice, physicalDevice.mMemoryProperties, indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
		this->mIndices->mapAndCopy ( logicalDevice, indexBuffer.data (), indexBufferSize );
		this->mIndices->bind ( logicalDevice );
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
	// typeCounts[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	// typeCounts[1].descriptorCount = 2;

	// Create the global descriptor pool
	// All descriptors used in this example are allocated from this pool
	VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.pNext = NULL;
	descriptorPoolInfo.poolSizeCount = 1;
	descriptorPoolInfo.pPoolSizes = typeCounts;
	// Set the max. number of descriptor sets that can be requested from this pool (requesting beyond this limit will result in an error)
	descriptorPoolInfo.maxSets = 1;

	VK_CHECK_RESULT ( vkCreateDescriptorPool ( logicalDevice, &descriptorPoolInfo, NULL, &this->mDescriptorPool ));
}

//----------------------------------------------------------------//
void MOAIOneTriVK::SetupDescriptorSet () {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	// Allocate a new descriptor set from the global descriptor pool
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = mDescriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &mDescriptorSetLayout;

	VK_CHECK_RESULT(vkAllocateDescriptorSets(logicalDevice, &allocInfo, &mDescriptorSet));

	// Update the descriptor set determining the shader binding points
	// For every binding point used in a shader there needs to be one
	// descriptor set matching that binding point

	VkWriteDescriptorSet writeDescriptorSet = {};

	// Binding 0 : Uniform buffer
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet = mDescriptorSet;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.pBufferInfo = &this->mUniformsDescriptor;
	// Binds this uniform buffer to binding point 0
	writeDescriptorSet.dstBinding = 0;

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
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
	descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorLayout.pNext = NULL;
	descriptorLayout.bindingCount = 1;
	descriptorLayout.pBindings = &layoutBinding;

	VK_CHECK_RESULT ( vkCreateDescriptorSetLayout ( logicalDevice, &descriptorLayout, NULL, &this->mDescriptorSetLayout));

	// Create the pipeline layout that is used to generate the rendering pipelines that are based on this descriptor set layout
	// In a more complex scenario you would have different pipeline layouts for different descriptor set layouts that could be reused
	VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
	pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pPipelineLayoutCreateInfo.pNext = NULL;
	pPipelineLayoutCreateInfo.setLayoutCount = 1;
	pPipelineLayoutCreateInfo.pSetLayouts = &mDescriptorSetLayout;

	VK_CHECK_RESULT ( vkCreatePipelineLayout ( logicalDevice, &pPipelineLayoutCreateInfo, NULL, &this->mPipelineLayout ));
}

//----------------------------------------------------------------//
void MOAIOneTriVK::UpdateUniformBuffers ( u32 width, u32 height ) {

	MOAIGfxMgrVK& gfxMgr = MOAIGfxMgrVK::Get ();
	MOAILogicalDeviceVK& logicalDevice = gfxMgr.GetLogicalDevice ();

	float aspect		= ( float )width / ( float )height;
	float fovy			= 60.0 * 0.01745329251994329576923690768489; // D2R
	float tanHalfFovy	= tan ( fovy / 2 );

	float xs = 1 / ( aspect * tanHalfFovy );
	float ys = 1 / tanHalfFovy;
	float zn = 0.1;
	float zf = 256;

	float projectionMatrix [] = {
		xs, 0, 0, 0,
		0, ys, 0, 0,
		0, 0, zf / ( zn - zf ), -1,
		0, 0, -( zf * zn ) / ( zf - zn ), 0,
	};

	float modelMatrixUnpack [ 16 ];
	memcpy ( modelMatrixUnpack, &projectionMatrix, sizeof ( float [ 16 ]));

	float viewMatrix [] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	float modelMatrix [] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -2.5, 1,
	};

	memcpy ( mMatrixUniforms.projectionMatrix, projectionMatrix, sizeof ( projectionMatrix ));
	memcpy ( mMatrixUniforms.viewMatrix, viewMatrix, sizeof ( viewMatrix ));
	memcpy ( mMatrixUniforms.modelMatrix, modelMatrix, sizeof ( modelMatrix ));

	this->mUniforms->mapAndCopy ( logicalDevice, &this->mMatrixUniforms, sizeof ( this->mMatrixUniforms ));
}

//----------------------------------------------------------------//
MOAIOneTriVK::MOAIOneTriVK () {

	this->PrepareVertices ( false );
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

	this->mVertices->cleanup ( logicalDevice );
	this->mIndices->cleanup ( logicalDevice );
	this->mUniforms->cleanup ( logicalDevice );
}
