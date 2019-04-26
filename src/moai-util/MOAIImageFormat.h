// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGEFORMAT_H
#define	MOAIIMAGEFORMAT_H

#include <moai-util/MOAIImage.h>

class MOAITextureBase;

//================================================================//
// MOAIImageFormat
//================================================================//
class MOAIImageFormat :
	public virtual RTTIBase {
protected:

	//----------------------------------------------------------------//
	void				Alloc						( MOAIImage& image ) const;
	const void*			GetBitmap					( MOAIImage& image ) const;
	void*				GetBitmapMutable			( MOAIImage& image ) const;
	const void*			GetRowAddr					( const MOAIImage& image, u32 y ) const;
	void*				GetRowAddrMutable			( MOAIImage& image, u32 y ) const;
	void				SetColorFormat				( MOAIImage& image, ZLColor::ColorFormat colorFormat ) const;
	bool				SetDimensions				( MOAIImage& image, u32 width, u32 height, u32 transform ) const;
	void				SetPixelFormat				( MOAIImage& image, MOAIImage::PixelFormat pixelFormat ) const;

public:

	//----------------------------------------------------------------//
	virtual bool		CheckHeader					( const void* buffer );
	virtual size_t		GetHeaderSize				();
	virtual size_t		GetHeaderGuessSize			();
	virtual bool		GuessHeader					( const void* buffer );
						MOAIImageFormat				();
	virtual				~MOAIImageFormat			();
	virtual bool		ReadImage					( MOAIImage& image, ZLStream& stream, u32 transform );
	virtual bool		WriteImage					( const MOAIImage& image, ZLStream& stream );
};

#endif
