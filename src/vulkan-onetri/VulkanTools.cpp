/*
* Assorted commonly used Vulkan helper functions
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include "VulkanTools.h"

namespace vks
{
	namespace tools
	{
		bool errorModeSilent = false;

        //----------------------------------------------------------------//
		std::string errorString(VkResult errorCode) {
			switch (errorCode) {
                #define STR(r) case VK_ ##r: return #r
				STR(NOT_READY);
				STR(TIMEOUT);
				STR(EVENT_SET);
				STR(EVENT_RESET);
				STR(INCOMPLETE);
				STR(ERROR_OUT_OF_HOST_MEMORY);
				STR(ERROR_OUT_OF_DEVICE_MEMORY);
				STR(ERROR_INITIALIZATION_FAILED);
				STR(ERROR_DEVICE_LOST);
				STR(ERROR_MEMORY_MAP_FAILED);
				STR(ERROR_LAYER_NOT_PRESENT);
				STR(ERROR_EXTENSION_NOT_PRESENT);
				STR(ERROR_FEATURE_NOT_PRESENT);
				STR(ERROR_INCOMPATIBLE_DRIVER);
				STR(ERROR_TOO_MANY_OBJECTS);
				STR(ERROR_FORMAT_NOT_SUPPORTED);
				STR(ERROR_SURFACE_LOST_KHR);
				STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
				STR(SUBOPTIMAL_KHR);
				STR(ERROR_OUT_OF_DATE_KHR);
				STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
				STR(ERROR_VALIDATION_FAILED_EXT);
				STR(ERROR_INVALID_SHADER_NV);
                #undef STR
			default:
				return "UNKNOWN_ERROR";
			}
		}

        //----------------------------------------------------------------//
		std::string physicalDeviceTypeString ( VkPhysicalDeviceType type ) {
  
            // TODO: this is awful. WTF?
			switch ( type ) {
                #define STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
				STR ( OTHER );
				STR ( INTEGRATED_GPU );
				STR ( DISCRETE_GPU );
				STR ( VIRTUAL_GPU );
                #undef STR
			default: return "UNKNOWN_DEVICE_TYPE";
			}
		}

        //----------------------------------------------------------------//
		VkBool32 getSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat *depthFormat)
		{
			// Since all depth formats may be optional, we need to find a suitable depth format to use
			// Start with the highest precision packed format
			std::vector<VkFormat> depthFormats;
			depthFormats.push_back ( VK_FORMAT_D32_SFLOAT_S8_UINT );
			depthFormats.push_back ( VK_FORMAT_D32_SFLOAT );
			depthFormats.push_back ( VK_FORMAT_D24_UNORM_S8_UINT );
			depthFormats.push_back ( VK_FORMAT_D16_UNORM_S8_UINT );
			depthFormats.push_back ( VK_FORMAT_D16_UNORM );

			for (auto& format : depthFormats)
			{
				VkFormatProperties formatProps;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
				// Format must support depth stencil attachment for optimal tiling
				if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				{
					*depthFormat = format;
					return true;
				}
			}

			return false;
		}

        //----------------------------------------------------------------//
		// Create an image memory barrier for changing the layout of
		// an image and put it into an active command buffer
		// See chapter 11.4 "Image Layout" for details
		void setImageLayout (
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkImageSubresourceRange subresourceRange,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask)
		{
			// Create an image barrier object
			VkImageMemoryBarrier imageMemoryBarrier = VkStruct::imageMemoryBarrier();
			imageMemoryBarrier.oldLayout = oldImageLayout;
			imageMemoryBarrier.newLayout = newImageLayout;
			imageMemoryBarrier.image = image;
			imageMemoryBarrier.subresourceRange = subresourceRange;

			// Source layouts (old)
			// Source access mask controls actions that have to be finished on the old layout
			// before it will be transitioned to the new layout
			switch (oldImageLayout)
			{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				// Image layout is undefined (or does not matter)
				// Only valid as initial layout
				// No flags required, listed only for completeness
				imageMemoryBarrier.srcAccessMask = 0;
				break;

			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				// Image is preinitialized
				// Only valid as initial layout for linear images, preserves memory contents
				// Make sure host writes have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image is a color attachment
				// Make sure any writes to the color buffer have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image is a depth/stencil attachment
				// Make sure any writes to the depth/stencil buffer have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image is a transfer source 
				// Make sure any reads from the image have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image is a transfer destination
				// Make sure any writes to the image have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image is read by a shader
				// Make sure any shader reads from the image have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;
			default:
				// Other source layouts aren't handled (yet)
				break;
			}

			// Target layouts (new)
			// Destination access mask controls the dependency for the new image layout
			switch (newImageLayout)
			{
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image will be used as a transfer destination
				// Make sure any writes to the image have been finished
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image will be used as a transfer source
				// Make sure any reads from the image have been finished
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image will be used as a color attachment
				// Make sure any writes to the color buffer have been finished
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image layout will be used as a depth/stencil attachment
				// Make sure any writes to depth/stencil buffer have been finished
				imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image will be read in a shader (sampler, input attachment)
				// Make sure any writes to the image have been finished
				if (imageMemoryBarrier.srcAccessMask == 0)
				{
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
				}
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;
			default:
				// Other source layouts aren't handled (yet)
				break;
			}

			// Put barrier inside setup command buffer
			vkCmdPipelineBarrier(
				cmdbuffer,
				srcStageMask,
				dstStageMask,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);
		}

		// Fixed sub resource on first mip level and layer
		void setImageLayout(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageAspectFlags aspectMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask)
		{
			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = aspectMask;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.layerCount = 1;
			setImageLayout(cmdbuffer, image, oldImageLayout, newImageLayout, subresourceRange, srcStageMask, dstStageMask);
		}

		void insertImageMemoryBarrier(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkAccessFlags srcAccessMask,
			VkAccessFlags dstAccessMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask,
			VkImageSubresourceRange subresourceRange)
		{
			VkImageMemoryBarrier imageMemoryBarrier = VkStruct::imageMemoryBarrier();
			imageMemoryBarrier.srcAccessMask = srcAccessMask;
			imageMemoryBarrier.dstAccessMask = dstAccessMask;
			imageMemoryBarrier.oldLayout = oldImageLayout;
			imageMemoryBarrier.newLayout = newImageLayout;
			imageMemoryBarrier.image = image;
			imageMemoryBarrier.subresourceRange = subresourceRange;

			vkCmdPipelineBarrier(
				cmdbuffer,
				srcStageMask,
				dstStageMask,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);
		}

        //----------------------------------------------------------------//
		void exitFatal(std::string message, int32_t exitCode) {
			std::cerr << message << "\n";
			exit(exitCode);
		}

        //----------------------------------------------------------------//
		void exitFatal(std::string message, VkResult resultCode) {
			exitFatal(message, (int32_t)resultCode);
		}

        //----------------------------------------------------------------//
		std::string readTextFile ( std::string fileName ) {
			std::string fileContent;
			std::ifstream fileStream ( fileName, std::ios::in );
			if (!fileStream.is_open ()) {
				printf ( "File %s not found\n", fileName.c_str ());
				return "";
			}
			std::string line = "";
			while ( !fileStream.eof ()) {
				getline ( fileStream, line );
				fileContent.append ( line + "\n" );
			}
			fileStream.close ();
			return fileContent;
		}

        //----------------------------------------------------------------//
        // Create a buffer on the device
        VkResult createBuffer (
            VkDevice device,
            VkBufferUsageFlags usageFlags,
            VkPhysicalDeviceMemoryProperties memoryProperties,
            VkMemoryPropertyFlags memoryPropertyFlags,
            VkDeviceSize size,
            VkBuffer *buffer,
            VkDeviceMemory *memory,
            void *data
        ) {
            // Create the buffer handle
            VkBufferCreateInfo bufferCreateInfo = VkStruct::bufferCreateInfo ( usageFlags, size );
            bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            VK_CHECK_RESULT ( vkCreateBuffer ( device, &bufferCreateInfo, nullptr, buffer ));

            // Create the memory backing up the buffer handle
            VkMemoryRequirements memReqs;
            vkGetBufferMemoryRequirements ( device, *buffer, &memReqs );

            VkMemoryAllocateInfo memAlloc = VkStruct::memoryAllocateInfo ( memReqs.size, vks::tools::getMemoryTypeIndex ( memReqs.memoryTypeBits, memoryProperties, memoryPropertyFlags ));
            VK_CHECK_RESULT ( vkAllocateMemory ( device, &memAlloc, nullptr, memory ));
            
            // If a pointer to the buffer data has been passed, map the buffer and copy over the data
            if (data != nullptr) {
                void *mapped;
                VK_CHECK_RESULT(vkMapMemory(device, *memory, 0, size, 0, &mapped));
                memcpy(mapped, data, size);
                // If host coherency hasn't been requested, do a manual flush to make writes visible
                if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0) {
                    VkMappedMemoryRange mappedRange = VkStruct::mappedMemoryRange();
                    mappedRange.memory = *memory;
                    mappedRange.offset = 0;
                    mappedRange.size = size;
                    vkFlushMappedMemoryRanges(device, 1, &mappedRange);
                }
                vkUnmapMemory(device, *memory);
            }

            // Attach the memory to the buffer object
            VK_CHECK_RESULT(vkBindBufferMemory(device, *buffer, *memory, 0));

            return VK_SUCCESS;
        }

        //----------------------------------------------------------------//
        // Allocate a command buffer from the command pool
        VkCommandBuffer createCommandBuffer ( VkDevice device, VkCommandPool commandPool, VkCommandBufferLevel level, bool begin ) {
        
            VkCommandBufferAllocateInfo cmdBufAllocateInfo = VkStruct::commandBufferAllocateInfo ( commandPool, level, 1 );

            VkCommandBuffer cmdBuffer;
            VK_CHECK_RESULT ( vkAllocateCommandBuffers ( device, &cmdBufAllocateInfo, &cmdBuffer ));

            // If requested, also start recording for the new command buffer
            if ( begin ) {
                VkCommandBufferBeginInfo cmdBufInfo = VkStruct::commandBufferBeginInfo ();
                VK_CHECK_RESULT(vkBeginCommandBuffer ( cmdBuffer, &cmdBufInfo ));
            }
            return cmdBuffer;
        }

        //----------------------------------------------------------------//
        // Create a command pool for allocation command buffers from
        VkCommandPool createCommandPool ( VkDevice device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags ) {
            VkCommandPoolCreateInfo cmdPoolInfo = {};
            cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
            cmdPoolInfo.flags = createFlags;
            VkCommandPool cmdPool;
            VK_CHECK_RESULT ( vkCreateCommandPool ( device, &cmdPoolInfo, nullptr, &cmdPool ));
            return cmdPool;
        }

        //----------------------------------------------------------------//
        // Finish command buffer recording and submit it to a queue
        void flushAndFreeCommandBuffer ( VkDevice device, VkQueue queue, VkCommandBuffer commandBuffer, VkCommandPool commandPool ) {
        
            if ( commandBuffer == VK_NULL_HANDLE ) return;

            VK_CHECK_RESULT ( vkEndCommandBuffer ( commandBuffer ));

            VkSubmitInfo submitInfo = VkStruct::submitInfo ( &commandBuffer, 1 );

            // Create fence to ensure that the command buffer has finished executing
            VkFenceCreateInfo fenceInfo = VkStruct::fenceCreateInfo ( VK_FLAGS_NONE );
            
            VkFence fence;
            VK_CHECK_RESULT ( vkCreateFence ( device, &fenceInfo, nullptr, &fence ));
            
            // Submit to the queue
            VK_CHECK_RESULT ( vkQueueSubmit ( queue, 1, &submitInfo, fence ));
            
            // Wait for the fence to signal that command buffer has finished executing
            VK_CHECK_RESULT ( vkWaitForFences ( device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));

            vkDestroyFence ( device, fence, nullptr );
            
            if ( commandPool ) {
                vkFreeCommandBuffers ( device, commandPool, 1, &commandBuffer );
            }
        }

        //----------------------------------------------------------------//
		bool fileExists(const std::string &filename) {
			std::ifstream f(filename.c_str());
			return !f.fail();
		}

        //----------------------------------------------------------------//
        // This function is used to request a device memory type that supports all the property flags we request (e.g. device local, host visibile)
        // Upon success it will return the index of the memory type that fits our requestes memory properties
        // This is necessary as implementations can offer an arbitrary number of memory types with different
        // memory properties.
        // You can check http://vulkan.gpuinfo.org/ for details on different memory configurations
        uint32_t getMemoryTypeIndex ( uint32_t typeBits, VkPhysicalDeviceMemoryProperties memoryProperties, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound ) {
            for ( uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++ ) {
                if (( typeBits & 1 ) == 1 ) {
                    if ((memoryProperties.memoryTypes [ i ].propertyFlags & properties ) == properties ) {
                        if ( memTypeFound ) {
                            *memTypeFound = true;
                        }
                        return i;
                    }
                }
                typeBits >>= 1;
            }

            if ( memTypeFound ) {
                *memTypeFound = false;
                return 0;
            }
            assert ( false );
            //throw std::runtime_error("Could not find a matching memory type");
        }

        //----------------------------------------------------------------//
        // Get the index of a queue family that supports the requested queue flags
        uint32_t getQueueFamilyIndex ( const std::vector < VkQueueFamilyProperties >& queueFamilyProperties, VkQueueFlagBits queueFlags, uint32_t fallback ) {
        
            // Dedicated queue for compute
            // Try to find a queue family index that supports compute but not graphics
            if ( queueFlags & VK_QUEUE_COMPUTE_BIT ) {
                for ( uint32_t i = 0; i < static_cast < uint32_t >( queueFamilyProperties.size ()); i++ ) {
                    if (( queueFamilyProperties [ i ].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 )) {
                        return i;
                    }
                }
            }

            // Dedicated queue for transfer
            // Try to find a queue family index that supports transfer but not graphics and compute
            if (queueFlags & VK_QUEUE_TRANSFER_BIT) {
                for ( uint32_t i = 0; i < static_cast < uint32_t >( queueFamilyProperties.size ()); i++ ) {
                    if (( queueFamilyProperties [ i ].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
                        return i;
                    }
                }
            }

            // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
            for ( uint32_t i = 0; i < static_cast < uint32_t >( queueFamilyProperties.size ()); i++ ) {
                if ( queueFamilyProperties [ i ].queueFlags & queueFlags) {
                    return i;
                }
            }
            return fallback;
        }

        //----------------------------------------------------------------//
        // Load a GLSL shader (text)
        // Note: GLSL support requires vendor-specific extensions to be enabled and is not a core-feature of Vulkan
        VkShaderModule loadShaderGLSL ( std::string fileName, VkDevice device, VkShaderStageFlagBits stage ) {
            std::string shaderSrc = readTextFile(fileName);
            const char *shaderCode = shaderSrc.c_str();
            size_t size = strlen(shaderCode);
            assert(size > 0);

            VkShaderModule shaderModule;
            VkShaderModuleCreateInfo moduleCreateInfo;
            moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            moduleCreateInfo.pNext = NULL;
            moduleCreateInfo.codeSize = 3 * sizeof(uint32_t) + size + 1;
            moduleCreateInfo.pCode = (uint32_t*)malloc(moduleCreateInfo.codeSize);
            moduleCreateInfo.flags = 0;

            // Magic SPV number
            ((uint32_t *)moduleCreateInfo.pCode)[0] = 0x07230203;
            ((uint32_t *)moduleCreateInfo.pCode)[1] = 0;
            ((uint32_t *)moduleCreateInfo.pCode)[2] = stage;
            memcpy(((uint32_t *)moduleCreateInfo.pCode + 3), shaderCode, size + 1);

            VK_CHECK_RESULT(vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule));

            return shaderModule;
        }
        
        //----------------------------------------------------------------//
        // Vulkan loads it's shaders from an immediate binary representation called SPIR-V
        // Shaders are compiled offline from e.g. GLSL using the reference glslang compiler
        // This function loads such a shader from a binary file and returns a shader module structure
        VkShaderModule loadShaderSPIRV ( std::string filename, VkDevice device ) {

            size_t shaderSize;
            char* shaderCode = NULL;

            std::ifstream is ( filename, std::ios::binary | std::ios::in | std::ios::ate );

            if ( is.is_open ()) {
                shaderSize = is.tellg ();
                is.seekg ( 0, std::ios::beg );
                // Copy file contents into a buffer
                shaderCode = new char [ shaderSize ];
                is.read ( shaderCode, shaderSize );
                is.close ();
                assert ( shaderSize > 0 );
            }

            if ( shaderCode ) {

                // Create a new shader module that will be used for pipeline creation
                VkShaderModuleCreateInfo moduleCreateInfo = VkStruct::shaderModuleCreateInfo (( uint32_t* )shaderCode, shaderSize );

                VkShaderModule shaderModule;
                VK_CHECK_RESULT ( vkCreateShaderModule(device, &moduleCreateInfo, NULL, &shaderModule ));

                delete [] shaderCode;
                return shaderModule;
            }
            else {
                std::cerr << "Error: Could not open shader file \"" << filename << "\"" << std::endl;
                return VK_NULL_HANDLE;
            }
        }
	}
}
