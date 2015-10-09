// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICESTATECACHE_H
#define	MOAIGFXDEVICESTATECACHE_H

#include <moai-sim/MOAIGfxDeviceBase.h>
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
// MOAIGfxDeviceStateCache
//================================================================//
class MOAIGfxDeviceStateCache :
	public MOAIGfxDeviceBase {
protected:

	int										mCullFunc;
	
	int										mDepthFunc;
	bool									mDepthMask;

	MOAIBlendMode							mBlendMode;
	bool									mBlendEnabled;

	float									mPenWidth;

	ZLRect									mScissorRect;
	MOAIShaderProgram*						mShaderProgram;
	
	ZLLeanArray < MOAISingleTexture* >		mTextureUnits;
	u32										mActiveTextures;

	MOAIFrameBuffer*						mCurrentFrameBuffer;
	MOAIIndexBuffer*						mCurrentIdxBuffer;
	MOAITextureBase*						mCurrentTexture;
	MOAIVertexArray*						mCurrentVtxArray;
	MOAIVertexBuffer*						mCurrentVtxBuffer;
	MOAIVertexFormat*						mCurrentVtxFormat;

	//----------------------------------------------------------------//
	virtual	MOAIFrameBuffer*	GetDefaultFrameBuffer		() = 0;
	virtual	MOAITexture*		GetDefaultTexture			() = 0;
	void						SetTexture					( u32 textureUnit, MOAISingleTexture* texture );

public:
	
	//----------------------------------------------------------------//
	void			BindFrameBuffer				( MOAIFrameBuffer* frameBuffer = 0 );
	void			BindIndexBuffer				( MOAIIndexBuffer* buffer = 0 );
	void			BindVertexArray				( MOAIVertexArray* vtxArray = 0 );
	void			BindVertexBuffer			( MOAIVertexBuffer* buffer = 0 );
	void			BindVertexFormat			( MOAIVertexFormat* format = 0 );
	
					MOAIGfxDeviceStateCache		();
	virtual			~MOAIGfxDeviceStateCache	();

	virtual void	OnGfxStateWillChange		() = 0;

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
	
	void			SetShader					( MOAIShader* shader = 0 );
	void			SetShader					( u32 preset );
	void			SetShader					( MOAIShaderProgram* program = 0 );
	
	void			SetTexture					( MOAITextureBase* textureSet = 0 );
};

#endif
