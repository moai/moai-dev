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

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <string>
#include <array>
#include <numeric>

#include <vulkan/vulkan.h>

#include "camera.hpp"
//#include "keycodes.hpp"
#include "VulkanTools.h"
#include "VulkanDebug.h"
#include "VkStruct.h"
#include "VulkanSwapChain.hpp"
#include "VulkanHost.h"

//================================================================//
// VulkanExampleBase
//================================================================//
class VulkanExampleBase {
private:	
	
	float           mFPSTimer           = 0.0f; // fps timer (one second interval)
    uint32_t        mDestWidth; // Destination dimensions for resizing the window
    uint32_t        mDestHeight; // Destination dimensions for resizing the window
    bool            mViewUpdated        = false; /** brief Indicates that the view (position, rotation) has changed and buffers containing camera matrices need to be updated */
    bool            mResizing           = false;

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

	uint32_t                            mFrameCounter       = 0;
	uint32_t                            mLastFPS            = 0;
    
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

	float                               mFrameTimer         = 1.0f; // Last frame time measured using a high performance timer (if available)
	VkClearColorValue                   mDefaultClearColor  = {{ 0.025f, 0.025f, 0.025f, 1.0f }};

	float                               mZoom               = 0;
	static std::vector<const char*>     mArgs;

	float                               mTimer              = 0.0f; // Defines a frame rate independent timer value clamped from -1.0...1.0 for use in animations, rotations, etc.
	float                               mTimerSpeed         = 0.25f; // Multiplier for speeding up (or slowing down) the global timer
	bool                                mPaused             = false;
	float                               mRotationSpeed      = 1.0f; // Use to adjust mouse rotation speed
	float                               mZoomSpeed          = 1.0f; // Use to adjust mouse zoom speed

	Camera                              mCamera;

	glm::vec3                           mRotation           = glm::vec3();
	glm::vec3                           mCameraPos          = glm::vec3();

	VulkanHost&                     	mHost;

    //----------------------------------------------------------------//
    virtual void                        buildCommandBuffers                 () = 0;
    void                                createCommandBuffers                (); // Create command buffers for drawing commands
    VkResult                            createLogicalDevice                 ( bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT );
    virtual VkResult                    createInstance                      ( std::string name, uint32_t apiVersion );
    void                                createPipelineCache                 (); // Create a cache pool for rendering pipelines
    void                                createSynchronizationPrimitives     ();
    void                                destroyCommandBuffers               ();
    void                                flushCommandBuffer                  ( VkCommandBuffer commandBuffer, VkQueue queue, bool free );
//    const std::string                   getAssetPath                        (); /** @brief Returns os specific base asset path (for shaders, models, textures) */
    //virtual void                        getEnabledFeatures                  () {}; /** @brief (Virtual) Called after the physical device features have been read, can be used to set features to enable on the device */
    VkPipelineShaderStageCreateInfo     loadShader                          ( std::string fileName, VkShaderStageFlagBits stage ); // Load a SPIR-V shader
    void                                prepareFrame                        ();
    virtual void                        render                              () = 0;
    void                                renderFrame                         (); // Render one frame of a render loop on platforms that sync rendering
    void                                renderLoop                          (); // Start the main render loop
    virtual void                        setupDepthStencil                   (); // Setup default depth and stencil views
    virtual void                        setupFrameBuffer                    ();
    virtual void                        setupRenderPass                     ();
    void                                submitFrame                         (); // Submit the frames' workload
    virtual void                        viewChanged                         () = 0;
                                        VulkanExampleBase                   ( VulkanHost& host, std::string name, bool enableValidation, bool useVsync, uint32_t apiVersion );
    virtual                             ~VulkanExampleBase                  ();
};

#define VULKAN_EXAMPLE_MAIN()
