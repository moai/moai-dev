// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxStructVK.h>
#include <moai-gfx-vk/MOAIImageTextureVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIOneTriVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderMgrVK.h>
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

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

//----------------------------------------------------------------//
void MOAIGfxMgrVK::BeginFrame () {

	VkResult result = this->mSwapChain.AcquireNextImage ( this->mLogicalDevice, this->mPresentCompleteSemaphore, this->mCurrentBufferIndex );
	
    if (( result == VK_ERROR_OUT_OF_DATE_KHR ) || ( result == VK_SUBOPTIMAL_KHR )) {
//        windowResize ();
    }
    else {
        VK_CHECK_RESULT ( result );
    }
}

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
void MOAIGfxMgrVK::DetectContext ( u32 width, u32 height, bool enableValidation ) {

	assert ( this->mHostCreateSurfaceFunc );
	assert ( this->mHostGetInstanceExtensionsFunc );

	cc8** hostInstanceExtensions = this->mHostGetInstanceExtensionsFunc.mFunc ( this->mHostGetInstanceExtensionsFunc.mUserdata );

	this->mInstance.Init ( "MOAI VK", VK_API_VERSION_1_0, hostInstanceExtensions, enableValidation );
	this->mSurface.Init ( this->mHostCreateSurfaceFunc.mFunc ( this->mInstance, this->mHostCreateSurfaceFunc.mUserdata ));
	this->mPhysicalDevice.Init ( this->mInstance, this->mSurface );
	this->mLogicalDevice.Init ( this->mPhysicalDevice );
	this->mSwapChain.Init ( this->mInstance, this->mPhysicalDevice, this->mLogicalDevice, this->mSurface, width, height );

	this->InitRenderPass ();
	this->InitDepthStencil ();
	this->InitFrameBuffers ();
	this->InitCommandBuffers ();
	this->InitSemaphores ();

	// create pipeline cache
//	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = MOAIGfxStructVK::pipelineCacheCreateInfo ();
//	VK_CHECK_RESULT ( vkCreatePipelineCache ( this->mLogicalDevice, &pipelineCacheCreateInfo, nullptr, &this->mPipelineCache ));

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

//----------------------------------------------------------------//
void MOAIGfxMgrVK::FinishFrame () {
	
	VkCommandBuffer& cmdBuffer = this->GetCommandBuffer ();
	VkFence& fence = this->GetFence ();
	
	// Use a fence to wait until the command buffer has finished execution before using it again
	VK_CHECK_RESULT ( vkWaitForFences ( this->mLogicalDevice, 1, &fence, VK_TRUE, UINT64_MAX ));
	VK_CHECK_RESULT ( vkResetFences ( this->mLogicalDevice, 1, &fence ));

	// Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
	VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	// The submit info structure specifices a command buffer queue submission batch
	VkSubmitInfo submitInfo = MOAIGfxStructVK::submitInfo (
		&cmdBuffer, 						// Command buffers to submit in this batch
		1,									// One command buffer
		&this->mRenderCompleteSemaphore,	// Semaphore(s) to be signaled when command buffers have completed
		1,									// One signal semaphore
		&this->mPresentCompleteSemaphore,	// Semaphore(s) to wait upon before the submitted command buffer starts executing
		1,									// One wait semaphore
		&waitStageMask						// Pointer to the list of pipeline stages that the semaphore waits will occur at
	);

	MOAIQueueVK& graphicsQueue = this->mLogicalDevice.GetGraphicsQueue ();

	VK_CHECK_RESULT ( graphicsQueue.Submit ( &submitInfo, 1, fence ));

    VkResult result = this->mSwapChain.QueuePresent ( this->mLogicalDevice, graphicsQueue, this->mCurrentBufferIndex, this->mRenderCompleteSemaphore );
    if (!((result == VK_SUCCESS) || (result == VK_SUBOPTIMAL_KHR))) {
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            // Swap chain is no longer compatible with the surface and needs to be recreated
//            windowResize();
            return;
        } else {
            VK_CHECK_RESULT ( result );
        }
    }
    VK_CHECK_RESULT ( graphicsQueue.WaitIdle ());
	
//	this->UnbindAll ();
	this->Reset ();
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::InitCommandBuffers () {

	ZLSize imageCount = this->mSwapChain.mImages.Size ();

	this->mDrawCommandBuffers.Init ( this->mSwapChain.mImages.Size ());
	
    VkCommandBufferAllocateInfo cmdBufAllocateInfo = MOAIGfxStructVK::commandBufferAllocateInfo (
    	this->mLogicalDevice.GetGraphicsQueue ().mPool,
    	VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    	( u32 )imageCount
	);
    this->mLogicalDevice.AllocateCommandBuffers ( cmdBufAllocateInfo, this->mDrawCommandBuffers.GetBuffer ());
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::InitDepthStencil () {

	VkImageCreateInfo imageCreateInfo = MOAIGfxStructVK::imageCreateInfo (
		VK_IMAGE_TYPE_2D,
		this->mPhysicalDevice.mDepthFormat,
		MOAIGfxStructVK::extent3D ( this->mSwapChain.mExtent.width, this->mSwapChain.mExtent.height, 1 ),
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
	);
	this->mDepthStencil.mImage = this->mLogicalDevice.CreateImage ( imageCreateInfo );

	VkImageViewCreateInfo depthStencilImageViewCreateInfo = MOAIGfxStructVK::imageViewCreateInfo (
		NULL,
		VK_IMAGE_VIEW_TYPE_2D,
		this->mPhysicalDevice.mDepthFormat,
		MOAIGfxStructVK::componentMapping (),
		MOAIGfxStructVK::imageSubresourceRange ( VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT )
	);

	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements ( this->mLogicalDevice, mDepthStencil.mImage, &memReqs );

	u32 memoryTypeIndex;
	bool didFindMemoryTypeIndex = this->mPhysicalDevice.FindMemoryTypeIndex ( memoryTypeIndex, memReqs.memoryTypeBits );
	assert ( didFindMemoryTypeIndex );

	VkMemoryAllocateInfo memAllocInfo = MOAIGfxStructVK::memoryAllocateInfo ( memReqs.size, memoryTypeIndex );
	this->mDepthStencil.mMem = this->mLogicalDevice.AllocateMemory ( memAllocInfo );
	this->mLogicalDevice.BindImageMemory ( this->mDepthStencil.mImage, this->mDepthStencil.mMem );

	depthStencilImageViewCreateInfo.image = mDepthStencil.mImage;
	this->mDepthStencil.mView = this->mLogicalDevice.CreateImageView ( depthStencilImageViewCreateInfo );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::InitFrameBuffers () {

	VkImageView attachments [ 2 ];

    // Depth/Stencil attachment is the same for all frame buffers
    attachments [ 1 ] = this->mDepthStencil.mView;

    VkFramebufferCreateInfo framebufferCreateInfo = MOAIGfxStructVK::framebufferCreateInfo (
    	this->mRenderPass,
    	attachments,
    	2,
    	this->mSwapChain.mExtent.width,
    	this->mSwapChain.mExtent.height
	);

	ZLSize imageCount = this->mSwapChain.mImages.Size ();

    // Create frame buffers for every swap chain image
    this->mFrameBuffers.Init ( imageCount );
    for ( ZLIndex i = ZLIndexOp::ZERO; i < imageCount; ++i ) {
        attachments [ 0 ] = this->mSwapChain.mBuffers [ i ].view;
        this->mFrameBuffers [ i ] = this->mLogicalDevice.CreateFramebuffer ( framebufferCreateInfo );
    }
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::InitRenderPass () {
	
	VkAttachmentDescription attachments [ 2 ];
	
    // Color attachment
    attachments [ 0 ] = MOAIGfxStructVK::attachmentDescription (
        this->mSwapChain.mSurfaceFormat.format,
        VK_SAMPLE_COUNT_1_BIT,
        VK_ATTACHMENT_LOAD_OP_CLEAR,
        VK_ATTACHMENT_STORE_OP_STORE,
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        VK_ATTACHMENT_STORE_OP_DONT_CARE,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    );

    // Depth attachment
    attachments [ 1 ] = MOAIGfxStructVK::attachmentDescription (
        this->mPhysicalDevice.mDepthFormat,
        VK_SAMPLE_COUNT_1_BIT,
        VK_ATTACHMENT_LOAD_OP_CLEAR,
        VK_ATTACHMENT_STORE_OP_STORE,
        VK_ATTACHMENT_LOAD_OP_CLEAR,
        VK_ATTACHMENT_STORE_OP_DONT_CARE,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    );

    VkAttachmentReference colorReference		= MOAIGfxStructVK::attachmentReference ( 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
    VkAttachmentReference depthReference		= MOAIGfxStructVK::attachmentReference ( 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );
    VkSubpassDescription subpassDescription		= MOAIGfxStructVK::subpassDescription ( VK_PIPELINE_BIND_POINT_GRAPHICS, &colorReference, 1, &depthReference );

    // Subpass dependencies for layout transitions
    VkSubpassDependency dependencies [ 2 ];

    dependencies [ 0 ] = MOAIGfxStructVK::subpassDependency (
        VK_SUBPASS_EXTERNAL,
        0,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_DEPENDENCY_BY_REGION_BIT
    );

    dependencies [ 1 ] = MOAIGfxStructVK::subpassDependency (
        0,
        VK_SUBPASS_EXTERNAL,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_ACCESS_MEMORY_READ_BIT,
        VK_DEPENDENCY_BY_REGION_BIT
    );

    VkRenderPassCreateInfo renderPassInfo = MOAIGfxStructVK::renderPassCreateInfo (
        attachments,
        2,
        &subpassDescription,
        1,
        dependencies,
        2
    );

 	this->mRenderPass = this->mLogicalDevice.CreateRenderPass ( renderPassInfo );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::InitSemaphores () {

	VkFenceCreateInfo fenceCreateInfo = MOAIGfxStructVK::fenceCreateInfo ( VK_FENCE_CREATE_SIGNALED_BIT );
	
	ZLSize cmdBufferCount = this->mDrawCommandBuffers.Size ();
	this->mWaitFences.Init ( cmdBufferCount );
    for ( ZLIndex i = ZLIndexOp::ZERO; i < cmdBufferCount; ++i ) {
        VK_CHECK_RESULT ( vkCreateFence ( this->mLogicalDevice, &fenceCreateInfo, NULL, &this->mWaitFences [ i ]));
    }

    VkSemaphoreCreateInfo semaphoreCreateInfo = MOAIGfxStructVK::semaphoreCreateInfo ();

	this->mPresentCompleteSemaphore	= this->mLogicalDevice.CreateSemaphore ( semaphoreCreateInfo ); // Create a semaphore used to synchronize image presentation
	this->mRenderCompleteSemaphore	= this->mLogicalDevice.CreateSemaphore ( semaphoreCreateInfo ); // Create a semaphore used to synchronize command submission
}

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
	mHostCreateSurfaceFunc ( NULL, NULL ),
	mCurrentBufferIndex ( ZLIndexOp::ZERO ) {
	
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
// virtual
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

	MOAITextureVK* textureBase = 0;

	textureBase = state.GetLuaObject < MOAITextureVK >( idx, false );
	if ( textureBase ) return textureBase;

	MOAITexture2DVK* texture = new MOAITexture2DVK ();
	if ( !texture->Init ( state, idx )) {
		// TODO: report error
		delete texture;
		texture = NULL;
	}
	return texture;
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

	return NULL;
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIGfxMgrVK::MOAIGfxMgr_CreateVertexBuffer () const {

	return new MOAIVertexBufferVK ();
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIGfxMgrVK::MOAIGfxMgr_CreateVertexFormat () const {

	return new MOAIVertexFormatVK ();
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

//----------------------------------------------------------------//
void MOAIGfxMgrVK::ZLContextClass_Finalize () {
	MOAIGfxMgr::ZLContextClass_Finalize ();

	this->GetRenderTreeVK ().mOneTri = NULL;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK::ZLContextClass_Initialize () {
	MOAIGfxMgr::ZLContextClass_Initialize ();
}
