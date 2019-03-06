// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLGFXMGR_H
#define	ZLGFXMGR_H

#include <zl-gfx/ZLAbstractGfxMgr.h>
#include <zl-gfx/ZLGfxPipelineClerk.h>
#include <zl-gfx/ZLGfxResourceClerk.h>
#include <zl-gfx/ZLGfxStateCPUCache.h>
#include <zl-gfx/ZLGfxStateGPUCache.h>
#include <zl-gfx/ZLGfxStateVertexCache.h>

//================================================================//
// ZLGfxStateFrame
//================================================================//
class ZLGfxStateFrame :
	public ZLGfxStateCPUCacheFrame,
	public ZLGfxStateGPUCacheFrame {
protected:
};

//================================================================//
// ZLGfxMgr
//================================================================//
class ZLGfxMgr :
	virtual public ZLAbstractGfxMgr,
	virtual public ZLGfxPipelineClerk,
	virtual public ZLGfxResourceClerk,
	virtual public ZLGfxStateGPUCache,
	virtual public ZLGfxStateVertexCache {
protected:

	bool								mHasContext;

	bool								mIsFramebufferSupported;
	bool								mIsOpenGLES;

	u32									mMajorVersion;
	u32									mMinorVersion;
	
	size_t								mTextureMemoryUsage;
	u32									mMaxTextureSize;

	u32									mRenderCounter;

	ZLLeanArray < ZLGfxStateFrame* >	mStateStack;
	ZLIndex								mStateStackTop;

	//----------------------------------------------------------------//
	ZLGfxMgr& 					ZLAbstractGfxStateCache_GetGfxMgr 					();
	ZLGfxPipelineClerk&			ZLAbstractGfxStateCache_GetGfxPipelineClerk			();
	ZLGfxResourceClerk&			ZLAbstractGfxStateCache_GetGfxResourceClerk			();
	ZLGfxStateCPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheCPU			();
	ZLGfxStateGPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheGPU			();
	ZLGfxStateVertexCache&		ZLAbstractGfxStateCache_GetGfxVertexCache			();

public:

	GET ( size_t, TextureMemoryUsage, mTextureMemoryUsage )
	GET ( u32, MaxTextureSize, mMaxTextureSize )
	GET ( bool, HasContext, mHasContext )
	
	GET_BOOL ( IsOpenGLES, mIsOpenGLES )
	GET_BOOL ( IsFramebufferSupported, mIsFramebufferSupported )

	GET ( u32, RenderCounter, mRenderCounter );

	//----------------------------------------------------------------//
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
	
	void					ReportTextureAlloc 			( cc8* name, size_t size );
	void					ReportTextureFree 			( cc8* name, size_t size );
	
	void					ResetDrawCount				();
	
	void					SetBufferScale				( float scale );
	void					SetBufferSize				( u32 width, u32 height );
	
							ZLGfxMgr					();
							~ZLGfxMgr					();
};

#endif
