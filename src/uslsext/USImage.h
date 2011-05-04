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
		POW_TWO				= 0x01,
		QUANTIZE			= 0x02,
		TRUECOLOR			= 0x04,
		PREMULTIPLY_ALPHA	= 0x08,
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
	
	void*	mData;
	void*	mPalette;
	void*	mBitmap;

	//----------------------------------------------------------------//
	void			Alloc				();
	static u32		GetMinPowerOfTwo	( u32 size ); // gets the smallest power of two greater than size
	void			Init				( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt, bool copy );
	void			LoadPng				( USStream& stream, u32 transform );
	void			LoadPng				( void* pngParam, void* pngInfoParam, u32 transform );

public:

	GET_CONST ( USPixel::Format, PixelFormat, mPixelFormat )
	GET_CONST ( USColor::Format, ColorFormat, mColorFormat )

	GET_CONST ( u32, Width, mWidth )
	GET_CONST ( u32, Height, mHeight )

	GET ( void*, Data, mData )
	GET ( void*, Palette, mPalette )
	GET ( void*, Bitmap, mBitmap )

	//----------------------------------------------------------------//
	void			BleedRect			( int xMin, int yMin, int xMax, int yMax );
	void			ClearBitmap			();
	void			ConvertColors		( const USImage& image, USColor::Format colorFmt );
	void			Copy				( const USImage& image );
	void			CopyBits			( const USImage& image, int srcX, int srcY, int destX, int destY, int width, int height );
	u32				GetBitmapSize		() const;
	u32				GetColor			( u32 i ) const;
	u32				GetColor			( u32 x, u32 y ) const;
	u32				GetDataSize			() const;
	u32				GetPaletteCount		() const;
	u32				GetPaletteColor		( u32 idx ) const;
	u32				GetPaletteSize		() const;
	u32				GetPixel			( u32 x, u32 y ) const;
	void*			GetRowAddr			( u32 y );
	const void*		GetRowAddr			( u32 y ) const;
	u32				GetRowSize			() const;
	void			Init				( u32 width, u32 height, USColor::Format colorFmt, USPixel::Format pixelFmt );
	void			Init				( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt );
	void			Load				( USData& data, u32 transform = 0 );
	void			Load				( cc8* filename, u32 transform = 0 );
	void			Load				( const void* buffer, u32 size, u32 transform = 0 );
	bool			IsOK				();
	void			PadToPow2			( const USImage& image );
	void			Release				();
	void			ResizeCanvas		( const USImage& image, USIntRect rect );
	void			SetColor			( u32 x, u32 y, u32 color );
	void			SetPaletteColor		( u32 idx, u32 rgba );
	void			SetPixel			( u32 x, u32 y, u32 pixel );
	void			Surrender			();
	void			Transform			( u32 transform );
					USImage				();
					~USImage			();
	bool			WritePNG			( USStream& stream );
};

#endif
