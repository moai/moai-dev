// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATECACHE_H
#define	MOAIGFXSTATECACHE_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIGfxPipelineClerk.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIIndexBuffer;
class MOAIShader;
class MOAIShaderProgram;
class MOAITextureBase;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexBufferWithFormat;
class MOAIVertexFormat;

//================================================================//
// MOAIVertexBufferWithFormat
//================================================================//
class MOAIVertexBufferWithFormat {
private:
	
	ZLSharedConstBuffer*						mBoundVtxBuffer;
	bool										mIsBound;

	friend class MOAIGfxStateCache;

public:

	MOAILuaSharedPtr < MOAIVertexBuffer >		mBuffer;
	MOAILuaSharedPtr < MOAIVertexFormat >		mFormat;
	
	//----------------------------------------------------------------//
				MOAIVertexBufferWithFormat		();
				~MOAIVertexBufferWithFormat		();
	void		SetBufferAndFormat				( MOAILuaObject& owner, MOAIVertexBuffer* buffer, MOAIVertexFormat* format );
	
};

//================================================================//
// MOAIGfxStateCacheClient
//================================================================//
class MOAIGfxStateCacheClient {
protected:

	friend class MOAIGfxStateCache;

	//----------------------------------------------------------------//
						MOAIGfxStateCacheClient				();
	virtual				~MOAIGfxStateCacheClient			();
	virtual void		OnGfxStateWillChange				();
};

//================================================================//
// MOAIGfxState
//================================================================//
class MOAIGfxState {
protected:

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	friend class MOAIGfxStateCache;

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	ZLStrongPtr < MOAIShader >				mShader;

	ZLStrongPtr < MOAIFrameBuffer >			mFrameBuffer;
	ZLStrongPtr < MOAIIndexBuffer >			mIdxBuffer;
	ZLStrongPtr < MOAIVertexArray >			mVtxArray;
	ZLStrongPtr < MOAIVertexBuffer >		mVtxBuffer;
	ZLStrongPtr < MOAIVertexFormat >		mVtxFormat;

	ZLRect									mViewRect;
	
	ZLLeanArray < ZLStrongPtr < MOAITextureBase > >		mTextureUnits;
	
	//----------------------------------------------------------------//
					MOAIGfxState			();
					~MOAIGfxState			();
};

//================================================================//
// MOAIGfxStateCache
//================================================================//
class MOAIGfxStateCache :
	public virtual MOAILuaObject {
protected:

	static const u32 MAX_TEXTURE_UNITS = 32; // enough? will need more flags below if not.

	friend class MOAIGfxVertexCache;
	friend class MOAIVertexArray;

	friend class MOAIGfxMgr; // TODO: this is only here so MOAIGfxMgr can call ApplyStateChanges in Clear (). move Clear () here.

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
	
	u32										mDirtyFlags;
	u32										mTextureDirtyFlags;
	u32										mMaxTextureUnits;
	
	u32										mApplyingStateChanges;

	MOAIGfxState*							mCurrentState;
	MOAIGfxState							mActiveState;
	MOAIGfxState							mPendingState;

	// don't think these need to be lua shared pointers...
	MOAILuaSharedPtr < MOAIFrameBuffer >	mDefaultFrameBuffer;
	MOAILuaSharedPtr < MOAITextureBase >	mDefaultTexture;

	ZLSharedConstBuffer*					mBoundIdxBuffer;
	ZLSharedConstBuffer*					mBoundVtxBuffer;

	MOAIGfxStateCacheClient*				mClient;

	//----------------------------------------------------------------//
	void			ApplyStateChange				( u32 stateID );
	void			ApplyStateChanges				();
	void			BindVertexBufferWithFormat		( MOAIVertexBufferWithFormat& buffer, bool useVAOs );
	void			FlushBlendMode					( bool blendEnabled, const MOAIBlendMode& blendMode );
	void			FlushCullFunc					( int cullFunc );
	void			FlushDepthFunc					( int depthFunc );
	void			FlushDepthMask					( bool depthMask );
	void			FlushFrameBuffer				( MOAIFrameBuffer* frameBuffer );
	void			FlushIndexBuffer				( MOAIIndexBuffer* buffer );
	void			FlushPenWidth					( float penWidth );
	void			FlushScissorRect				( bool scissorEnabled, ZLRect rect );
	void			FlushShader						( MOAIShader* shader );
	void			FlushTexture					( u32 textureUnit, MOAITextureBase* texture );
	void			FlushVertexArray				( MOAIVertexArray* vtxArray );
	void			FlushVertexBuffer				( MOAIVertexBuffer* buffer ); // must be called *after* BindVertexFormat
	void			FlushVertexFormat				( MOAIVertexFormat* vtxFormat );
	void			FlushViewRect					( ZLRect rect );
	void			ForceIndexBuffer				( MOAIIndexBuffer* buffer );
	void			ForceVertexBuffer				( MOAIVertexBuffer* buffer );
	void			ResumeChanges					();
	void			SuspendChanges					();
	void			UnbindVertexBufferWithFormat	( MOAIVertexBufferWithFormat& buffer );

public:
	
	GET ( MOAIBlendMode, BlendMode, mCurrentState->mBlendMode )
	GET ( bool, DepthMask, mCurrentState->mDepthMask )
	GET ( MOAIFrameBuffer*, CurrentFrameBuffer, mCurrentState->mFrameBuffer )
	GET ( MOAIShader*, CurrentShader, mCurrentState->mShader )
	GET ( MOAIVertexFormat*, CurrentVtxFormat, mCurrentState->mVtxFormat )
	GET ( const ZLRect&, ViewRect, mCurrentState->mViewRect )
	GET ( MOAIFrameBuffer*, DefaultFrameBuffer, mDefaultFrameBuffer )
	GET ( MOAITextureBase*, DefaultTexture, mDefaultTexture )
	
	//----------------------------------------------------------------//
	size_t			CountTextureUnits			();
	
	void			DrawPrims					( u32 primType, u32 base, u32 count );
	
	u32				GetBufferHeight				() const;
	u32				GetBufferWidth				() const;
		
	float			GetViewHeight				() const;
	float			GetViewWidth				() const;
	
	void			InitTextureUnits			( size_t nTextureUnits );
	
					MOAIGfxStateCache			();
	virtual			~MOAIGfxStateCache			();

	void			ResetState					();

	void			SetBlendMode				();
	void			SetBlendMode				( const MOAIBlendMode& blendMode );
	void			SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );

	void			SetClient					();
	void			SetClient					( MOAIGfxStateCacheClient* client );

	void			SetCullFunc					();
	void			SetCullFunc					( int cullFunc );
	
	void			SetDefaultFrameBuffer		( MOAILuaObject& owner, MOAIFrameBuffer* frameBuffer );
	void			SetDefaultTexture			( MOAILuaObject& owner, MOAITextureBase* texture );
	
	void			SetDepthFunc				();
	void			SetDepthFunc				( int depthFunc );
	void			SetDepthMask				( bool depthMask );
	
	bool			SetFrameBuffer				( MOAIFrameBuffer* frameBuffer = 0 );
	bool			SetIndexBuffer				( MOAIIndexBuffer* buffer = 0 );
		
	void			SetPenWidth					( float penWidth );
	
	void			SetScissorRect				();
	void			SetScissorRect				( ZLRect rect );
	
	bool			SetShader					( MOAIShaderMgr::Preset preset );
	bool			SetShader					( MOAIShader* shader = 0 );
	bool			SetTexture					( MOAITextureBase* texture = 0, u32 textureUnit = 0 );
	
	bool			SetVertexArray				( MOAIVertexArray* vtxArray = 0 );
	bool			SetVertexBuffer				( MOAIVertexBuffer* buffer = 0 ); // must be called *after* BindVertexFormat
	void			SetVertexFormat				( MOAIVertexFormatMgr::Preset preset );
	void			SetVertexFormat				( MOAIVertexFormat* format = 0 );
	
	void			SetViewRect					();
	void			SetViewRect					( ZLRect rect );
	
	void			UnbindAll					();
	
	//----------------------------------------------------------------//
	inline void GfxStateWillChange () {
		
		if ( this->mClient ) {
			MOAIGfxStateCacheClient* client = this->mClient;
			this->mClient = 0;
			client->OnGfxStateWillChange ();
		}
	}
};

#endif
