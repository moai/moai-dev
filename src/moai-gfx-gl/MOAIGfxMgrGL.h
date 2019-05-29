// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_H
#define	MOAIGFXMGRGL_H

#include <moai-gfx-gl/MOAIAbstractGfxMgrComponentsGL.h>
#include <moai-gfx-gl/MOAIGfxPipelineClerkGL.h>
#include <moai-gfx-gl/MOAIGfxResourceClerkGL.h>
#include <moai-gfx-gl/MOAIGfxStateGPUCacheGL.h>
#include <moai-gfx-gl/MOAIVertexCacheGL.h>

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
	virtual public MOAIAbstractGfxMgr,
	virtual public MOAIAbstractGfxMgrComponentsGL,
	virtual public MOAIGfxPipelineClerkGL,
	virtual public MOAIGfxResourceClerkGL,
	virtual public MOAIGfxStateGPUCacheGL,
	virtual public MOAIVertexCacheGL {
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
	MOAIAbstractShader*			MOAIAbstractGfxMgr_AffirmShader							( MOAILuaState& state, int idx ) const;
	MOAIAbstractTexture*			MOAIAbstractGfxMgr_AffirmTexture						( MOAILuaState& state, int idx ) const;
	MOAIGfxMgrGL&				MOAIAbstractGfxMgrComponentsGL_GetGfxMgrGL 				();
	MOAIGfxPipelineClerkGL&		MOAIAbstractGfxMgrComponentsGL_GetGfxPipelineClerkGL	();
	MOAIGfxResourceClerkGL&		MOAIAbstractGfxMgrComponentsGL_GetGfxResourceClerkGL	();
	MOAIGfxStateGPUCacheGL&		MOAIAbstractGfxMgrComponentsGL_GetGfxStateGPUCacheGL	();
	MOAIVertexCacheGL&			MOAIAbstractGfxMgrComponentsGL_GetVertexCacheGL			();

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
