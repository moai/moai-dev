// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLIMAGEFORMAT_H
#define	ZLIMAGEFORMAT_H

#include <zl-core/ZLImage.h>
#include <zl-core/ZLRtti.h>

//================================================================//
// ZLImageFormat
//================================================================//
class ZLImageFormat :
	public virtual RTTIBase {
protected:

	//----------------------------------------------------------------//
	void				Alloc						( ZLImage& image ) const;
	const void*			GetBitmap					( ZLImage& image ) const;
	void*				GetBitmapMutable			( ZLImage& image ) const;
	const void*			GetRowAddr					( const ZLImage& image, u32 y ) const;
	void*				GetRowAddrMutable			( ZLImage& image, u32 y ) const;
	void				SetColorFormat				( ZLImage& image, ZLColor::ColorFormat colorFormat ) const;
	bool				SetDimensions				( ZLImage& image, u32 width, u32 height, u32 transform ) const;
	void				SetPixelFormat				( ZLImage& image, ZLImage::PixelFormat pixelFormat ) const;

public:

	//----------------------------------------------------------------//
	virtual bool		CheckHeader					( const void* buffer );
	virtual size_t		GetHeaderSize				();
	virtual size_t		GetHeaderGuessSize			();
	virtual bool		GuessHeader					( const void* buffer );
						ZLImageFormat				();
	virtual				~ZLImageFormat				();
	virtual bool		ReadImage					( ZLImage& image, ZLStream& stream, u32 transform );
	virtual bool		WriteImage					( const ZLImage& image, ZLStream& stream );
};

#endif
