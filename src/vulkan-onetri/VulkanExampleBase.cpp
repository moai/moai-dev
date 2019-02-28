/*
* Vulkan Example base class
*
* Copyright (C) 2016-2017 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#include <set>
#include "VulkanExampleBase.h"

//================================================================//
// VulkanExampleBase
//================================================================//

std::vector<const char*> VulkanExampleBase::mArgs;

//----------------------------------------------------------------//
void VulkanExampleBase::createCommandBuffers ()
{
    // Create one command buffer for each swap chain image and reuse for rendering
    mDrawCmdBuffers.resize ( mSwapChain.imageCount );
    VkCommandBufferAllocateInfo cmdBufAllocateInfo = VkStruct::commandBufferAllocateInfo ( mSwapChainQueueCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, static_cast < uint32_t >( mDrawCmdBuffers.size ()));
    VK_CHECK_RESULT ( vkAllocateCommandBuffers ( mDevice, &cmdBufAllocateInfo, mDrawCmdBuffers.data ()));
}

//----------------------------------------------------------------//
VkResult VulkanExampleBase::createInstance ( std::string name, uint32_t apiVersion ) {

    printf ( "VK_LAYER_PATH: %s\n", getenv ( "VK_LAYER_PATH" ));
    printf ( "VK_ICD_FILENAMES: %s\n", getenv ( "VK_ICD_FILENAMES" ));

    uint32_t instance_layer_count;
    VK_CHECK_RESULT ( vkEnumerateInstanceLayerProperties ( &instance_layer_count, nullptr ));
    std::cout << instance_layer_count << " layers found!\n";
    if ( instance_layer_count > 0 ) {
        std::unique_ptr<VkLayerProperties []> instance_layers ( new VkLayerProperties [ instance_layer_count ]);
        VK_CHECK_RESULT ( vkEnumerateInstanceLayerProperties ( &instance_layer_count, instance_layers.get ()));
        for ( uint32_t i = 0; i < instance_layer_count; ++i ) {
            std::cout << instance_layers [ i ].layerName << "\n";
        }
    }

    VkApplicationInfo appInfo = VkStruct::applicationInfo ( name.c_str (), VK_MAKE_VERSION ( 0, 0, 0 ), name.c_str(), VK_MAKE_VERSION ( 0, 0, 0 ), apiVersion );

    // need to build an array of c strings for VkInstanceCreateInfo. can't be an array of stl strings.
    std::vector < const char* > instanceExtensions;
    std::vector < const char* > validationLayers;
	
	this->mHost.pushInstanceExtensions ( instanceExtensions );
	
//    instanceExtensions.push_back ( VK_KHR_SURFACE_EXTENSION_NAME );

//    #if defined ( VK_USE_PLATFORM_IOS_MVK )
//        instanceExtensions.push_back ( VK_MVK_IOS_SURFACE_EXTENSION_NAME );
//    #elif defined ( VK_USE_PLATFORM_MACOS_MVK )
//        instanceExtensions.push_back ( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
//    #endif

    for ( size_t i = 0; i < this->mEnabledInstanceExtensions.size (); ++i ) {
        instanceExtensions.push_back ( this->mEnabledInstanceExtensions [ i ]);
    }

    if ( this->mEnableValidation ) {
        instanceExtensions.push_back ( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
        validationLayers.push_back ( "VK_LAYER_LUNARG_standard_validation" );
    }

    VkInstanceCreateInfo instanceCreateInfo = VkStruct::instanceCreateInfo (
        &appInfo,
        instanceExtensions.data (),
        ( uint32_t )instanceExtensions.size (),
        validationLayers.data (),
        ( uint32_t )validationLayers.size ()
     );
    
    VK_CHECK_RESULT ( vkCreateInstance ( &instanceCreateInfo, nullptr, &mInstance ));
    
    if ( this->mEnableValidation ) {
        VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        vks::debug::setupDebugging ( this->mInstance, debugReportFlags, VK_NULL_HANDLE );
    }
    return VK_SUCCESS;
}

//----------------------------------------------------------------//
// Create the logical device based on the assigned physical device, also gets default queue family indices
VkResult VulkanExampleBase::createLogicalDevice ( bool useSwapChain, VkQueueFlags requestedQueueTypes ) {

    assert ( this->mPhysicalDevice );

    std::set < std::string > supportedExtensions;

//    // Get list of supported extensions
//    uint32_t extCount = 0;
//    vkEnumerateDeviceExtensionProperties ( mPhysicalDevice, nullptr, &extCount, nullptr );
//    if ( extCount > 0 ) {
//        std::vector < VkExtensionProperties > extensions ( extCount );
//        if ( vkEnumerateDeviceExtensionProperties ( mPhysicalDevice, nullptr, &extCount, &extensions.front ()) == VK_SUCCESS ) {
//            for (auto ext : extensions) {
//                supportedExtensions.insert ( ext.extensionName );
//                printf ( "%s\n", ext.extensionName );
//            }
//        }
//    }

    // Desired queues need to be requested upon logical device creation
    // Due to differing queue family configurations of Vulkan implementations this can be a bit tricky, especially if the application
    // requests different queue types

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    // Get queue family indices for the requested queue family types
    // Note that the indices may overlap depending on the implementation

    // Queue family properties, used for setting up requested queues upon device creation
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties ( mPhysicalDevice, &queueFamilyCount, nullptr );
    assert ( queueFamilyCount > 0 );

    std::vector < VkQueueFamilyProperties > queueFamilyProperties;
    queueFamilyProperties.resize ( queueFamilyCount );
    vkGetPhysicalDeviceQueueFamilyProperties ( mPhysicalDevice, &queueFamilyCount, queueFamilyProperties.data ());

    uint32_t graphicsQueueFamilyIndex = 0;
    uint32_t computeQueueFamilyIndex = 0;
    uint32_t transferQueueFamilyIndex = 0;

    // Graphics queue
    if ( requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT ) {
        graphicsQueueFamilyIndex = vks::tools::getQueueFamilyIndex ( queueFamilyProperties, VK_QUEUE_GRAPHICS_BIT, 0 );
        queueCreateInfos.push_back ( VkStruct::deviceQueueCreateInfo ( graphicsQueueFamilyIndex ));
    }

    // Dedicated compute queue
    if ( requestedQueueTypes & VK_QUEUE_COMPUTE_BIT ) {
        computeQueueFamilyIndex = vks::tools::getQueueFamilyIndex ( queueFamilyProperties, VK_QUEUE_COMPUTE_BIT, graphicsQueueFamilyIndex );
        if ( computeQueueFamilyIndex != graphicsQueueFamilyIndex ) {
            queueCreateInfos.push_back ( VkStruct::deviceQueueCreateInfo ( computeQueueFamilyIndex ));
        }
    }

    // Dedicated transfer queue
    if ( requestedQueueTypes & VK_QUEUE_TRANSFER_BIT ) {
        transferQueueFamilyIndex = vks::tools::getQueueFamilyIndex ( queueFamilyProperties, VK_QUEUE_TRANSFER_BIT, graphicsQueueFamilyIndex );
        if (( transferQueueFamilyIndex != graphicsQueueFamilyIndex ) && ( transferQueueFamilyIndex != computeQueueFamilyIndex )) {
            queueCreateInfos.push_back ( VkStruct::deviceQueueCreateInfo ( transferQueueFamilyIndex ));
        }
    }

    // Create the logical device representation
	
//    std::vector<const char*> deviceExtensions ( mEnabledDeviceExtensions );
//    if ( useSwapChain ) {
//        // If the device will be used for presenting to a display via a swapchain we need to request the swapchain extension
//        deviceExtensions.push_back ( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
//    }

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast < uint32_t>( queueCreateInfos.size ());;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data ();
    deviceCreateInfo.pEnabledFeatures = &mEnabledFeatures;

	std::vector < const char* > deviceExtensions;
	this->mHost.pushDeviceExtensions ( deviceExtensions );

//    // Enable the debug marker extension if it is present (likely meaning a debugging tool is present)
//    if ( supportedExtensions.find ( VK_EXT_DEBUG_MARKER_EXTENSION_NAME ) != supportedExtensions.end ()) {
//        deviceExtensions.push_back ( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
//        vks::debugmarker::setup ( mDevice );
//    }

    if (deviceExtensions.size () > 0) {
        deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }

    VK_CHECK_RESULT ( vkCreateDevice ( mPhysicalDevice, &deviceCreateInfo, nullptr, &mDevice ));

    // Create a default command pool for graphics command buffers
    mGraphicsCommandPool = vks::tools::createCommandPool ( mDevice, graphicsQueueFamilyIndex );

    // Get a graphics queue from the device
    vkGetDeviceQueue ( mDevice, graphicsQueueFamilyIndex, 0, &mQueue );

    return VK_SUCCESS;
}

//----------------------------------------------------------------//
// Destroy all command buffers and set their handles to VK_NULL_HANDLE
// May be necessary during runtime if options are toggled
void VulkanExampleBase::destroyCommandBuffers () {
    vkFreeCommandBuffers(mDevice, mSwapChainQueueCommandPool, static_cast<uint32_t>(mDrawCmdBuffers.size()), mDrawCmdBuffers.data());
}

//----------------------------------------------------------------//
// End the command buffer, submit it to the queue and free (if requested)
// Note : Waits for the queue to become idle
void VulkanExampleBase::flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free)
{
	if (commandBuffer == VK_NULL_HANDLE)
	{
		return;
	}
	
	VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

	VkSubmitInfo submitInfo = VkStruct::submitInfo ( &commandBuffer, 1 );
	VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
	VK_CHECK_RESULT(vkQueueWaitIdle(queue));

	if (free)
	{
		vkFreeCommandBuffers(mDevice, mSwapChainQueueCommandPool, 1, &commandBuffer);
	}
}

//----------------------------------------------------------------//
VkPipelineShaderStageCreateInfo VulkanExampleBase::loadShader ( std::string fileName, VkShaderStageFlagBits stage ) {
    VkShaderModule module = vks::tools::loadShaderSPIRV ( fileName.c_str (), mDevice );
    assert ( module != VK_NULL_HANDLE );
    VkPipelineShaderStageCreateInfo shaderStage = VkStruct::pipelineShaderStageCreateInfo ( stage, module, "main" );
    mShaderModules.push_back ( module );
    return shaderStage;
}

//----------------------------------------------------------------//
void VulkanExampleBase::prepareFrame () {
    VkResult err = mSwapChain.acquireNextImage ( semaphores.presentComplete, &mCurrentBuffer );
    if (( err == VK_ERROR_OUT_OF_DATE_KHR ) || ( err == VK_SUBOPTIMAL_KHR )) {
        windowResize ();
    }
    else {
        VK_CHECK_RESULT ( err );
    }
}

//----------------------------------------------------------------//
void VulkanExampleBase::renderFrame()
{
	auto tStart = std::chrono::high_resolution_clock::now();
	if (mViewUpdated)
	{
		mViewUpdated = false;
		viewChanged();
	}

	render();
	mFrameCounter++;
	auto tEnd = std::chrono::high_resolution_clock::now();
	auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
	mFrameTimer = (float)tDiff / 1000.0f;
	mCamera.update(mFrameTimer);
	if (mCamera.moving())
	{
		mViewUpdated = true;
	}
	// Convert to clamped timer value
	if (!mPaused)
	{
		mTimer += mTimerSpeed * mFrameTimer;
		if (mTimer > 1.0)
		{
			mTimer -= 1.0f;
		}
	}
	mFPSTimer += (float)tDiff;
	if (mFPSTimer > 1000.0f)
	{
		mLastFPS = static_cast<uint32_t>((float)mFrameCounter * (1000.0f / mFPSTimer));
		mFPSTimer = 0.0f;
		mFrameCounter = 0;
	}
}

//----------------------------------------------------------------//
void VulkanExampleBase::renderLoop() {

	mDestWidth = mWidth;
	mDestHeight = mHeight;

	// Flush device to make sure all resources can be freed
	if (mDevice != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(mDevice);
	}
}

//----------------------------------------------------------------//
void VulkanExampleBase::setupDepthStencil()
{
    VkImageCreateInfo image = VkStruct::imageCreateInfo (
        VK_IMAGE_TYPE_2D,
        mDepthFormat,
        VkStruct::extent3D ( mWidth, mHeight, 1 ),
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
    );

    VkImageViewCreateInfo depthStencilView = VkStruct::imageViewCreateInfo (
        VK_IMAGE_VIEW_TYPE_2D,
        NULL,
        mDepthFormat,
        VkStruct::componentMapping (),
        VkStruct::imageSubresourceRange ( VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT )
    );
    VK_CHECK_RESULT ( vkCreateImage ( mDevice, &image, nullptr, &mDepthStencil.mImage ));
    
    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements ( mDevice, mDepthStencil.mImage, &memReqs );
    
    VkMemoryAllocateInfo mem_alloc = VkStruct::memoryAllocateInfo ( memReqs.size, vks::tools::getMemoryTypeIndex ( memReqs.memoryTypeBits, this->mPhysicalDeviceMemoryProperties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ));
    VK_CHECK_RESULT ( vkAllocateMemory ( mDevice, &mem_alloc, nullptr, &mDepthStencil.mMem ));
    VK_CHECK_RESULT ( vkBindImageMemory ( mDevice, mDepthStencil.mImage, mDepthStencil.mMem, 0 ));

    depthStencilView.image = mDepthStencil.mImage;
    VK_CHECK_RESULT ( vkCreateImageView ( mDevice, &depthStencilView, nullptr, &mDepthStencil.mView ));
}

//----------------------------------------------------------------//
// Create framebuffers for all requested swap chain images
// Can be overriden in derived class to setup a custom framebuffer (e.g. for MSAA)
void VulkanExampleBase::setupFrameBuffer()
{
    VkImageView attachments [ 2 ];

    // Depth/Stencil attachment is the same for all frame buffers
    attachments [ 1 ] = mDepthStencil.mView;

    VkFramebufferCreateInfo framebufferCreateInfo = VkStruct::framebufferCreateInfo ( mRenderPass, attachments, 2, mWidth, mHeight );

    // Create frame buffers for every swap chain image
    mFrameBuffers.resize ( mSwapChain.imageCount );
    for ( uint32_t i = 0; i < mFrameBuffers.size(); i++ ) {
        attachments [ 0 ] = mSwapChain.buffers [ i ].view;
        VK_CHECK_RESULT ( vkCreateFramebuffer ( mDevice, &framebufferCreateInfo, nullptr, &mFrameBuffers [ i ]));
    }
}

//----------------------------------------------------------------//
// Setup a default render pass
// Can be overriden in derived class to setup a custom render pass (e.g. for MSAA)
void VulkanExampleBase::setupRenderPass()
{
    std::array < VkAttachmentDescription, 2 > attachments = {};
    
    // Color attachment
    attachments [ 0 ] = VkStruct::attachmentDescription (
        mSwapChain.colorFormat,
        VK_SAMPLE_COUNT_1_BIT,
        VK_ATTACHMENT_LOAD_OP_CLEAR,
        VK_ATTACHMENT_STORE_OP_STORE,
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        VK_ATTACHMENT_STORE_OP_DONT_CARE,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    );

    // Depth attachment
    attachments [ 1 ] = VkStruct::attachmentDescription (
        mDepthFormat,
        VK_SAMPLE_COUNT_1_BIT,
        VK_ATTACHMENT_LOAD_OP_CLEAR,
        VK_ATTACHMENT_STORE_OP_STORE,
        VK_ATTACHMENT_LOAD_OP_CLEAR,
        VK_ATTACHMENT_STORE_OP_DONT_CARE,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    );

    VkAttachmentReference colorReference = VkStruct::attachmentReference ( 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
    VkAttachmentReference depthReference = VkStruct::attachmentReference ( 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );

    VkSubpassDescription subpassDescription = VkStruct::subpassDescription ( VK_PIPELINE_BIND_POINT_GRAPHICS, &colorReference, 1, &depthReference );

    // Subpass dependencies for layout transitions
    std::array < VkSubpassDependency, 2 > dependencies;

    dependencies [ 0 ] = VkStruct::subpassDependency (
        VK_SUBPASS_EXTERNAL,
        0,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_DEPENDENCY_BY_REGION_BIT
    );

    dependencies [ 1 ] = VkStruct::subpassDependency (
        0,
        VK_SUBPASS_EXTERNAL,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_ACCESS_MEMORY_READ_BIT,
        VK_DEPENDENCY_BY_REGION_BIT
    );

    VkRenderPassCreateInfo renderPassInfo = VkStruct::renderPassCreateInfo (
        attachments.data (),
        static_cast<  uint32_t >( attachments.size ()),
        &subpassDescription,
        1,
        dependencies.data (),
        static_cast < uint32_t >( dependencies.size ())
    );

    VK_CHECK_RESULT ( vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass ));
}

//----------------------------------------------------------------//
void VulkanExampleBase::submitFrame () {

    VkResult result = mSwapChain.queuePresent ( mQueue, mCurrentBuffer, semaphores.renderComplete );
    if (!((result == VK_SUCCESS) || (result == VK_SUBOPTIMAL_KHR))) {
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            // Swap chain is no longer compatible with the surface and needs to be recreated
            windowResize();
            return;
        } else {
            VK_CHECK_RESULT(result);
        }
    }
    VK_CHECK_RESULT(vkQueueWaitIdle(mQueue));
}

//----------------------------------------------------------------//
VulkanExampleBase::VulkanExampleBase ( VulkanHost& host, std::string name, bool enableValidation, bool useVsync, uint32_t apiVersion ) :
    mHost ( host ),
    mEnableValidation ( enableValidation ),
    mUseVsync ( useVsync ) {

    // Check for a valid asset path
    struct stat info;
    if ( stat ( this->mHost.getAssetPath ().c_str (), &info ) != 0 ) {
        std::cerr << "Error: Could not find asset path in " << this->mHost.getAssetPath () << std::endl;
        exit ( -1 );
    }
    
    VK_CHECK_RESULT ( createInstance ( name, apiVersion ));

    uint32_t gpuCount = 0;
    VK_CHECK_RESULT ( vkEnumeratePhysicalDevices ( mInstance, &gpuCount, nullptr ));
    assert ( gpuCount > 0 );
    
    std::vector < VkPhysicalDevice > physicalDevices ( gpuCount );
    VK_CHECK_RESULT ( vkEnumeratePhysicalDevices ( mInstance, &gpuCount, physicalDevices.data ()));

    mPhysicalDevice = physicalDevices [ 0 ];
    vkGetPhysicalDeviceProperties ( mPhysicalDevice, &mPhysicalDeviceProperties );
    vkGetPhysicalDeviceFeatures ( mPhysicalDevice, &mPhysicalDeviceFeature );
    vkGetPhysicalDeviceMemoryProperties ( mPhysicalDevice, &mPhysicalDeviceMemoryProperties );

    // Find a suitable depth format
    VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat ( mPhysicalDevice, &mDepthFormat );
    assert ( validDepthFormat );

    // Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
    //getEnabledFeatures();

    VK_CHECK_RESULT ( this->createLogicalDevice ());

    // create pipeline cache
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = VkStruct::pipelineCacheCreateInfo ();
    VK_CHECK_RESULT ( vkCreatePipelineCache ( mDevice, &pipelineCacheCreateInfo, nullptr, &mPipelineCache ));

    // init swapchain
    mSwapChain.connect ( mInstance, mPhysicalDevice, mDevice ) ;
    mSwapChain.initSurface ( this->mHost );
    mSwapChainQueueCommandPool = vks::tools::createCommandPool ( mDevice, mSwapChain.queueNodeIndex );
    mSwapChain.create ( &mWidth, &mHeight, this->mUseVsync );

    // gotta do after creating the swap chain
    setupRenderPass ();

    createCommandBuffers ();
    setupDepthStencil ();
    setupFrameBuffer ();

    // create synchronization primitives
    // Wait fences to sync command buffer access
    VkFenceCreateInfo fenceCreateInfo = VkStruct::fenceCreateInfo ( VK_FENCE_CREATE_SIGNALED_BIT );
    mWaitFences.resize ( mDrawCmdBuffers.size ());
    for ( auto& fence : mWaitFences ) {
        VK_CHECK_RESULT ( vkCreateFence ( mDevice, &fenceCreateInfo, nullptr, &fence ));
    }

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = VkStruct::semaphoreCreateInfo();
    
    VK_CHECK_RESULT ( vkCreateSemaphore ( mDevice, &semaphoreCreateInfo, nullptr, &semaphores.presentComplete )); // Create a semaphore used to synchronize image presentation
    VK_CHECK_RESULT ( vkCreateSemaphore ( mDevice, &semaphoreCreateInfo, nullptr, &semaphores.renderComplete )); // Create a semaphore used to synchronize command submission
}

//----------------------------------------------------------------//
VulkanExampleBase::~VulkanExampleBase () {

	vkDeviceWaitIdle(mDevice);

    // Clean up Vulkan resources
    mSwapChain.cleanup();
    if (mDescriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(mDevice, mDescriptorPool, nullptr);
    }
    destroyCommandBuffers();
    vkDestroyRenderPass(mDevice, mRenderPass, nullptr);
    for (uint32_t i = 0; i < mFrameBuffers.size(); i++) {
        vkDestroyFramebuffer(mDevice, mFrameBuffers[i], nullptr);
    }

    for (auto& shaderModule : mShaderModules) {
        vkDestroyShaderModule(mDevice, shaderModule, nullptr);
    }
    vkDestroyImageView(mDevice, mDepthStencil.mView, nullptr);
    vkDestroyImage(mDevice, mDepthStencil.mImage, nullptr);
    vkFreeMemory(mDevice, mDepthStencil.mMem, nullptr);

    vkDestroyPipelineCache(mDevice, mPipelineCache, nullptr);

    vkDestroyCommandPool(mDevice, mSwapChainQueueCommandPool, nullptr);

    vkDestroySemaphore(mDevice, semaphores.presentComplete, nullptr);
    vkDestroySemaphore(mDevice, semaphores.renderComplete, nullptr);
    
    for (auto& fence : mWaitFences) {
        vkDestroyFence(mDevice, fence, nullptr);
    }

    if ( this->mEnableValidation ) {
        vks::debug::freeDebugCallback ( mInstance );
    }

    vkDestroyCommandPool ( mDevice, mGraphicsCommandPool, nullptr);
    vkDestroyDevice ( mDevice, nullptr );

    vkDestroyInstance ( mInstance, nullptr );
}

//----------------------------------------------------------------//
void VulkanExampleBase::windowResize () {

    // Ensure all operations on the device have been finished before destroying resources
    vkDeviceWaitIdle ( mDevice );

    // Recreate swap chain
    mWidth = mDestWidth;
    mHeight = mDestHeight;
    mSwapChain.create ( &mWidth, &mHeight, this->mUseVsync );

    // Recreate the frame buffers
    vkDestroyImageView(mDevice, mDepthStencil.mView, nullptr);
    vkDestroyImage(mDevice, mDepthStencil.mImage, nullptr);
    vkFreeMemory(mDevice, mDepthStencil.mMem, nullptr);
    setupDepthStencil();
    for (uint32_t i = 0; i < mFrameBuffers.size(); i++) {
        vkDestroyFramebuffer(mDevice, mFrameBuffers[i], nullptr);
    }
    setupFrameBuffer();

    // Command buffers need to be recreated as they may store
    // references to the recreated frame buffer
    destroyCommandBuffers();
    createCommandBuffers();
    buildCommandBuffers();

    vkDeviceWaitIdle(mDevice);

    if (( mWidth > 0.0f ) && ( mHeight > 0.0f )) {
        mCamera.updateAspectRatio (( float )mWidth / ( float )mHeight );
    }

    // Notify derived class
    viewChanged();
}
