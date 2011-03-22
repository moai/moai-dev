// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#ifndef USIMAGE_H
#define USIMAGE_H

#include <uslsext/USColor.h>

class USData;

//================================================================//
// USImageTransform
//================================================================//
namespace USImageTransform {
	enum Flags {
		POW_TWO				= 1 << 0,
		QUANTIZE			= 1 << 1,
		TRUECOLOR			= 1 << 2,
		PREMULTIPLY_ALPHA	= 1 << 3,
	};
}

//================================================================//
// USImage
//================================================================//
class USImage {
private:

	USPixel::Format		mPixelFormat;
	USColor::Format		mColorFormat;

	u32		mWidth;
	u32		mHeight;
	
	u32		mPadWidth;
	u32		mPadHeight;
	
	void*	mData;
	void*	mPalette;
	void*	mBitmap;

	//----------------------------------------------------------------//
	void		Alloc				();
	u32			GetMinPowerOfTwo	( u32 size ); // gets the smallest power of two greater than size
	void		InitWithBitmap		( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt, bool copy );
	void		InitWithPng			( USStream& stream, u32 transform );
	void		InitWithPng			( void* pngParam, void* pngInfoParam, u32 transform );
	bool		IsPadded			();
	void		SetDimensions		( u32 width, u32 height, bool padToPow2 = false );

public:

	GET ( USPixel::Format, PixelFormat, mPixelFormat )
	GET ( USColor::Format, ColorFormat, mColorFormat )

	GET ( u32, Width, mWidth )
	GET ( u32, Height, mHeight )

	GET ( u32, PaddedWidth, mPadWidth )
	GET ( u32, PaddedHeight, mPadHeight )

	GET ( void*, Data, mData )
	GET ( void*, Palette, mPalette )
	GET ( void*, Bitmap, mBitmap )

	//----------------------------------------------------------------//
	void			ClearBitmap			();
	u32				GetBitmapSize		();
	u32				GetColor			( u32 i );
	u32				GetColor			( u32 x, u32 y );
	u32				GetDataSize			();
	u32				GetPaletteCount		();
	u32				GetPaletteSize		();
	u32				GetPixel			( u32 x, u32 y );
	void*			GetRow				( u32 y );
	u32				GetRowSize			();
	void			Init				( u32 width, u32 height, USColor::Format colorFmt, USPixel::Format pixelFmt );
	void			Init				( USData& data, u32 transform = 0 );
	void			Init				( cc8* filename, u32 transform = 0 );
	void			Init				( const void* buffer, u32 size, u32 transform = 0 );
	void			InitWithBitmap		( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt );
	bool			IsOK				();
	void			Release				();
	void			SetColor			( u32 x, u32 y, u32 color );
	void			SetPixel			( u32 x, u32 y, u32 pixel );
	void			Surrender			();
	void			Transform			( u32 transform );
					USImage				();
					~USImage			();
};

#endif
