// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATECACHE_H
#define	MOAIGFXSTATECACHE_H

#include <moai-sim/MOAIGfxPipelineMgr.h>
#include <moai-sim/MOAIGfxMtxCache.h>
#include <moai-sim/MOAIBlendMode.h>

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
// MOAIGfxStateCache
//================================================================//
class MOAIGfxStateCache :
	public virtual MOAILuaObject {
protected:

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	bool									mScissorEnabled;
	ZLRect									mScissorRect;
	
	MOAIShader*								mShader;
	MOAIShaderProgram*						mShaderProgram;
	
	ZLLeanArray < MOAISingleTexture* >		mTextureUnits;
	u32										mActiveTextures;

	MOAIFrameBuffer*						mCurrentFrameBuffer;
	MOAIIndexBuffer*						mCurrentIdxBuffer;
	MOAITextureBase*						mCurrentTexture;
	MOAIVertexArray*						mCurrentVtxArray;
	MOAIVertexBuffer*						mCurrentVtxBuffer;
	MOAIVertexFormat*						mCurrentVtxFormat;

	ZLRect									mViewRect;

	MOAILuaSharedPtr < MOAIFrameBuffer >	mDefaultFrameBuffer;
	MOAILuaSharedPtr < MOAITexture >		mDefaultTexture;

	//----------------------------------------------------------------//
	bool				BindShaderProgram			( MOAIShaderProgram* program ); // only called by BindShader ()
	bool				BindTexture					( u32 textureUnit, MOAISingleTexture* texture );

public:
	
	GET ( MOAIBlendMode, BlendMode, mBlendMode )
	GET ( bool, DepthMask, mDepthMask )
	GET ( MOAIFrameBuffer*, CurrentFrameBuffer, mCurrentFrameBuffer )
	GET ( const ZLRect&, ViewRect, mViewRect )
	GET ( MOAIFrameBuffer*, DefaultFrameBuffer, mDefaultFrameBuffer )
	GET ( MOAITexture*, DefaultTexture, mDefaultTexture )
	
	//----------------------------------------------------------------//
	bool			BindFrameBuffer				( MOAIFrameBuffer* frameBuffer = 0 );
	bool			BindIndexBuffer				( MOAIIndexBuffer* buffer = 0 );
	bool			BindShader					( MOAIShader* shader = 0 );
	bool			BindShader					( u32 preset );
	bool			BindTexture					( MOAITextureBase* textureSet = 0 );
	bool			BindVertexArray				( MOAIVertexArray* vtxArray = 0 );
	bool			BindVertexBuffer			( MOAIVertexBuffer* buffer = 0 );
	bool			BindVertexFormat			( MOAIVertexFormat* format = 0, bool copyBuffer = false );
	
	size_t			CountTextureUnits			();
	
	float			GetDeviceScale				();
	u32				GetHeight					() const;
	u32				GetWidth					() const;
	
	void			InitTextureUnits			( size_t nTextureUnits );
	
					MOAIGfxStateCache			();
	virtual			~MOAIGfxStateCache			();

	void			ResetState					();

	void			SetBlendMode				();
	void			SetBlendMode				( const MOAIBlendMode& blendMode );
	void			SetBlendMode				( int srcFactor, int dstFactor, int equation = ZGL_BLEND_MODE_ADD );

	void			SetCullFunc					();
	void			SetCullFunc					( int cullFunc );
	
	void			SetDepthFunc				();
	void			SetDepthFunc				( int depthFunc );
	void			SetDepthMask				( bool depthMask );
	
	void			SetPenWidth					( float penWidth );
	
	void			SetScissorRect				();
	void			SetScissorRect				( ZLRect rect );
	
	void			SetViewRect					();
	void			SetViewRect					( ZLRect rect );
	
	void			UnbindAll					();
	void			UpdateAndBindUniforms		(); // call this immediately before drawing
};

#endif
