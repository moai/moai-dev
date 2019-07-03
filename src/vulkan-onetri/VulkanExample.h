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
// VulkanExample
//================================================================//
class VulkanExample :
    public VulkanAbstractExample {
public:

	struct Vertex {
		float position[3];
		float color[3];
		
		Vertex ( float x, float y, float z, float r, float g, float b ) {
		
			position [ 0 ] = x;
			position [ 1 ] = y;
			position [ 2 ] = z;
			
			color [ 0 ] = r;
			color [ 1 ] = g;
			color [ 2 ] = b;
		}
	};

	struct {
		VkDeviceMemory memory;
		VkBuffer buffer;
	} mVertices;

	struct {
		VkDeviceMemory memory;
		VkBuffer buffer;
		uint32_t count;
	} mIndices;

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
