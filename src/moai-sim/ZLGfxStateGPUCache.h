// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLGFXSTATEGPUCACHE_H
#define	ZLGFXSTATEGPUCACHE_H

#include <moai-sim/ZLAbstractGfxStateCache.h>
#include <moai-sim/ZLBlendMode.h>
#include <moai-sim/ZLGfxPipelineClerk.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

class ZLFrameBuffer;
class ZLIndexBuffer;
class MOAIShader;
class MOAIShaderProgram;
class ZLVertexArray;
class ZLVertexBuffer;
class MOAIVertexFormat;
class ZLAbstractGfxResource;
class ZLVertexBufferWithFormat;
class ZLTexture;

//================================================================//
// ZLGfxStateGPUCacheFrame
//================================================================//
class ZLGfxStateGPUCacheFrame {
protected:

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	friend class ZLGfxStateGPUCache;

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	ZLBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	ZLStrongPtr < MOAIShader >				mShader;

	ZLStrongPtr < ZLFrameBuffer >			mFrameBuffer;
	ZLStrongPtr < ZLIndexBuffer >			mIdxBuffer;
	ZLStrongPtr < ZLVertexArray >			mVtxArray;
	ZLStrongPtr < ZLVertexBuffer >			mVtxBuffer;
	ZLStrongPtr < MOAIVertexFormat >		mVtxFormat;

	ZLRect									mViewRect;
	
	ZLLeanArray < ZLStrongPtr < ZLTexture > >		mTextureUnits;
	
	//----------------------------------------------------------------//
					ZLGfxStateGPUCacheFrame			();
					~ZLGfxStateGPUCacheFrame		();
};

//================================================================//
// ZLGfxStateGPUCache
//================================================================//
class ZLGfxStateGPUCache :
	virtual public ZLAbstractGfxStateCache {
protected:

	static const u32 MAX_TEXTURE_UNITS = 32; // enough? will need more flags below if not.

	friend class ZLGfxStateVertexCache;
	friend class ZLVertexArray;

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

	ZLGfxStateGPUCacheFrame*				mCurrentState;
	ZLGfxStateGPUCacheFrame					mActiveState;
	ZLGfxStateGPUCacheFrame					mPendingState;

	// don't think these need to be lua shared pointers...
	ZLStrongPtr < ZLFrameBuffer >			mDefaultFrameBuffer;
	ZLStrongPtr < ZLTexture >				mDefaultTexture;

	ZLSharedConstBuffer*					mBoundIdxBuffer;
	ZLSharedConstBuffer*					mBoundVtxBuffer;

	//----------------------------------------------------------------//
	void			ApplyStateChange				( u32 stateID );
	void			ApplyStateChanges				();
	void			FlushBlendMode					( bool blendEnabled, const ZLBlendMode& blendMode );
	void			FlushCullFunc					( int cullFunc );
	void			FlushDepthFunc					( int depthFunc );
	void			FlushDepthMask					( bool depthMask );
	void			FlushFrameBuffer				( ZLFrameBuffer* frameBuffer );
	void			FlushIndexBuffer				( ZLIndexBuffer* buffer );
	void			FlushPenWidth					( float penWidth );
	void			FlushScissorRect				( bool scissorEnabled, ZLRect rect );
	void			FlushShader						( MOAIShader* shader );
	void			FlushTexture					( ZLIndex textureUnit, ZLTexture* texture );
	void			FlushVertexArray				( ZLVertexArray* vtxArray );
	void			FlushVertexBuffer				( ZLVertexBuffer* buffer );
	void			FlushVertexFormat				( MOAIVertexFormat* vtxFormat );
	void			FlushViewRect					( ZLRect rect );
	void			ForceIndexBuffer				( ZLIndexBuffer* buffer );
	void			ForceVertexBuffer				( ZLVertexBuffer* buffer );
	void			GfxStateWillChange				();
	void			InitTextureUnits				( size_t nTextureUnits );
	void			RecalculateDirtyFlags			();
	void			RestoreGPUState					( const ZLGfxStateGPUCacheFrame& frame );
	void			ResumeChanges					();
	void			StoreGPUState					( ZLGfxStateGPUCacheFrame& frame ) const;
	void			SuspendChanges					();

public:
	
	GET ( ZLBlendMode, BlendMode, mCurrentState->mBlendMode )
	GET ( bool, DepthMask, mCurrentState->mDepthMask )
	GET ( ZLFrameBuffer*, CurrentFrameBuffer, mCurrentState->mFrameBuffer )
	GET ( MOAIShader*, CurrentShader, mCurrentState->mShader )
	GET ( MOAIVertexFormat*, CurrentVtxFormat, mCurrentState->mVtxFormat )
	GET ( const ZLRect&, ViewRect, mCurrentState->mViewRect )
	GET ( ZLFrameBuffer*, DefaultFrameBuffer, mDefaultFrameBuffer )
	GET ( ZLTexture*, DefaultTexture, mDefaultTexture )
	
	//----------------------------------------------------------------//
	void			ClearSurface				(); // takes zgl clear flags
	
	size_t			CountTextureUnits			();
	
	void			DrawPrims					( u32 primType, u32 base, u32 count );
	
	u32				GetBufferHeight				() const;
	u32				GetBufferWidth				() const;
		
	float			GetViewHeight				() const;
	float			GetViewWidth				() const;
	
					ZLGfxStateGPUCache			();
	virtual			~ZLGfxStateGPUCache			();

	void			ResetState					();

	void			SetBlendMode				();
	void			SetBlendMode				( const ZLBlendMode& blendMode );
	void			SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );

	void			SetCullFunc					();
	void			SetCullFunc					( int cullFunc );
	
	void			SetDefaultFrameBuffer		( ZLFrameBuffer* frameBuffer );
	void			SetDefaultTexture			( ZLTexture* texture );
	
	void			SetDepthFunc				();
	void			SetDepthFunc				( int depthFunc );
	void			SetDepthMask				( bool depthMask );
	
	bool			SetFrameBuffer				( ZLFrameBuffer* frameBuffer = 0 );
	bool			SetIndexBuffer				( ZLIndexBuffer* buffer = 0 );
		
	void			SetPenWidth					( float penWidth );
	
	void			SetScissorRect				();
	void			SetScissorRect				( ZLRect rect );
	
	bool			SetShader					( MOAIShaderMgr::Preset preset );
	bool			SetShader					( MOAIShader* shader = 0 );
	bool			SetTexture					( ZLTexture* texture = 0, ZLIndex textureUnit = ZLIndexOp::ZERO );
	
	bool			SetVertexArray				( ZLVertexArray* vtxArray = 0 );
	bool			SetVertexBuffer				( ZLVertexBuffer* buffer = 0 );
	void			SetVertexFormat				( MOAIVertexFormatMgr::Preset preset );
	void			SetVertexFormat				( MOAIVertexFormat* format = 0 );
	
	void			SetViewRect					();
	void			SetViewRect					( ZLRect rect );
	
	void			UnbindAll					();
};

#endif
