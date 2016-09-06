// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATECACHE_H
#define	MOAIGFXSTATECACHE_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIGfxPipelineClerk.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

class MOAICamera;
class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIIndexBuffer;
class MOAIMultiTexture;
class MOAIShader;
class MOAIShaderProgram;
class MOAITexture;
class MOAISingleTexture;
class MOAITextureBase;
class MOAIVertexArray;
class MOAIVertexBuffer;
class MOAIVertexFormat;
class MOAIViewport;

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

	friend class MOAIGfxStateCache;

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	MOAISharedPtr < MOAIShader >			mShader;
	MOAISharedPtr < MOAIShaderProgram >		mShaderProgram;

	MOAISharedPtr < MOAIFrameBuffer >		mFrameBuffer;
	MOAISharedPtr < MOAIIndexBuffer >		mIdxBuffer;
	MOAISharedPtr < MOAITextureBase >		mTextureSet;
	MOAISharedPtr < MOAIVertexArray >		mVtxArray;
	MOAISharedPtr < MOAIVertexBuffer >		mVtxBuffer;
	MOAISharedPtr < MOAIVertexFormat >		mVtxFormat;

	// TODO: multithread will impact caching of buffer behavior as gfx.Copy () may produce a different result each time it is called

	ZLRect									mViewRect;
	
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

	friend class MOAIGfxVertexCache;

	// right now we just test every flag for an update in a for loop.
	// these flags ordered roughly by (guessed) call frequency just so we can bail out of the
	// state update loop early. if the update loop is ever a bottleneck (or if this list grows
	// a lot) then we'll have to look at a data structure for scheduling updates.
	// note that some of these also have to be done in a certain order.
	enum {
		TEXTURE					= 1 << 0,
		SHADER					= 1 << 1,
		VERTEX_FORMAT			= 1 << 2,	// must come before vertex buffer
		VERTEX_BUFFER			= 1 << 3,
		INDEX_BUFFER			= 1 << 4,
		VERTEX_ARRAY			= 1 << 5,
		DEPTH_MODE				= 1 << 6,
		BLEND_MODE				= 1 << 7,
		PEN_WIDTH				= 1 << 8,
		CULL_FUNC				= 1 << 9,
		FRAME_BUFFER			= 1 << 10,
		VIEW_RECT				= 1 << 11,	// must come *after* frame buffer
		SCISSOR_RECT			= 1 << 12,	// must come *after* frame buffer
		END_STATE_FLAGS			= 1 << 13,
	};
	
	u32										mDirtyFlags;
	
	u32										mApplyingStateChanges;

	MOAIGfxState*							mCurrentState;
	MOAIGfxState							mActiveState;
	MOAIGfxState							mPendingState;

	// don't think these need to be lua shared pointers...
	MOAILuaSharedPtr < MOAIFrameBuffer >	mDefaultFrameBuffer;
	MOAILuaSharedPtr < MOAITexture >		mDefaultTexture;

	ZLLeanArray < MOAISharedPtr < MOAISingleTexture > >		mTextureUnits;
	u32														mActiveTextures;

	ZLSharedConstBuffer*					mBoundIdxBuffer;
	ZLSharedConstBuffer*					mBoundVtxBuffer;

	MOAIGfxStateCacheClient*				mClient;

	//----------------------------------------------------------------//
	void			ApplyStateChange			( u32 stateID );
	void			ApplyStateChanges			();
	void			ResumeChanges				();
	void			SuspendChanges				();

public:
	
	GET ( MOAIBlendMode, BlendMode, this->mCurrentState->mBlendMode )
	GET ( bool, DepthMask, this->mCurrentState->mDepthMask )
	GET ( MOAIFrameBuffer*, CurrentFrameBuffer, this->mCurrentState->mFrameBuffer )
	GET ( MOAIVertexFormat*, CurrentVtxFormat, this->mCurrentState->mVtxFormat )
	GET ( const ZLRect&, ViewRect, this->mCurrentState->mViewRect )
	GET ( MOAIFrameBuffer*, DefaultFrameBuffer, mDefaultFrameBuffer )
	GET ( MOAITexture*, DefaultTexture, mDefaultTexture )
	
	//----------------------------------------------------------------//
	bool			BindFrameBuffer				( MOAIFrameBuffer* frameBuffer = 0 );
	bool			BindIndexBuffer				( MOAIIndexBuffer* buffer = 0 );
	bool			BindShader					( MOAIShaderMgr::Preset preset );
	bool			BindShader					( MOAIShader* shader = 0 );
	bool			BindTexture					( MOAITextureBase* textureSet = 0 );
	bool			BindVertexArray				( MOAIVertexArray* vtxArray = 0 );
	bool			BindVertexBuffer			( MOAIVertexBuffer* buffer = 0 ); // must be called *after* BindVertexFormat
	
	size_t			CountTextureUnits			();
	
	void			DrawPrims					( u32 primType, u32 base, u32 count );
	
	u32				GetBufferHeight				() const;
	u32				GetBufferWidth				() const;
	
	//float			GetDeviceScale				();
	u32				GetShaderGlobalsMask		();
	
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
	void			SetDefaultTexture			( MOAILuaObject& owner, MOAITexture* texture );
	
	void			SetDepthFunc				();
	void			SetDepthFunc				( int depthFunc );
	void			SetDepthMask				( bool depthMask );
	
	void			SetPenWidth					( float penWidth );
	
	void			SetScissorRect				();
	void			SetScissorRect				( ZLRect rect );
	
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
