/*
* Vulkan Example - Basic indexed triangle rendering
*
* Note:
*	This is a "pedal to the metal" example to show off how to get Vulkan up an displaying something
*	Contrary to the other examples, this one won't make use of helper functions or initializers
*	Except in a few cases (swap chain setup e.g.)
*
* Copyright (C) 2016-2017 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fstream>
#include <vector>
#include <exception>

#include <vulkan/vulkan.h>
#include "VulkanAbstractExample.h"

// Set to "true" to enable Vulkan's validation layers (see vulkandebug.cpp for details)
#define ENABLE_VALIDATION false
// Set to "true" to use staging buffers for uploading vertex and index data to device local memory
// See "prepareVertices" for details on what's staging and on why to use it
#define USE_STAGING true

//================================================================//
// BufferVK
//================================================================//
class BufferVK {
public:

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
	void init ( VkDevice device, VkPhysicalDeviceMemoryProperties memProps, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memPropFlags ) {
	
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
		memAlloc.memoryTypeIndex = vks::tools::getMemoryTypeIndex ( memReqs.memoryTypeBits, memProps, memPropFlags );
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
// VulkanExample
//================================================================//
class VulkanExample :
    public VulkanAbstractExample {
public:

	struct Vertex {
		float position [ 3 ];
		float color [ 3 ];
		
		Vertex ( float x, float y, float z, float r, float g, float b ) {
		
			position [ 0 ] = x;
			position [ 1 ] = y;
			position [ 2 ] = z;
			
			color [ 0 ] = r;
			color [ 1 ] = g;
			color [ 2 ] = b;
		}
	};

	BufferVK 	mVertices;
	BufferVK 	mIndices;
	
	uint32_t	mTotalIndices;

	struct {
		VkDeviceMemory memory;
		VkBuffer buffer;
		VkDescriptorBufferInfo descriptor;
	}  mUniformBufferVS;

	struct {
		float projectionMatrix [ 16 ];
		float modelMatrix [ 16 ];
		float viewMatrix [ 16 ];
	} mMatrixUniforms;

	VkPipelineLayout		mPipelineLayout;
	VkPipeline				mPipeline;
	VkDescriptorSetLayout	mDescriptorSetLayout;
	VkDescriptorSet			mDescriptorSet;

    //----------------------------------------------------------------//
	void			VulkanAbstractExample_render			();
	void			VulkanAbstractExample_viewChanged		();
	
	//----------------------------------------------------------------//
    void			buildCommandBuffers					();
	void			preparePipelines					();
	void			prepareUniformBuffers				();
    void			prepareVertices						( bool useStagingBuffers );
    void			setupDescriptorPool					();
    void			setupDescriptorSet					();
    void			setupDescriptorSetLayout			();
    void			updateUniformBuffers				();
    				VulkanExample						( VulkanHost& host, bool enableValidation = false, bool useVsync = false, uint32_t apiVersion = VK_API_VERSION_1_0 );
    				~VulkanExample						();
};
