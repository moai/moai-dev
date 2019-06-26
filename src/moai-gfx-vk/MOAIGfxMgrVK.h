// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRVK_H
#define	MOAIGFXMGRVK_H

#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>
//#include <moai-gfx-vk/MOAIGfxMgrVK_PipelineClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_RenderTreeVK.h>
//#include <moai-gfx-vk/MOAIGfxMgrVK_ResourceClerkVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_VertexCacheVK.h>

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
//	public virtual MOAIGfxMgrVK_ResourceClerkVK,
	public virtual MOAIGfxMgrVK_VertexCacheVK {
protected:

//	bool									mHasContext;
//
//	bool									mIsFramebufferSupported;
//	bool									mIsOpenGLES;
//
//	u32										mMajorVersion;
//	u32										mMinorVersion;
//
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
//	void							OnGlobalsFinalize			();
//	void							OnGlobalsInitialize			();

	//----------------------------------------------------------------//
	MOAIShader*						MOAIGfxMgr_AffirmShader						( MOAILuaState& state, int idx ) const;
	MOAITexture*					MOAIGfxMgr_AffirmTexture					( MOAILuaState& state, int idx ) const;
	MOAIImageTexture*				MOAIGfxMgr_CreateImageTexture				() const;
	MOAIIndexBuffer*				MOAIGfxMgr_CreateIndexBuffer				() const;
	MOAITexture2D*					MOAIGfxMgr_CreateTexture2D					() const;
	MOAIVertexArray*				MOAIGfxMgr_CreateVertexArray				() const;
	MOAIVertexBuffer*				MOAIGfxMgr_CreateVertexBuffer				() const;
	MOAIShader*						MOAIGfxMgr_GetShaderPreset					( MOAIShaderPresetEnum preset ) const;
	size_t							MOAIGfxMgr_GetTextureMemoryUsage			() const;
	MOAIVertexFormat*				MOAIGfxMgr_GetVertexFormatPreset			( MOAIVertexFormatPresetEnum preset ) const;
	void							MOAIGfxMgr_PopState							();
	void							MOAIGfxMgr_PushState						();
	MOAIGfxMgrVK&					MOAIGfxMgrVKComponents_GetGfxMgrVK			();
	MOAIGfxMgrVK_GPUCacheVK&		MOAIGfxMgrVKComponents_GetGPUCacheVK		();
//	MOAIGfxMgrVK_PipelineClerkVK&	MOAIGfxMgrVKComponents_GetPipelineClerkVK	();
	MOAIGfxMgrVK_RenderTreeVK&		MOAIGfxMgrVKComponents_GetRenderTreeVK		();
//	MOAIGfxMgrVK_ResourceClerkVK&	MOAIGfxMgrVKComponents_GetResourceClerkVK	();
	MOAIGfxMgrVK_VertexCacheVK&		MOAIGfxMgrVKComponents_GetVertexCacheVK		();
	void							MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );

public:

	DECL_LUA_SINGLETON ( MOAIGfxMgrVK )

//	GET ( u32, MaxTextureSize, mMaxTextureSize )
//	GET ( bool, HasContext, mHasContext )
//
//	GET_BOOL ( IsOpenGLES, mIsOpenGLES )
//	GET_BOOL ( IsFramebufferSupported, mIsFramebufferSupported )
//
//	GET ( u32, RenderCounter, mRenderCounter );

	//----------------------------------------------------------------//
//	void					Clear						();
//	void					ClearErrors					();
//	void					DetectContext				();
//	void					DetectFramebuffer			();
//	void					FinishFrame					();
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
