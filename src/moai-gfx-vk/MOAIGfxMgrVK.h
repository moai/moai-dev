// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRVK_H
#define	MOAIGFXMGRVK_H

#include <moai-gfx-vk/MOAICommandBufferVK.h>
#include <moai-gfx-vk/MOAIFenceVK.h>
#include <moai-gfx-vk/MOAIGfxInstanceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>
//#include <moai-gfx-vk/MOAIGfxMgrVK_PipelineClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_RenderTreeVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_ResourceCacheVK.h>
//#include <moai-gfx-vk/MOAIGfxMgrVK_ResourceClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_VertexCacheVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAIPhysicalDeviceVK.h>
#include <moai-gfx-vk/MOAISemaphoreVK.h>
#include <moai-gfx-vk/MOAISwapChainVK.h>

////================================================================//
//// MOAIGfxStateFrameVK
////================================================================//
//class MOAIGfxStateFrameVK :
//	public MOAIGfxStateCPUCacheFrame,
//	public MOAIGfxStateGPUCacheFrameVK {
//protected:
//};

//================================================================//
// MOAIGfxStateFrameVK
//================================================================//
class MOAIGfxStateFrameVK :
	public MOAIGfxStateCPUCacheFrame {
protected:
};

//================================================================//
// MOAIGfxMgrVK
//================================================================//
class MOAIGfxMgrVK :
	public ZLContextClass < MOAIGfxMgrVK >,
	public virtual MOAIGfxMgrVKComponents,
	public virtual MOAIGfxMgr,
	public virtual MOAIGfxMgrVK_GPUCacheVK,
//	public virtual MOAIGfxMgrVK_PipelineClerkVK,
	public virtual MOAIGfxMgrVK_RenderTreeVK,
	public virtual MOAIGfxMgrVK_ResourceCacheVK,
//	public virtual MOAIGfxMgrVK_ResourceClerkVK,
	public virtual MOAIGfxMgrVK_VertexCacheVK {
public:

	typedef ZLCallbackWithUserdata < VkSurfaceKHR ( * )( VkInstance, void* )>	HostCreateSurfaceFunc;
	typedef ZLCallbackWithUserdata < cc8** ( * )( void* )>						HostGetInstanceExtensionsFunc;

protected:

	HostCreateSurfaceFunc				mHostCreateSurfaceFunc;
	HostGetInstanceExtensionsFunc		mHostGetInstanceExtensionsFunc;

	MOAIGfxInstanceVK            		mInstance; // Vulkan instance, stores all per-application states
    MOAISurfaceVK                     	mSurface; // Wraps the swap chain to present images (framebuffers) to the windowing system
	MOAIPhysicalDeviceVK				mPhysicalDevice; // Physical device (GPU) that Vulkan will use
	MOAILogicalDeviceVK					mLogicalDevice;
	MOAISwapChainVK						mSwapChain;

	struct {
        VkImage                         mImage;
        VkDeviceMemory                  mMem;
        VkImageView                     mView;
    } mDepthStencil;

	VkRenderPass                        	mRenderPass; // Global render pass for frame buffer writes
	ZLLeanArray < MOAICommandBufferVK >		mDrawCommandBuffers;
	ZLLeanArray < VkFramebuffer >			mFrameBuffers; // Allocate as needed? Bind to command buffer?

	MOAISemaphoreVK						mRenderSemaphore;
	MOAISemaphoreVK						mPresentSemaphore;

//    VkDescriptorPool                    mDescriptorPool = VK_NULL_HANDLE; // Descriptor set pool
//    VkPipelineCache                     mPipelineCache; // Pipeline cache object

//	bool									mHasContext;

//	size_t									mTextureMemoryUsage;
//	u32										mMaxTextureSize;
//
//	u32										mRenderCounter;
//
//	ZLLeanArray < MOAIGfxStateFrameVK* >	mStateStack;
//	ZLIndex									mStateStackTop;

	//----------------------------------------------------------------//
//	static int						_enablePipelineLogging		( lua_State* L );
//	static int						_getFrameBuffer				( lua_State* L );
//	static int						_getMaxTextureSize			( lua_State* L );
//	static int						_getMaxTextureUnits			( lua_State* L );
//	static int						_getViewSize				( lua_State* L );
//	static int						_purgeResources				( lua_State* L );
//	static int						_renewResources				( lua_State* L );

	//----------------------------------------------------------------//
	void							InitCommandBuffers			();
	void							InitDepthStencil			();
	void							InitFrameBuffers			();
	void							InitRenderPass				();

	//----------------------------------------------------------------//
	MOAIShader*						MOAIGfxMgr_AffirmShader						( MOAILuaState& state, int idx ) const;
	MOAITexture*					MOAIGfxMgr_AffirmTexture					( MOAILuaState& state, int idx ) const;
	MOAIGfxScriptRetained*			MOAIGfxMgr_CreateGfxScriptRetained			();
	MOAIImageTexture*				MOAIGfxMgr_CreateImageTexture				();
	MOAIIndexBuffer*				MOAIGfxMgr_CreateIndexBuffer				();
	MOAITexture2D*					MOAIGfxMgr_CreateTexture2D					();
	MOAIVertexArray*				MOAIGfxMgr_CreateVertexArray				();
	MOAIVertexBuffer*				MOAIGfxMgr_CreateVertexBuffer				();
	MOAIVertexFormat*				MOAIGfxMgr_CreateVertexFormat				();
	MOAIShader*						MOAIGfxMgr_GetShaderPreset					( MOAIShaderPresetEnum preset ) const;
	size_t							MOAIGfxMgr_GetTextureMemoryUsage			() const;
	void							MOAIGfxMgr_PopState							();
	void							MOAIGfxMgr_PushState						();
	MOAIGfxMgrVK&					MOAIGfxMgrVKComponents_GetGfxMgrVK			();
	MOAIGfxMgrVK_GPUCacheVK&		MOAIGfxMgrVKComponents_GetGPUCacheVK		();
	MOAIGfxMgrVK_RenderTreeVK&		MOAIGfxMgrVKComponents_GetRenderTreeVK		();
	MOAIGfxMgrVK_ResourceCacheVK&	MOAIGfxMgrVKComponents_GetResourceCacheVK	();
	MOAIGfxMgrVK_VertexCacheVK&		MOAIGfxMgrVKComponents_GetVertexCacheVK		();
	void							ZLContextClass_Finalize						();
	void							ZLContextClass_Initialize					();

public:

	DECL_LUA_SINGLETON ( MOAIGfxMgrVK )

//	GET ( u32, MaxTextureSize, mMaxTextureSize )
//	GET ( bool, HasContext, mHasContext )
//	GET ( u32, RenderCounter, mRenderCounter );

	GET ( MOAICommandBufferVK&, CommandBuffer, this->mDrawCommandBuffers [ this->mSwapChain.GetImageIndex ()]);
	GET ( VkFramebuffer&, FrameBuffer, this->mFrameBuffers [ this->mSwapChain.GetImageIndex ()]);
	GET ( MOAILogicalDeviceVK&, LogicalDevice, this->mLogicalDevice );
	GET ( MOAIPhysicalDeviceVK&, PhysicalDevice, this->mPhysicalDevice );
	GET ( VkRenderPass&, RenderPass, this->mRenderPass );
	GET ( MOAISwapChainVK&, SwapChain, this->mSwapChain );
	
	GET ( MOAISemaphoreVK&, RenderSemaphore, this->mRenderSemaphore );
	GET ( MOAISemaphoreVK&, PresentSemaphore, this->mPresentSemaphore );

	GET_SET ( HostCreateSurfaceFunc, HostCreateSurfaceFunc, mHostCreateSurfaceFunc );
	GET_SET ( HostGetInstanceExtensionsFunc, HostGetInstanceExtensionsFunc, mHostGetInstanceExtensionsFunc );

	//----------------------------------------------------------------//
	void					BeginFrame					();
//	void					Clear						();
//	void					ClearErrors					();
	void					DetectContext				( u32 width, u32 height, bool enableValidation );
//	void					DetectFramebuffer			();
	void					FinishFrame					();
	MOAIShaderVK*			GetShaderPresetVK			( MOAIShaderPresetEnum preset ) const;
	MOAIVertexFormatVK*		GetVertexFormatPresetVK		( MOAIVertexFormatPresetEnum preset ) const;
//	u32						LogErrors					();
							MOAIGfxMgrVK				();
							~MOAIGfxMgrVK				();
//	void					ReportTextureAlloc 			( cc8* name, size_t size );
//	void					ReportTextureFree 			( cc8* name, size_t size );
//	void					ResetDrawCount				();
	
	//----------------------------------------------------------------//
	// disambiguate
	static MOAIGfxMgrVK& Get () {
		return ZLContextClass < MOAIGfxMgrVK >::Get ();
	}
};

#endif
