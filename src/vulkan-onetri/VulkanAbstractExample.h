/*
* Vulkan Example base class
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#ifdef _WIN32
#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <ShellScalingAPI.h>
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"
#elif defined(_DIRECT2DISPLAY)
//
#elif defined(VK_USE_PLATFORM_XCB_KHR)
#include <xcb/xcb.h>
#endif

#include <iostream>
#include <chrono>
#include <sys/stat.h>

#include <string>
#include <array>
#include <numeric>

#include <vulkan/vulkan.h>

#include "VulkanTools.h"
#include "VulkanDebug.h"
#include "VkStruct.h"
#include "VulkanSwapChain.hpp"
#include "VulkanHost.h"

//================================================================//
// VulkanAbstractExample
//================================================================//
class VulkanAbstractExample {
private:
	
    //----------------------------------------------------------------//
    void            windowResize        (); // Called if the window is resized and some resources have to be recreated
    
protected:

    bool                                mEnableValidation;
    bool                                mUseVsync;

    // Synchronization semaphores
    struct {
        VkSemaphore     presentComplete;    // Swap chain image presentation
        VkSemaphore     renderComplete;     // Command buffer submission and execution
    } semaphores;
	
    std::vector<VkFence>                mWaitFences;
    
    VkInstance                          mInstance; // Vulkan instance, stores all per-application states
    VkPhysicalDevice                    mPhysicalDevice; // Physical device (GPU) that Vulkan will use
	VkPhysicalDeviceProperties          mPhysicalDeviceProperties; // Stores physical device properties (for e.g. checking device limits)
	VkPhysicalDeviceFeatures            mPhysicalDeviceFeature; // Stores the features available on the selected physical device (for e.g. checking if a feature is available)
	VkPhysicalDeviceMemoryProperties    mPhysicalDeviceMemoryProperties; // Stores all available memory (type) properties for the physical device
    VkFormat                            mDepthFormat; // Depth buffer format (selected during Vulkan initialization)

    std::vector<const char*>            mEnabledDeviceExtensions;
    std::vector<const char*>            mEnabledInstanceExtensions;
    VkQueue                             mQueue; // Handle to the device graphics queue that command buffers are submitted to
    VkCommandPool                       mSwapChainQueueCommandPool; // Command buffer pool
    std::vector<VkCommandBuffer>        mDrawCmdBuffers; // Command buffers used for rendering
    VkRenderPass                        mRenderPass; // Global render pass for frame buffer writes
    std::vector<VkFramebuffer>          mFrameBuffers; // List of available frame buffers (same as number of swap chain images)
    uint32_t                            mCurrentBuffer = 0; // Active frame buffer index
    VkDescriptorPool                    mDescriptorPool = VK_NULL_HANDLE; // Descriptor set pool
    std::vector<VkShaderModule>         mShaderModules; // List of shader modules created (stored for cleanup)
    VkPipelineCache                     mPipelineCache; // Pipeline cache object
    VulkanSwapChain                     mSwapChain; // Wraps the swap chain to present images (framebuffers) to the windowing system
	VkPhysicalDeviceFeatures            mEnabledFeatures; // Set of physical device features to be enabled for this example (must be set in the derived constructor). By default no phyiscal device features are enabled

    VkDevice                            mDevice = VK_NULL_HANDLE;                // Logical device representation (application's view of the device)
    VkCommandPool                       mGraphicsCommandPool = VK_NULL_HANDLE;   // Default command pool for the graphics queue family index

public:

    struct {
        VkImage                         mImage;
        VkDeviceMemory                  mMem;
        VkImageView                     mView;
    } mDepthStencil;

	uint32_t                            mWidth              = 1280;
	uint32_t                            mHeight             = 720;

	VkClearColorValue                   mDefaultClearColor  = {{ 0.025f, 0.025f, 0.025f, 1.0f }};

	static std::vector<const char*>     mArgs;

	VulkanHost&                     	mHost;

	//----------------------------------------------------------------//
    virtual void                        VulkanAbstractExample_render			() = 0;
    virtual void                        VulkanAbstractExample_viewChanged		() = 0;

    //----------------------------------------------------------------//
    void                                createCommandBuffers                	(); // Create command buffers for drawing commands
    VkResult                            createLogicalDevice                 	( VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT );
    virtual VkResult                    createInstance                      	( std::string name, uint32_t apiVersion );
    void                                createPipelineCache                 	(); // Create a cache pool for rendering pipelines
    void                                createSynchronizationPrimitives     	();
    void                                destroyCommandBuffers               	();
    void                                flushCommandBuffer                  	( VkCommandBuffer commandBuffer, VkQueue queue, bool free );
    VkPipelineShaderStageCreateInfo     loadShader                          	( std::string fileName, VkShaderStageFlagBits stage ); // Load a SPIR-V shader
    void                                prepareFrame                        	();
    void                                renderFrame                         	(); // Render one frame of a render loop on platforms that sync rendering
    void                                renderLoop                          	(); // Start the main render loop
	void                        		setupDepthStencil                   	(); // Setup default depth and stencil views
	void                        		setupFrameBuffer                    	();
	void                        		setupRenderPass                     	();
    void                                submitFrame                         	(); // Submit the frames' workload
                                        VulkanAbstractExample                   ( VulkanHost& host, std::string name, bool enableValidation, bool useVsync, uint32_t apiVersion );
    virtual                             ~VulkanAbstractExample                  ();
};

#define VULKAN_EXAMPLE_MAIN()
