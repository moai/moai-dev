// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLGFXSTATEGPUCACHEGL_H
#define	ZLGFXSTATEGPUCACHEGL_H

#include <zl-gfx/ZLAbstractGfxMgrGL.h>
#include <zl-gfx/ZLAbstractGPU.h>
#include <zl-gfx/ZLBlendMode.h>
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxPipelineClerkGL.h>

class ZLFrameBufferGL;
class ZLIndexBufferGL;
class ZLShaderGL;
class ZLShaderProgramGL;
class ZLVertexArrayGL;
class ZLVertexBufferGL;
class ZLVertexFormatGL;
class ZLAbstractGfxResourceGL;
class ZLVertexBufferWithFormatGL;
class ZLTextureBaseGL;

//================================================================//
// ZLGfxStateGPUCacheFrameGL
//================================================================//
class ZLGfxStateGPUCacheFrameGL {
protected:

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	friend class ZLGfxStateGPUCacheGL;

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	ZLBlendMode								mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	ZLStrongPtr < ZLShaderGL >				mShader;

	ZLStrongPtr < ZLFrameBufferGL >			mFrameBuffer;
	ZLStrongPtr < ZLIndexBufferGL >			mIdxBuffer;
	ZLStrongPtr < ZLVertexArrayGL >			mVtxArray;
	ZLStrongPtr < ZLVertexBufferGL >			mVtxBuffer;
	ZLStrongPtr < ZLVertexFormatGL >			mVtxFormat;

	ZLRect									mViewRect;
	
	ZLLeanArray < ZLStrongPtr < ZLTextureBaseGL > >		mTextureUnits;
	
	//----------------------------------------------------------------//
	void			Clear							();
					ZLGfxStateGPUCacheFrameGL		();
					~ZLGfxStateGPUCacheFrameGL		();
};

//================================================================//
// ZLGfxStateGPUCacheGL
//================================================================//
class ZLGfxStateGPUCacheGL :
	virtual public ZLAbstractGfxMgrGL,
	virtual public ZLAbstractGPU {
protected:

	static const u32 MAX_TEXTURE_UNITS = 32; // enough? will need more flags below if not.

	friend class ZLGfxStateVertexCache;
	friend class ZLVertexArrayGL;

	friend class MOAIGfxMgr;

	// right now we just test every flag for an update in a for loop.
	// these flags ordered roughly by (guessed) call frequency so we can bail out of the
	// state update loop early. if the update loop is ever a bottleneck (or if this list grows
	// a lot) then we'll have to look at a data structure for scheduling updates.
	// note that some of these also have to be done in a certain order.
	enum {
		SHADER					= 1 << 0,
		VERTEX_FORMAT			= 1 << 1, // must come *before* vertex buffer
		VERTEX_BUFFER			= 1 << 2,
		INDEX_BUFFER			= 1 << 3,
		VERTEX_ARRAY			= 1 << 4,
		DEPTH_MODE				= 1 << 5,
		BLEND_MODE				= 1 << 6,
		PEN_WIDTH				= 1 << 7,
		CULL_FUNC				= 1 << 8,
		FRAME_BUFFER			= 1 << 9,
		VIEW_RECT				= 1 << 10,	// must come *after* frame buffer
		SCISSOR_RECT			= 1 << 11,	// must come *after* frame buffer
		END_STATE_FLAGS			= 1 << 12,
	};
	
	u32										mMaxTextureUnits;
	
	u32										mDirtyFlags;
	u32										mTextureDirtyFlags;
	u32										mApplyingStateChanges;

	ZLGfxStateGPUCacheFrameGL*				mCurrentState;
	ZLGfxStateGPUCacheFrameGL				mActiveState;
	ZLGfxStateGPUCacheFrameGL				mPendingState;

	// don't think these need to be lua shared pointers...
	ZLStrongPtr < ZLFrameBufferGL >			mDefaultFrameBuffer;
	ZLStrongPtr < ZLTextureBaseGL >			mDefaultTexture;

	ZLSharedConstBuffer*					mBoundIdxBuffer;
	ZLSharedConstBuffer*					mBoundVtxBuffer;

	//----------------------------------------------------------------//
	void			Clear							();
	void			ApplyStateChange				( u32 stateID );
	void			ApplyStateChanges				();
	void			FlushBlendMode					( bool blendEnabled, const ZLBlendMode& blendMode );
	void			FlushCullFunc					( int cullFunc );
	void			FlushDepthFunc					( int depthFunc );
	void			FlushDepthMask					( bool depthMask );
	void			FlushFrameBuffer				( ZLFrameBufferGL* frameBuffer );
	void			FlushIndexBuffer				( ZLIndexBufferGL* buffer );
	void			FlushPenWidth					( float penWidth );
	void			FlushScissorRect				( bool scissorEnabled, ZLRect rect );
	void			FlushShader						( ZLShaderGL* shader );
	void			FlushTexture					( ZLIndex textureUnit, ZLTextureBaseGL* texture );
	void			FlushVertexArray				( ZLVertexArrayGL* vtxArray );
	void			FlushVertexBuffer				( ZLVertexBufferGL* buffer );
	void			FlushVertexFormat				( ZLVertexFormatGL* vtxFormat );
	void			FlushViewRect					( ZLRect rect );
	void			ForceIndexBuffer				( ZLIndexBufferGL* buffer );
	void			ForceVertexBuffer				( ZLVertexBufferGL* buffer );
	void			GfxStateWillChange				();
	void			InitTextureUnits				( size_t nTextureUnits );
	void			RecalculateDirtyFlags			();
	void			RestoreGPUState					( const ZLGfxStateGPUCacheFrameGL& frame );
	void			ResumeChanges					();
	void			StoreGPUState					( ZLGfxStateGPUCacheFrameGL& frame ) const;
	void			SuspendChanges					();

	//----------------------------------------------------------------//
	void				ZLAbstractGPU_ClearSurface				(); // takes zgl clear flags
	size_t				ZLAbstractGPU_CountTextureUnits			();
	void				ZLAbstractGPU_DrawPrims					( u32 primType, u32 base, u32 count );
	ZLBlendMode			ZLAbstractGPU_GetBlendMode				() const;
	u32					ZLAbstractGPU_GetBufferHeight			() const;
	u32					ZLAbstractGPU_GetBufferWidth			() const;
	ZLFrameBufferGL*		ZLAbstractGPU_GetCurrentFrameBuffer		();
	ZLAbstractShader*	ZLAbstractGPU_GetCurrentShader			();
	ZLVertexFormatGL*		ZLAbstractGPU_GetCurrentVtxFormat		();
	ZLFrameBufferGL*		ZLAbstractGPU_GetDefaultFrameBuffer		();
	ZLAbstractTexture*	ZLAbstractGPU_GetDefaultTexture			();
	bool				ZLAbstractGPU_GetDepthMask				() const;
	float				ZLAbstractGPU_GetViewHeight				() const;
	ZLRect				ZLAbstractGPU_GetViewRect				() const;
	float				ZLAbstractGPU_GetViewWidth				() const;
	void				ZLAbstractGPU_ResetGPUState				();
	void				ZLAbstractGPU_SetBlendMode				();
	void				ZLAbstractGPU_SetBlendMode				( const ZLBlendMode& blendMode );
	void				ZLAbstractGPU_SetBlendMode				( int srcFactor, int dstFactor, int equation );
	void				ZLAbstractGPU_SetCullFunc				();
	void				ZLAbstractGPU_SetCullFunc				( int cullFunc );
	void				ZLAbstractGPU_SetDefaultFrameBuffer		( ZLFrameBufferGL* frameBuffer );
	void				ZLAbstractGPU_SetDefaultTexture			( ZLAbstractTexture* texture );
	void				ZLAbstractGPU_SetDepthFunc				();
	void				ZLAbstractGPU_SetDepthFunc				( int depthFunc );
	void				ZLAbstractGPU_SetDepthMask				( bool depthMask );
	bool				ZLAbstractGPU_SetFrameBuffer			( ZLFrameBufferGL* frameBuffer );
	bool				ZLAbstractGPU_SetIndexBuffer			( ZLIndexBufferGL* buffer );
	void				ZLAbstractGPU_SetPenWidth				( float penWidth );
	void				ZLAbstractGPU_SetScissorRect			();
	void				ZLAbstractGPU_SetScissorRect			( ZLRect rect );
	bool				ZLAbstractGPU_SetShader					( ZLAbstractShader* shader );
	bool				ZLAbstractGPU_SetTexture				( ZLAbstractTexture* texture, ZLIndex textureUnit );
	bool				ZLAbstractGPU_SetVertexArray			( ZLVertexArrayGL* vtxArray );
	bool				ZLAbstractGPU_SetVertexBuffer			( ZLVertexBufferGL* buffer );
	void				ZLAbstractGPU_SetVertexFormat			( ZLVertexFormatGL* format );
	void				ZLAbstractGPU_SetViewRect				();
	void				ZLAbstractGPU_SetViewRect				( ZLRect rect );
	void				ZLAbstractGPU_UnbindAll					();

public:
	
	//----------------------------------------------------------------//
						ZLGfxStateGPUCacheGL						();
	virtual				~ZLGfxStateGPUCacheGL						();
};

#endif
