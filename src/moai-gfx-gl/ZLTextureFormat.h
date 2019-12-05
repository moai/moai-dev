// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTEXTUREFORMAT_H
#define	ZLTEXTUREFORMAT_H

class MOAITextureGL;

//================================================================//
// ZLTextureInfo
//================================================================//
class ZLTextureInfo {
public:
	u32			mWidth;
	u32			mHeight;
	size_t		mSize;
};

//================================================================//
// ZLTextureFormat
//================================================================//
class ZLTextureFormat :
	public virtual ZLImageFormat {
protected:

	//----------------------------------------------------------------//
	void				CleanupTexture				( MOAITextureGL& texture );
	void				SetGLTexture				( MOAITextureGL& texture, const ZLGfxHandle& glTexture, ZLGfxEnum::_ internalFormat, ZLGfxEnum::_ pixelType, size_t textureSize );

public:

	//----------------------------------------------------------------//
	virtual bool		CreateTexture				( MOAITextureGL& texture, const void* data, size_t size ) = 0;
	virtual bool		GetTextureInfo				( ZLStream& stream, ZLTextureInfo& info ) = 0;
						ZLTextureFormat				();
	virtual				~ZLTextureFormat			();
};

#endif
