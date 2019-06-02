// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_H
#define	MOAIGFXMGRGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_PipelineClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_GPUCacheGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_RenderTreeGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_ResourceClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_VertexCacheGL.h>

//================================================================//
// MOAIGfxStateFrameGL
//================================================================//
class MOAIGfxStateFrameGL :
	public MOAIGfxStateCPUCacheFrame,
	public MOAIGfxStateGPUCacheFrameGL {
protected:
};

//================================================================//
// MOAIGfxMgrGL
//================================================================//
class MOAIGfxMgrGL :
	public ZLContextClass < MOAIGfxMgrGL >,
	public virtual MOAIGfxMgrGLComponents,
	public virtual MOAIGfxMgr,
	public virtual MOAIGfxMgrGL_GPUCacheGL,
	public virtual MOAIGfxMgrGL_PipelineClerkGL,
	public virtual MOAIGfxMgrGL_RenderTreeGL,
	public virtual MOAIGfxMgrGL_ResourceClerkGL,
	public virtual MOAIGfxMgrGL_VertexCacheGL {
protected:

	bool									mHasContext;

	bool									mIsFramebufferSupported;
	bool									mIsOpenGLES;

	u32										mMajorVersion;
	u32										mMinorVersion;
	
	size_t									mTextureMemoryUsage;
	u32										mMaxTextureSize;

	u32										mRenderCounter;

	ZLLeanArray < MOAIGfxStateFrameGL* >	mStateStack;
	ZLIndex									mStateStackTop;

	//----------------------------------------------------------------//
	static int						_enablePipelineLogging		( lua_State* L );
	static int						_getFrameBuffer				( lua_State* L );
	static int						_getMaxTextureSize			( lua_State* L );
	static int						_getMaxTextureUnits			( lua_State* L );
	static int						_getViewSize				( lua_State* L );
	static int						_purgeResources				( lua_State* L );
	static int						_renewResources				( lua_State* L );

	//----------------------------------------------------------------//
	void							OnGlobalsFinalize			();
	void							OnGlobalsInitialize			();

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
	MOAIGfxMgrGL&					MOAIGfxMgrGLComponents_GetGfxMgrGL			();
	MOAIGfxMgrGL_GPUCacheGL&		MOAIGfxMgrGLComponents_GetGPUCacheGL		();
	MOAIGfxMgrGL_PipelineClerkGL&	MOAIGfxMgrGLComponents_GetPipelineClerkGL	();
	MOAIGfxMgrGL_RenderTreeGL&		MOAIGfxMgrGLComponents_GetRenderTreeGL		();
	MOAIGfxMgrGL_ResourceClerkGL&	MOAIGfxMgrGLComponents_GetResourceClerkGL	();
	MOAIGfxMgrGL_VertexCacheGL&		MOAIGfxMgrGLComponents_GetVertexCacheGL		();

public:

	DECL_LUA_SINGLETON ( MOAIGfxMgrGL )

	GET ( u32, MaxTextureSize, mMaxTextureSize )
	GET ( bool, HasContext, mHasContext )
	
	GET_BOOL ( IsOpenGLES, mIsOpenGLES )
	GET_BOOL ( IsFramebufferSupported, mIsFramebufferSupported )

	GET ( u32, RenderCounter, mRenderCounter );

	//----------------------------------------------------------------//
//	void					Clear						();
	void					ClearErrors					();
	void					DetectContext				();
	void					DetectFramebuffer			();
	void					FinishFrame					();
	u32						LogErrors					();
							MOAIGfxMgrGL				();
							~MOAIGfxMgrGL				();
	void					MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void					ReportTextureAlloc 			( cc8* name, size_t size );
	void					ReportTextureFree 			( cc8* name, size_t size );
	void					ResetDrawCount				();
	
	//----------------------------------------------------------------//
	// disambiguate
	static MOAIGfxMgrGL& Get () {
		return ZLContextClass < MOAIGfxMgrGL >::Get ();
	}
};

#endif
