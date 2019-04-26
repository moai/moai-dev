// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREFORMAT_H
#define	MOAITEXTUREFORMAT_H

#include <moai-util/MOAIImage.h>

class MOAITextureBase;

//================================================================//
// MOAITextureInfo
//================================================================//
class MOAITextureInfo {
public:
	u32			mWidth;
	u32			mHeight;
	size_t		mSize;
};

//================================================================//
// MOAITextureFormat
//================================================================//
class MOAITextureFormat :
	public virtual MOAIImageFormat {
protected:

	//----------------------------------------------------------------//
	void				CleanupTexture				( MOAITextureBase& texture );
	void				SetGLTexture				( MOAITextureBase& texture, const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize );

public:

	//----------------------------------------------------------------//
	virtual bool		CreateTexture				( MOAITextureBase& texture, const void* data, size_t size ) = 0;
	virtual bool		GetTextureInfo				( ZLStream& stream, MOAITextureInfo& info ) = 0;
						MOAITextureFormat			();
	virtual				~MOAITextureFormat			();
};

#endif
