// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_H
#define	MOAIGFXMGRGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_PipelineClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_ResourceClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_GPUCacheGL.h>
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
	virtual public MOAIGfxMgrGLComponents,
	virtual public MOAIGfxMgr,
	virtual public MOAIGfxMgrGL_GPUCacheGL,
	virtual public MOAIGfxMgrGL_PipelineClerkGL,
	virtual public MOAIGfxMgrGL_ResourceClerkGL,
	virtual public MOAIGfxMgrGL_VertexCacheGL {
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
	static int					_enablePipelineLogging		( lua_State* L );
	static int					_getFrameBuffer				( lua_State* L );
	static int					_getMaxTextureSize			( lua_State* L );
	static int					_getMaxTextureUnits			( lua_State* L );
	static int					_getViewSize				( lua_State* L );
	static int					_purgeResources				( lua_State* L );
	static int					_renewResources				( lua_State* L );

	//----------------------------------------------------------------//
	void						OnGlobalsFinalize			();
	void						OnGlobalsInitialize			();

	//----------------------------------------------------------------//
	MOAIAbstractShader*				MOAIAbstractGfxMgr_AffirmShader				( MOAILuaState& state, int idx ) const;
	MOAIAbstractTexture*			MOAIAbstractGfxMgr_AffirmTexture			( MOAILuaState& state, int idx ) const;
	MOAIGfxMgrGL&					MOAIGfxMgrGLComponents_GetGfxMgrGL			();
	MOAIGfxMgrGL_GPUCacheGL&		MOAIGfxMgrGLComponents_GetGPUCacheGL		();
	MOAIGfxMgrGL_PipelineClerkGL&	MOAIGfxMgrGLComponents_GetPipelineClerkGL	();
	MOAIGfxMgrGL_ResourceClerkGL&	MOAIGfxMgrGLComponents_GetResourceClerkGL	();
	MOAIGfxMgrGL_VertexCacheGL&		MOAIGfxMgrGLComponents_GetVertexCacheGL		();

public:

	DECL_LUA_SINGLETON ( MOAIGfxMgrGL )

	GET ( size_t, TextureMemoryUsage, mTextureMemoryUsage )
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
	
	ZLMatrix4x4				GetNormToWndMtx				();
	ZLMatrix4x4				GetNormToWndMtx				( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWorldToWndMtx			();
	ZLMatrix4x4				GetWorldToWndMtx			( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToNormMtx				();
	ZLMatrix4x4				GetWndToNormMtx				( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToWorldMtx			();
	ZLMatrix4x4				GetWndToWorldMtx			( const ZLRect& wndRect );
	
	void					PopState					();
	void					PushState					();
	
	void					RegisterLuaClass			( MOAILuaState& state );
	
	void					ReportTextureAlloc 			( cc8* name, size_t size );
	void					ReportTextureFree 			( cc8* name, size_t size );
	
	void					ResetDrawCount				();
	
	void					SetBufferScale				( float scale );
	void					SetBufferSize				( u32 width, u32 height );
	
							MOAIGfxMgrGL					();
							~MOAIGfxMgrGL					();
	
	//----------------------------------------------------------------//
	// disambiguate
	static MOAIGfxMgrGL& Get () {
		return ZLContextClass < MOAIGfxMgrGL >::Get ();
	}
};

#endif
