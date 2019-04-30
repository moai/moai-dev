// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTEXTUREFORMAT_H
#define	ZLTEXTUREFORMAT_H

#include <zl-gfx/ZLGfxHandle.h>
#include <zl-gfx/ZLTextureBase.h>

class ZLTexture;

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
	void				CleanupTexture				( ZLTextureBase& texture );
	void				SetGLTexture				( ZLTextureBase& texture, const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize );

public:

	//----------------------------------------------------------------//
	virtual bool		CreateTexture				( ZLTextureBase& texture, const void* data, size_t size ) = 0;
	virtual bool		GetTextureInfo				( ZLStream& stream, ZLTextureInfo& info ) = 0;
						ZLTextureFormat				();
	virtual				~ZLTextureFormat			();
};

#endif
