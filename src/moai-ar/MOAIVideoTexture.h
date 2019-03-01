// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVIDEOTEXTURE_H
#define	MOAIVIDEOTEXTURE_H

#include <moai-sim/MOAITextureBase.h>

#if MOAI_WITH_ARTOOLKIT

//================================================================//
// MOAIVideoTexture
//================================================================//
class MOAIVideoTexture :
	public MOAITextureBase {
private:

	friend class MOAIMarkerMgr;

	enum {
		VIDEO_TEXTURE_PLANE_0,
		VIDEO_TEXTURE_PLANE_1,
	};


	AR_PIXEL_FORMAT		mFormat;

	bool				mUsePlane;
	u32					mVideoPlane;

	int					mGLFormat;
	int					mGLPixelSize;
	int					mPixelStoreUnpackAlignment;

	ZLCopyOnWrite		mBuffer;

	//----------------------------------------------------------------//
	void			Init					( u32 videoPlane, AR_PIXEL_FORMAT format );
	void			UpdateBuffer			( void* buffer );

	//----------------------------------------------------------------//
	bool			ZLAbstractGfxResource_OnGPUCreate				();
	bool			ZLAbstractGfxResource_OnGPUUpdate				();

public:

	DECL_LUA_FACTORY ( MOAIVideoTexture )

	//----------------------------------------------------------------//
					MOAIVideoTexture		();
					~MOAIVideoTexture		();
};

#endif
#endif
