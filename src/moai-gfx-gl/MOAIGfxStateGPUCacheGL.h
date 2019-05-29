// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATEGPUCACHEGL_H
#define	MOAIGFXSTATEGPUCACHEGL_H

#include <moai-gfx-gl/MOAIAbstractGfxMgrComponentsGL.h>

class MOAIFrameBufferGL;
class MOAIIndexBufferGL;
class MOAIShaderGL;
class MOAIShaderProgramGL;
class MOAIVertexArrayGL;
class MOAIVertexBufferGL;
class MOAIVertexFormatGL;
class MOAIAbstractGfxResourceGL;
class MOAIVertexBufferWithFormatGL;
class MOAITextureBaseGL;

//================================================================//
// MOAIGfxStateGPUCacheFrameGL
//================================================================//
class MOAIGfxStateGPUCacheFrameGL {
protected:

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	friend class MOAIGfxStateGPUCacheGL;

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	ZLStrongPtr < MOAIShaderGL >				mShader;

	ZLStrongPtr < MOAIFrameBufferGL >			mFrameBuffer;
	ZLStrongPtr < MOAIIndexBufferGL >			mIdxBuffer;
	ZLStrongPtr < MOAIVertexArrayGL >			mVtxArray;
	ZLStrongPtr < MOAIVertexBufferGL >			mVtxBuffer;
	ZLStrongPtr < MOAIVertexFormatGL >			mVtxFormat;

	ZLRect									mViewRect;
	
	ZLLeanArray < ZLStrongPtr < MOAITextureBaseGL > >		mTextureUnits;
	
	//----------------------------------------------------------------//
	void			Clear							();
					MOAIGfxStateGPUCacheFrameGL		();
					~MOAIGfxStateGPUCacheFrameGL		();
};

//================================================================//
// MOAIGfxStateGPUCacheGL
//================================================================//
class MOAIGfxStateGPUCacheGL :
	virtual public MOAIAbstractGfxMgrComponentsGL,
	virtual public MOAIAbstractGfxStateGPUCache {
protected:

	static const u32 MAX_TEXTURE_UNITS = 32; // enough? will need more flags below if not.

	friend class MOAIVertexCacheGL;
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

	MOAIGfxStateGPUCacheFrameGL*				mCurrentState;
	MOAIGfxStateGPUCacheFrameGL				mActiveState;
	MOAIGfxStateGPUCacheFrameGL				mPendingState;

	// don't think these need to be lua shared pointers...
	ZLStrongPtr < MOAIFrameBufferGL >			mDefaultFrameBuffer;
	ZLStrongPtr < MOAITextureBaseGL >			mDefaultTexture;

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
	void			FlushTexture					( ZLIndex textureUnit, MOAITextureBaseGL* texture );
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
	void					ZLAbstractGPU_ClearSurface				(); // takes zgl clear flags
	size_t					ZLAbstractGPU_CountTextureUnits			();
	void					ZLAbstractGPU_DrawPrims					( u32 primType, u32 base, u32 count );
	MOAIBlendMode			ZLAbstractGPU_GetBlendMode				() const;
	u32						ZLAbstractGPU_GetBufferHeight			() const;
	u32						ZLAbstractGPU_GetBufferWidth			() const;
	MOAIAbstractFrameBuffer*		ZLAbstractGPU_GetCurrentFrameBuffer		();
	MOAIAbstractShader*		ZLAbstractGPU_GetCurrentShader			();
	MOAIAbstractVertexFormat*		ZLAbstractGPU_GetCurrentVtxFormat		();
	MOAIAbstractFrameBuffer*		ZLAbstractGPU_GetDefaultFrameBuffer		();
	MOAIAbstractTexture*	ZLAbstractGPU_GetDefaultTexture			();
	bool					ZLAbstractGPU_GetDepthMask				() const;
	float					ZLAbstractGPU_GetViewHeight				() const;
	ZLRect					ZLAbstractGPU_GetViewRect				() const;
	float					ZLAbstractGPU_GetViewWidth				() const;
	void					ZLAbstractGPU_ResetGPUState				();
	void					ZLAbstractGPU_SetBlendMode				();
	void					ZLAbstractGPU_SetBlendMode				( const MOAIBlendMode& blendMode );
	void					ZLAbstractGPU_SetBlendMode				( int srcFactor, int dstFactor, int equation );
	void					ZLAbstractGPU_SetCullFunc				();
	void					ZLAbstractGPU_SetCullFunc				( int cullFunc );
	void					ZLAbstractGPU_SetDefaultFrameBuffer		( MOAIAbstractFrameBuffer* frameBuffer );
	void					ZLAbstractGPU_SetDefaultTexture			( MOAIAbstractTexture* texture );
	void					ZLAbstractGPU_SetDepthFunc				();
	void					ZLAbstractGPU_SetDepthFunc				( int depthFunc );
	void					ZLAbstractGPU_SetDepthMask				( bool depthMask );
	bool					ZLAbstractGPU_SetFrameBuffer			( MOAIAbstractFrameBuffer* frameBuffer );
	bool					ZLAbstractGPU_SetIndexBuffer			( MOAIAbstractIndexBuffer* buffer );
	void					ZLAbstractGPU_SetPenWidth				( float penWidth );
	void					ZLAbstractGPU_SetScissorRect			();
	void					ZLAbstractGPU_SetScissorRect			( ZLRect rect );
	bool					ZLAbstractGPU_SetShader					( MOAIAbstractShader* shader );
	bool					ZLAbstractGPU_SetTexture				( MOAIAbstractTexture* texture, ZLIndex textureUnit );
	bool					ZLAbstractGPU_SetVertexArray			( MOAIAbstractVertexArray* vtxArray );
	bool					ZLAbstractGPU_SetVertexBuffer			( MOAIAbstractVertexBuffer* buffer );
	void					ZLAbstractGPU_SetVertexFormat			( MOAIAbstractVertexFormat* format );
	void					ZLAbstractGPU_SetViewRect				();
	void					ZLAbstractGPU_SetViewRect				( ZLRect rect );
	void					ZLAbstractGPU_UnbindAll					();

public:
	
	//----------------------------------------------------------------//
						MOAIGfxStateGPUCacheGL					();
	virtual				~MOAIGfxStateGPUCacheGL					();
};

#endif
