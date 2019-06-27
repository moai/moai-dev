// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_GPUCACHEGL_H
#define	MOAIGFXMGRGL_GPUCACHEGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>

class MOAIFrameBufferGL;
class MOAIIndexBufferGL;
class MOAIShaderGL;
class MOAIShaderProgramGL;
class MOAIVertexArrayGL;
class MOAIVertexBufferGL;
class MOAIVertexFormatGL;
class MOAIGfxResourceGL;
class MOAIVertexBufferWithFormatGL;
class MOAITextureGL;

//================================================================//
// MOAIGfxStateGPUCacheFrameGL
//================================================================//
class MOAIGfxStateGPUCacheFrameGL {
protected:

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	friend class MOAIGfxMgrGL_GPUCacheGL;

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	ZLStrongPtr < MOAIShaderGL >			mShader;

	ZLStrongPtr < MOAIFrameBufferGL >		mFrameBuffer;
	ZLStrongPtr < MOAIIndexBufferGL >		mIdxBuffer;
	ZLStrongPtr < MOAIVertexArrayGL >		mVtxArray;
	ZLStrongPtr < MOAIVertexBufferGL >		mVtxBuffer;
	ZLStrongPtr < MOAIVertexFormatGL >		mVtxFormat;

	ZLRect									mViewRect;
	
	ZLLeanArray < ZLStrongPtr < MOAITextureGL > >		mTextureUnits;
	
	//----------------------------------------------------------------//
	void			Clear							();
					MOAIGfxStateGPUCacheFrameGL		();
					~MOAIGfxStateGPUCacheFrameGL	();
};

//================================================================//
// MOAIGfxMgrGL_GPUCacheGL
//================================================================//
class MOAIGfxMgrGL_GPUCacheGL :
	public virtual MOAIGfxMgrGLComponents,
	public virtual MOAIGfxMgr_GPUCache {
protected:

	static const u32 MAX_TEXTURE_UNITS = 32; // enough? will need more flags below if not.

	friend class MOAIGfxMgrGL_VertexCacheGL;
	friend class MOAIVertexArrayGL;

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

	MOAIGfxStateGPUCacheFrameGL*			mCurrentState;
	MOAIGfxStateGPUCacheFrameGL				mActiveState;
	MOAIGfxStateGPUCacheFrameGL				mPendingState;

	// don't think these need to be lua shared pointers...
	ZLStrongPtr < MOAIFrameBufferGL >		mDefaultFrameBuffer;
	ZLStrongPtr < MOAITextureGL >			mDefaultTexture;

	ZLSharedConstBuffer*					mBoundIdxBuffer;
	ZLSharedConstBuffer*					mBoundVtxBuffer;

	//----------------------------------------------------------------//
	void			Clear							();
	void			ApplyStateChange				( u32 stateID );
	void			ApplyStateChanges				();
	void			FlushBlendMode					( bool blendEnabled, const MOAIBlendMode& blendMode );
	void			FlushCullFunc					( int cullFunc );
	void			FlushDepthFunc					( int depthFunc );
	void			FlushDepthMask					( bool depthMask );
	void			FlushFrameBuffer				( MOAIFrameBufferGL* frameBuffer );
	void			FlushIndexBuffer				( MOAIIndexBufferGL* buffer );
	void			FlushPenWidth					( float penWidth );
	void			FlushScissorRect				( bool scissorEnabled, ZLRect rect );
	void			FlushShader						( MOAIShaderGL* shader );
	void			FlushTexture					( ZLIndex textureUnit, MOAITextureGL* texture );
	void			FlushVertexArray				( MOAIVertexArrayGL* vtxArray );
	void			FlushVertexBuffer				( MOAIVertexBufferGL* buffer );
	void			FlushVertexFormat				( MOAIVertexFormatGL* vtxFormat );
	void			FlushViewRect					( ZLRect rect );
	void			ForceIndexBuffer				( MOAIIndexBufferGL* buffer );
	void			ForceVertexBuffer				( MOAIVertexBufferGL* buffer );
	void			GfxStateWillChange				();
	void			InitTextureUnits				( size_t nTextureUnits );
	void			RecalculateDirtyFlags			();
	void			RestoreGPUState					( const MOAIGfxStateGPUCacheFrameGL& frame );
	void			ResumeChanges					();
	void			StoreGPUState					( MOAIGfxStateGPUCacheFrameGL& frame ) const;
	void			SuspendChanges					();

	//----------------------------------------------------------------//
	void					MOAIGfxMgr_GPUCache_ClearSurface				(); // takes zgl clear flags
	size_t					MOAIGfxMgr_GPUCache_CountTextureUnits			();
	void					MOAIGfxMgr_GPUCache_DrawPrims					( u32 primType, u32 base, u32 count );
	MOAIBlendMode			MOAIGfxMgr_GPUCache_GetBlendMode				() const;
	u32						MOAIGfxMgr_GPUCache_GetBufferHeight				() const;
	u32						MOAIGfxMgr_GPUCache_GetBufferWidth				() const;
	MOAIFrameBuffer*		MOAIGfxMgr_GPUCache_GetCurrentFrameBuffer		();
	MOAIShader*				MOAIGfxMgr_GPUCache_GetCurrentShader			();
	MOAIVertexFormat*		MOAIGfxMgr_GPUCache_GetCurrentVtxFormat			();
	MOAIFrameBuffer*		MOAIGfxMgr_GPUCache_GetDefaultFrameBuffer		();
	MOAITexture*			MOAIGfxMgr_GPUCache_GetDefaultTexture			();
	bool					MOAIGfxMgr_GPUCache_GetDepthMask				() const;
	float					MOAIGfxMgr_GPUCache_GetViewHeight				() const;
	ZLRect					MOAIGfxMgr_GPUCache_GetViewRect					() const;
	float					MOAIGfxMgr_GPUCache_GetViewWidth				() const;
	void					MOAIGfxMgr_GPUCache_ResetGPUState				();
	void					MOAIGfxMgr_GPUCache_SetBlendMode				();
	void					MOAIGfxMgr_GPUCache_SetBlendMode				( const MOAIBlendMode& blendMode );
	void					MOAIGfxMgr_GPUCache_SetBlendMode				( int srcFactor, int dstFactor, int equation );
	void					MOAIGfxMgr_GPUCache_SetCullFunc					();
	void					MOAIGfxMgr_GPUCache_SetCullFunc					( int cullFunc );
	void					MOAIGfxMgr_GPUCache_SetDefaultFrameBuffer		( MOAIFrameBuffer* frameBuffer );
	void					MOAIGfxMgr_GPUCache_SetDefaultTexture			( MOAITexture* texture );
	void					MOAIGfxMgr_GPUCache_SetDepthFunc				();
	void					MOAIGfxMgr_GPUCache_SetDepthFunc				( int depthFunc );
	void					MOAIGfxMgr_GPUCache_SetDepthMask				( bool depthMask );
	bool					MOAIGfxMgr_GPUCache_SetFrameBuffer				( MOAIFrameBuffer* frameBuffer );
	bool					MOAIGfxMgr_GPUCache_SetIndexBuffer				( MOAIIndexBuffer* buffer );
	void					MOAIGfxMgr_GPUCache_SetPenWidth					( float penWidth );
	void					MOAIGfxMgr_GPUCache_SetScissorRect				();
	void					MOAIGfxMgr_GPUCache_SetScissorRect				( ZLRect rect );
	bool					MOAIGfxMgr_GPUCache_SetShader					( MOAIShader* shader );
	bool					MOAIGfxMgr_GPUCache_SetTexture					( MOAITexture* texture, ZLIndex textureUnit );
	bool					MOAIGfxMgr_GPUCache_SetVertexArray				( MOAIVertexArray* vtxArray );
	bool					MOAIGfxMgr_GPUCache_SetVertexBuffer				( MOAIVertexBuffer* buffer );
	void					MOAIGfxMgr_GPUCache_SetVertexFormat				( MOAIVertexFormat* format );
	void					MOAIGfxMgr_GPUCache_SetViewRect					();
	void					MOAIGfxMgr_GPUCache_SetViewRect					( ZLRect rect );
	void					MOAIGfxMgr_GPUCache_UnbindAll					();

public:
	
	//----------------------------------------------------------------//
						MOAIGfxMgrGL_GPUCacheGL					();
	virtual				~MOAIGfxMgrGL_GPUCacheGL				();
};

#endif
