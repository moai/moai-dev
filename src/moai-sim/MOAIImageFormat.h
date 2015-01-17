// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMAT_H
#define	MOAIIMAGEFORMAT_H

#include <moai-sim/MOAIImage.h>

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
// MOAIImageFormat
//================================================================//
class MOAIImageFormat {
protected:

	//----------------------------------------------------------------//
	void				Alloc						( MOAIImage& image ) const;
	void				CleanupTexture				( MOAITextureBase& texture );
	void*				GetBitmap					( MOAIImage& image ) const;
	void*				GetRowAddr					( MOAIImage& image, u32 y ) const;
	const void*			GetRowAddr					( const MOAIImage& image, u32 y ) const;
	void				SetColorFormat				( MOAIImage& image, ZLColor::ColorFormat colorFormat ) const;
	bool				SetDimensions				( MOAIImage& image, u32 width, u32 height, u32 transform ) const;
	void				SetPixelFormat				( MOAIImage& image, MOAIImage::PixelFormat pixelFormat ) const;
	void				SetTextureID				( MOAITextureBase& texture, u32 glTexID, int internalFormat, int pixelType, size_t textureSize );

public:

	//----------------------------------------------------------------//
	virtual bool		CheckHeader					( const void* buffer ) = 0;
	virtual bool		CreateTexture				( MOAITextureBase& texture, const void* data, size_t size );
	virtual size_t		GetHeaderSize				() = 0;
	virtual bool		GetTextureInfo				( ZLStream& stream, MOAITextureInfo& info );
						MOAIImageFormat				();
	virtual				~MOAIImageFormat			();
	virtual bool		ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	virtual bool		WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
