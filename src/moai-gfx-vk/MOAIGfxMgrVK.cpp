// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIImageTextureVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderMgrVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAIVertexArrayVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatMgrVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIGfxMgrVK::_enablePipelineLogging ( lua_State* L ) {
//	MOAI_LUA_SETUP_SINGLE ( MOAIGfxMgrVK, "" )
//
//	MOAIGfxMgrVK::Get ().EnableQueueLogging ( state.GetValue < bool >( 1, false ));
//
//	ZLFileSys::DeleteDirectory ( GFX_PIPELINE_LOGGING_FOLDER, true, true );
//	ZLFileSys::AffirmPath ( GFX_PIPELINE_LOGGING_FOLDER );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	getFrameBuffer
//	@text	Returns the frame buffer associated with the device.
//
//	@out	MOAIFrameBufferVK frameBuffer
//*/
//int MOAIGfxMgrVK::_getFrameBuffer ( lua_State* L ) {
//
//	MOAILuaState state ( L );
//	state.Push ( MOAIGfxMgrVK::Get ().GetDefaultFrameBuffer ());
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	getMaxTextureSize
//	@text	Returns the maximum texture size supported by device
//
//	@out	number maxTextureSize
//*/
//int MOAIGfxMgrVK::_getMaxTextureSize ( lua_State* L ) {
//
//	MOAILuaState state ( L );
//	state.Push ( MOAIGfxMgrVK::Get ().mMaxTextureSize );
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	getMaxTextureUnits
//	@text	Returns the total number of texture units available on the device.
//
//	@out	number maxTextureUnits
//*/
//int MOAIGfxMgrVK::_getMaxTextureUnits ( lua_State* L ) {
//
//	lua_pushnumber ( L, ( double )MOAIGfxMgrVK::Get ().CountTextureUnits ());
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	getViewSize
//	@text	Returns the width and height of the view
//
//	@out	number width
//	@out	number height
//*/
//int MOAIGfxMgrVK::_getViewSize ( lua_State* L  ) {
//
//	MOAIFrameBufferVK* frameBuffer = MOAICast < MOAIFrameBufferVK >( MOAIGfxMgrVK::Get ().GetCurrentFrameBuffer ());
//
//	lua_pushnumber ( L, frameBuffer->GetBufferWidth ());
//	lua_pushnumber ( L, frameBuffer->GetBufferHeight ());
//
//	return 2;
//}
//
////----------------------------------------------------------------//
///**	@lua	purgeResources
//	@text	Purges all resources older that a given age (in render cycles).
//			If age is 0 then all resources are purged.
//
//	@opt	number age		Default value is 0.
//	@out	nil
//*/
//int MOAIGfxMgrVK::_purgeResources ( lua_State* L ) {
//	MOAILuaState state ( L );
//
//	u32 age = state.GetValue < u32 >( 1, 0 );
//
//	MOAIGfxMgrVK::Get ().PurgeResources ( age );
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	renewResources
//	@text	Renews all resources.
//
//	@out	nil
//*/
//int MOAIGfxMgrVK::_renewResources ( lua_State* L ) {
//	MOAILuaState state ( L );
//
//	MOAIGfxMgrVK::Get ().RenewResources ();
//
//	return 0;
//}

//================================================================//
// MOAIGfxMgrVK
//================================================================//

////----------------------------------------------------------------//
//void MOAIGfxMgrVK::Clear () {
//
//	this->MOAIGfxMgrVK_GPUCacheVK::Clear ();
//	this->MOAIGfxMgr_VertexCache::Clear ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ClearErrors () {
//
//	#ifndef MOAI_OS_NACL
//		if ( this->mHasContext ) {
//			while ( ZLGfxDevice::GetError () != ZGL_ERROR_NONE );
//		}
//	#endif
//}

//----------------------------------------------------------------//
VkResult MOAIGfxMgrVK::CreateInstance ( cc8* name, uint32_t apiVersion ) {

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

    VkApplicationInfo appInfo = MOAIGfxStructVK::applicationInfo ( name, VK_MAKE_VERSION ( 0, 0, 0 ), name, VK_MAKE_VERSION ( 0, 0, 0 ), apiVersion );

    // need to build an array of c strings for VkInstanceCreateInfo. can't be an array of stl strings.
    std::vector < const char* > instanceExtensions;
    std::vector < const char* > validationLayers;
	
//	this->mHost.pushInstanceExtensions ( instanceExtensions );
	
//    instanceExtensions.push_back ( VK_KHR_SURFACE_EXTENSION_NAME );

//    #if defined ( VK_USE_PLATFORM_IOS_MVK )
//        instanceExtensions.push_back ( VK_MVK_IOS_SURFACE_EXTENSION_NAME );
//    #elif defined ( VK_USE_PLATFORM_MACOS_MVK )
//        instanceExtensions.push_back ( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );
//    #endif

	// platform specific
	instanceExtensions.push_back ( VK_KHR_SURFACE_EXTENSION_NAME );
	instanceExtensions.push_back ( VK_MVK_MACOS_SURFACE_EXTENSION_NAME );

    for ( size_t i = 0; i < this->mEnabledInstanceExtensions.size (); ++i ) {
        instanceExtensions.push_back ( this->mEnabledInstanceExtensions [ i ]);
    }

//    if ( this->mEnableValidation ) {
//        instanceExtensions.push_back ( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
//        validationLayers.push_back ( "VK_LAYER_LUNARG_standard_validation" );
//    }

    VkInstanceCreateInfo instanceCreateInfo = MOAIGfxStructVK::instanceCreateInfo (
        &appInfo,
        instanceExtensions.data (),
        ( uint32_t )instanceExtensions.size (),
        validationLayers.data (),
        ( uint32_t )validationLayers.size ()
     );
	
    VK_CHECK_RESULT ( vkCreateInstance ( &instanceCreateInfo, nullptr, &mInstance ));
	
//    if ( this->mEnableValidation ) {
//        VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
//        vks::debug::setupDebugging ( this->mInstance, debugReportFlags, VK_NULL_HANDLE );
//    }
    return VK_SUCCESS;
}

//----------------------------------------------------------------//
// Create the logical device based on the assigned physical device, also gets default queue family indices
VkResult MOAIGfxMgrVK::CreateLogicalDevice ( bool useSwapChain, VkQueueFlags requestedQueueTypes ) {

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
    vkGetPhysicalDeviceQueueFamilyProperties ( this->mPhysicalDevice, &queueFamilyCount, nullptr );
    assert ( queueFamilyCount > 0 );

    std::vector < VkQueueFamilyProperties > queueFamilyProperties;
    queueFamilyProperties.resize ( queueFamilyCount );
    vkGetPhysicalDeviceQueueFamilyProperties ( this->mPhysicalDevice, &queueFamilyCount, queueFamilyProperties.data ());

    uint32_t graphicsQueueFamilyIndex = 0;
    uint32_t computeQueueFamilyIndex = 0;
    uint32_t transferQueueFamilyIndex = 0;

    // Graphics queue
    if ( requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT ) {
        graphicsQueueFamilyIndex = MOAIGfxUtilVK::GetQueueFamilyIndex ( queueFamilyProperties, VK_QUEUE_GRAPHICS_BIT, 0 );
        queueCreateInfos.push_back ( MOAIGfxStructVK::deviceQueueCreateInfo ( graphicsQueueFamilyIndex ));
    }

    // Dedicated compute queue
    if ( requestedQueueTypes & VK_QUEUE_COMPUTE_BIT ) {
        computeQueueFamilyIndex = MOAIGfxUtilVK::GetQueueFamilyIndex ( queueFamilyProperties, VK_QUEUE_COMPUTE_BIT, graphicsQueueFamilyIndex );
        if ( computeQueueFamilyIndex != graphicsQueueFamilyIndex ) {
            queueCreateInfos.push_back ( MOAIGfxStructVK::deviceQueueCreateInfo ( computeQueueFamilyIndex ));
        }
    }

    // Dedicated transfer queue
    if ( requestedQueueTypes & VK_QUEUE_TRANSFER_BIT ) {
        transferQueueFamilyIndex = MOAIGfxUtilVK::GetQueueFamilyIndex ( queueFamilyProperties, VK_QUEUE_TRANSFER_BIT, graphicsQueueFamilyIndex );
        if (( transferQueueFamilyIndex != graphicsQueueFamilyIndex ) && ( transferQueueFamilyIndex != computeQueueFamilyIndex )) {
            queueCreateInfos.push_back ( MOAIGfxStructVK::deviceQueueCreateInfo ( transferQueueFamilyIndex ));
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
    deviceCreateInfo.pEnabledFeatures = &this->mEnabledFeatures;

	std::vector < const char* > deviceExtensions;

	// platform specific
	deviceExtensions.push_back ( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

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
    this->mGraphicsCommandPool = MOAIGfxUtilVK::CreateCommandPool ( this->mDevice, graphicsQueueFamilyIndex );

    // Get a graphics queue from the device
    vkGetDeviceQueue ( this->mDevice, graphicsQueueFamilyIndex, 0, &this->mQueue );

    return VK_SUCCESS;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::DetectContext ( u32 width, u32 height, bool vsync ) {

	bool enableValidation	= false;
	bool useVsync			= false;
	uint32_t apiVersion		= VK_API_VERSION_1_0;

	VK_CHECK_RESULT ( this->CreateInstance ( "MOAI VK", apiVersion ));

    uint32_t gpuCount = 0;
    VK_CHECK_RESULT ( vkEnumeratePhysicalDevices ( mInstance, &gpuCount, nullptr ));
    assert ( gpuCount > 0 );
	
    std::vector < VkPhysicalDevice > physicalDevices ( gpuCount );
    VK_CHECK_RESULT ( vkEnumeratePhysicalDevices ( mInstance, &gpuCount, physicalDevices.data ()));

    mPhysicalDevice = physicalDevices [ 0 ];
    vkGetPhysicalDeviceProperties ( mPhysicalDevice, &this->mPhysicalDeviceProperties );
    vkGetPhysicalDeviceFeatures ( mPhysicalDevice, &this->mPhysicalDeviceFeature );
    vkGetPhysicalDeviceMemoryProperties ( mPhysicalDevice, &this->mPhysicalDeviceMemoryProperties );

    // Find a suitable depth format
//    VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat ( mPhysicalDevice, &mDepthFormat );
//    assert ( validDepthFormat );

    // Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
    //getEnabledFeatures();

    VK_CHECK_RESULT ( this->CreateLogicalDevice ());

	// create pipeline cache
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = MOAIGfxStructVK::pipelineCacheCreateInfo ();
	VK_CHECK_RESULT ( vkCreatePipelineCache ( this->mDevice, &pipelineCacheCreateInfo, nullptr, &this->mPipelineCache ));

	// create the surface
	if ( this->mHostCreateSurfaceFunc.first ) {
		
		VkSurfaceKHR surface;
		this->mHostCreateSurfaceFunc.first ( this->mInstance, surface, this->mHostCreateSurfaceFunc.second );
		assert ( surface );

		// init swapchain
		this->mSwapChain.initSurface ( this->mInstance, this->mDevice, this->mPhysicalDevice, surface );
		this->mSwapChain.createSwapChain ( this->mInstance, this->mDevice, this->mPhysicalDevice, width, height, vsync ); // TODO: use vsync?
//		mSwapChainQueueCommandPool = vks::tools::createCommandPool ( mDevice, mSwapChain.queueNodeIndex );
	}

//    // gotta do after creating the swap chain
//    setupRenderPass ();
//
//    createCommandBuffers ();
//    setupDepthStencil ();
//    setupFrameBuffer ();
//
//    // create synchronization primitives
//    // Wait fences to sync command buffer access
//    VkFenceCreateInfo fenceCreateInfo = MOAIGfxStructVK::fenceCreateInfo ( VK_FENCE_CREATE_SIGNALED_BIT );
//    mWaitFences.resize ( mDrawCmdBuffers.size ());
//    for ( auto& fence : mWaitFences ) {
//        VK_CHECK_RESULT ( vkCreateFence ( mDevice, &fenceCreateInfo, nullptr, &fence ));
//    }
//
//    // Create synchronization objects
//    VkSemaphoreCreateInfo semaphoreCreateInfo = MOAIGfxStructVK::semaphoreCreateInfo();
//
//    VK_CHECK_RESULT ( vkCreateSemaphore ( mDevice, &semaphoreCreateInfo, nullptr, &semaphores.presentComplete )); // Create a semaphore used to synchronize image presentation
//    VK_CHECK_RESULT ( vkCreateSemaphore ( mDevice, &semaphoreCreateInfo, nullptr, &semaphores.renderComplete )); // Create a semaphore used to synchronize command submission

//	this->mHasContext = true;
//
//	ZLGfxDevice::Initialize ();
//
//	u32 maxTextureUnits = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_UNITS );
//	this->InitTextureUnits ( maxTextureUnits );
//
//	this->mMaxTextureSize = ZLGfxDevice::GetCap ( ZGL_CAPS_MAX_TEXTURE_SIZE );
//
//	// renew resources in immediate mode
//	this->SelectDrawingAPI ();
//
//	MOAICast < MOAIFrameBufferVK >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
//
//	// TODO: ZLGfx - does this need to be called after shader mgr setup>
//	this->RenewResources ();
}

////----------------------------------------------------------------//
//void MOAIGfxMgrVK::DetectFramebuffer () {
//
//	MOAICast < MOAIFrameBufferVK >( this->GetDefaultFrameBuffer ())->DetectGLFrameBufferID ( *this );
//	this->SetFrameBuffer ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::FinishFrame () {
//
//	this->FlushToGPU (); // TODO: need to do this here?
//	this->UnbindAll ();
//	this->Reset ();
//}
//
////----------------------------------------------------------------//
//u32 MOAIGfxMgrVK::LogErrors () {
//
//	// TODO: ZLGfx
//
//	u32 count = 0;
//	#ifndef MOAI_OS_NACL
//		if ( this->mHasContext ) {
//			for ( u32 error = ZLGfxDevice::GetError (); error != ZGL_ERROR_NONE; error = ZLGfxDevice::GetError (), ++count ) {
////				MOAILogF ( 0, ZLLog::LOG_ERROR, MOAISTRING_MOAIGfxDevice_OpenGLError_S, ZLGfxDevice::GetErrorString ( error ));
//			}
//		}
//	#endif
//	return count;
//}
//
////----------------------------------------------------------------//
//MOAIGfxMgrVK::MOAIGfxMgrVK () :
//	mHasContext ( false ),
//	mIsFramebufferSupported ( 0 ),
//	#if defined ( MOAI_OS_NACL ) || defined ( MOAI_OS_IPHONE ) || defined ( MOAI_OS_ANDROID ) || defined ( EMSCRIPTEN )
//		mIsOpenGLES ( true ),
//	#else
//		mIsOpenGLES ( false ),
//	#endif
//	mMajorVersion ( 0 ),
//	mMinorVersion ( 0 ),
//	mTextureMemoryUsage ( 0 ),
//	mMaxTextureSize ( 0 ),
//	mRenderCounter ( 0 ),
//	mStateStackTop ( ZLIndexOp::ZERO ) {
//
//	RTTI_BEGIN
//		RTTI_SINGLE ( MOAIGfxMgr )
//	RTTI_END
//
//	this->SetDefaultFrameBuffer ( new MOAIFrameBufferVK ());
//}

//----------------------------------------------------------------//
MOAIGfxMgrVK::MOAIGfxMgrVK () :
	mHostCreateSurfaceFunc ( NULL, NULL ) {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGfxMgr )
	RTTI_END
	
//	this->SetDefaultFrameBuffer ( new MOAIFrameBufferVK ());
}

//----------------------------------------------------------------//
MOAIGfxMgrVK::~MOAIGfxMgrVK () {

//	this->Clear ();
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mStateStack.Size (); ++i ) {
//		delete this->mStateStack [ i ];
//	}
//
//	this->SetDefaultFrameBuffer ( 0 );
//	this->SetDefaultTexture ( 0 );
}

////----------------------------------------------------------------//
//void MOAIGfxMgrVK::OnGlobalsFinalize () {
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::OnGlobalsInitialize () {
//
//	this->AffirmBuffers ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ReportTextureAlloc ( cc8* name, size_t size ) {
//	UNUSED ( name );
//	UNUSED ( size );
//
//	// TODO: ZLGfx
//
////	this->mTextureMemoryUsage += size;
////	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
////	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "+", size / 1024, mb, name );
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ReportTextureFree ( cc8* name, size_t size ) {
//	UNUSED ( name );
//	UNUSED ( size );
//
//	// TODO: ZLGfx
//
////	this->mTextureMemoryUsage -= size;
////	float mb = ( float )this->mTextureMemoryUsage / 1024.0f / 1024.0f;
////	MOAILogF ( 0, ZLLog::LOG_STATUS, MOAISTRING_MOAITexture_MemoryUse_SDFS, "-", size / 1024, mb, name );
//}
//
////----------------------------------------------------------------//
//void MOAIGfxMgrVK::ResetDrawCount () {
//	//this->mDrawCount = 0;
//}

//================================================================//
// overrides
//================================================================//

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrVK::MOAIGfxMgr_AffirmShader ( MOAILuaState& state, int idx ) const {

//	MOAIShaderVK* shader = 0;
//
//	if ( state.IsType ( idx, LUA_TNUMBER )) {
//		shader = MOAIShaderMgrVK::Get ().GetShader (( MOAIShaderPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER ));
//	}
//	else {
//		shader = state.GetLuaObject < MOAIShaderVK >( idx, true );
//	}
//	return shader;
	return new MOAIShaderVK ();
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgrVK::MOAIGfxMgr_AffirmTexture ( MOAILuaState& state, int idx ) const {

//	MOAITextureVK* textureBase = 0;
//
//	textureBase = state.GetLuaObject < MOAITextureVK >( idx, false );
//	if ( textureBase ) return textureBase;
//
//	MOAITexture2DVK* texture = new MOAITexture2DVK ();
//	if ( !texture->Init ( state, idx )) {
//		// TODO: report error
//		delete texture;
//		texture = 0;
//	}
//	return texture;

	return new MOAITexture2DVK ();
}

//----------------------------------------------------------------//
MOAIImageTexture* MOAIGfxMgrVK::MOAIGfxMgr_CreateImageTexture () const {

	return new MOAIImageTextureVK ();
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIGfxMgrVK::MOAIGfxMgr_CreateIndexBuffer () const {

	return new MOAIIndexBufferVK ();
}

//----------------------------------------------------------------//
MOAITexture2D* MOAIGfxMgrVK::MOAIGfxMgr_CreateTexture2D () const {

	return new MOAITexture2DVK ();
}

//----------------------------------------------------------------//
MOAIVertexArray* MOAIGfxMgrVK::MOAIGfxMgr_CreateVertexArray () const {

	return new MOAIVertexArrayVK ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIGfxMgrVK::MOAIGfxMgr_CreateVertexBuffer () const {

	return new MOAIVertexBufferVK ();
}

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgrVK::MOAIGfxMgr_GetShaderPreset ( MOAIShaderPresetEnum preset ) const {

	return MOAIShaderMgrVK::Get ().GetShader ( preset );
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrVK::MOAIGfxMgr_GetTextureMemoryUsage () const {

//	return this->mTextureMemoryUsage;
	return 0;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgrVK::MOAIGfxMgr_GetVertexFormatPreset ( MOAIVertexFormatPresetEnum preset ) const {

	return MOAIVertexFormatMgrVK::Get ().GetFormat ( preset );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::MOAIGfxMgr_PopState () {

//	assert ( this->mStateStackTop > 0 );
//
//	this->FlushToGPU ();
//
//	MOAIGfxStateFrameVK* frame = this->mStateStack [ --this->mStateStackTop ];
//
//	this->RestoreCPUState ( *frame );
//	this->RestoreGPUState ( *frame );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::MOAIGfxMgr_PushState () {

//	this->FlushToGPU ();
//
//	this->mStateStack.Grow (( ZLSize )this->mStateStackTop + 1, 0 );
//	if ( !this->mStateStack [ this->mStateStackTop ]) {
//		this->mStateStack [ this->mStateStackTop ] = new MOAIGfxStateFrameVK ();
//	}
//
//	MOAIGfxStateFrameVK* frame = this->mStateStack [ this->mStateStackTop++ ];
//
//	this->StoreCPUState ( *frame );
//	this->StoreGPUState ( *frame );
}

//----------------------------------------------------------------//
MOAIGfxMgrVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetGfxMgrVK () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetGPUCacheVK () {
	return *this;
}

////----------------------------------------------------------------//
//MOAIGfxMgrVK_PipelineClerkVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetPipelineClerkVK () {
//	return *this;
//}

//----------------------------------------------------------------//
MOAIGfxMgrVK_RenderTreeVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetRenderTreeVK () {
	return *this;
}

////----------------------------------------------------------------//
//MOAIGfxMgrVK_ResourceClerkVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetResourceClerkVK () {
//	return *this;
//}

//----------------------------------------------------------------//
MOAIGfxMgrVK_VertexCacheVK& MOAIGfxMgrVK::MOAIGfxMgrVKComponents_GetVertexCacheVK () {
	return *this;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

//	state.SetField ( -1, "EVENT_RESIZE",	( u32 )EVENT_RESIZE );
//	
//	state.SetField ( -1, "DRAWING_QUEUE",	( u32 )MOAIGfxMgrVK_PipelineClerkVK::DRAWING_QUEUE );
//	state.SetField ( -1, "LOADING_QUEUE",	( u32 )MOAIGfxMgrVK_PipelineClerkVK::LOADING_QUEUE );
//
//	luaL_Reg regTable [] = {
//		{ "enablePipelineLogging",		_enablePipelineLogging },
//		{ "getFrameBuffer",				_getFrameBuffer },
//		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIGfxMgrVK > },
//		{ "getMaxTextureSize",			_getMaxTextureSize },
//		{ "getMaxTextureUnits",			_getMaxTextureUnits },
//		{ "getViewSize",				_getViewSize },
//		{ "purgeResources",				_purgeResources },
//		{ "renewResources",				_renewResources },
//		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIGfxMgrVK > },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}
