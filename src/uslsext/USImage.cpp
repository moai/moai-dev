// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <png.h>
#include <uslsext/USImage.h>
#include <uslsext/USData.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static void _pngError ( png_structp png, png_const_charp err ) {
	UNUSED ( png );
	UNUSED ( err );
}

//----------------------------------------------------------------//
static void _pngRead ( png_structp png, png_bytep buffer, png_size_t size ) {

	USStream* stream = ( USStream* )png_get_io_ptr ( png );
	stream->ReadBytes ( buffer, ( u32 )size );
}

//================================================================//
// USImage
//================================================================//

//----------------------------------------------------------------//
void USImage::Alloc () {

	if ( this->mData ) {
		free ( this->mData );
	}

	u32 paletteSize = this->GetPaletteSize ();
	u32 bitmapSize = this->GetBitmapSize ();

	this->mData = malloc ( paletteSize + bitmapSize );
	this->mBitmap = ( void* )(( u32 )this->mData + paletteSize );
	
	if ( this->IsPadded ()) {
		memset ( this->mBitmap, 0x00, bitmapSize );
	}
	
	if ( paletteSize ) {
		this->mPalette = this->mData;
		memset ( this->mPalette, 0xff, paletteSize );
	}
	else {
		this->mPalette = 0;
	}
}

//----------------------------------------------------------------//
void USImage::ClearBitmap () {

	if ( this->mBitmap ) {
		memset ( this->mBitmap, 0, this->GetBitmapSize ());
	}
}

//----------------------------------------------------------------//
u32 USImage::GetBitmapSize () {

	return this->GetRowSize () * this->mPadHeight;
}

//----------------------------------------------------------------//
u32 USImage::GetColor ( u32 i ) {

	u32 colorDepth = USColor::GetDepth ( this->mColorFormat );

	u32 total = this->GetPaletteCount ();
	if ( i < total ) {
		
		u8* stream = ( u8* )(( u32 )this->mPalette + (( u32 )( colorDepth >> 3 ) * i ));
		return USColor::ReadRGBA ( stream, this->mColorFormat );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 USImage::GetColor ( u32 x, u32 y ) {

	if ( !this->mBitmap ) return 0;

	switch ( this->mPixelFormat ) {
		case USPixel::TRUECOLOR: {
			
			u32 pixel = this->GetPixel ( x, y );
			return USColor::ConvertToRGBA ( pixel, this->mColorFormat );
		}
		case USPixel::INDEX_4:
		case USPixel::INDEX_8: {
			
			u32 i = this->GetPixel ( x, y );
			return this->GetColor ( i );
		}
		default:
			break;
	}

	return 0;	
}

//----------------------------------------------------------------//
u32 USImage::GetDataSize () {

	return this->GetPaletteSize () + this->GetBitmapSize ();
}

//----------------------------------------------------------------//
u32 USImage::GetMinPowerOfTwo ( u32 size ) {

	u32 pow2 = 0x01;
	while (( pow2 < size ) && ( pow2 != 0x10000000 )) pow2 = pow2 << 0x01;
	return pow2;
}

//----------------------------------------------------------------//
u32 USImage::GetPaletteCount () {

	return USPixel::GetPaletteCount ( this->mPixelFormat );
}

//----------------------------------------------------------------//
u32 USImage::GetPaletteSize () {

	return USPixel::GetPaletteSize ( this->mPixelFormat, this->mColorFormat );
}

//----------------------------------------------------------------//
u32 USImage::GetPixel ( u32 x, u32 y ) {

	if ( y > this->mHeight ) return 0;
	if ( x > this->mWidth ) return 0;

	void* row = this->GetRow ( y );

	float pixelSize = USPixel::GetSize ( this->mPixelFormat, this->mColorFormat );
	u32 pixel;
	
	if ( pixelSize == 0.5f ) {
		u8* stream = ( u8* )(( u32 )row + ( x >> 0x01 ));
		pixel = ( u32 )( *stream );
		pixel = (( pixel >> (( x & 0x01 ) ? 0x04 : 0x00 )) & 0x0F );
	}
	else {
		u32 size = ( u32 )pixelSize;
		u8* stream = ( u8* )(( u32 )row + ( x * size ));
		pixel = USPixel::ReadPixel ( stream, size );
	}
	
	return pixel;
}

//----------------------------------------------------------------//
void* USImage::GetRow ( u32 y ) {

	return ( void* )(( u32 )this->mBitmap + ( this->GetRowSize () * y ));
}

//----------------------------------------------------------------//
u32 USImage::GetRowSize () {

	u32 rowSize = ( u32 )(( float )this->mPadWidth * USPixel::GetSize ( this->mPixelFormat, this->mColorFormat ));
	return ( rowSize + ( rowSize & 0x01 ));
}

//----------------------------------------------------------------//
void USImage::Init ( u32 width, u32 height, USColor::Format colorFmt, USPixel::Format pixelFmt ) {

	this->mColorFormat = colorFmt;
	this->mPixelFormat = pixelFmt;

	this->mWidth = width;
	this->mHeight = height;
	
	this->mPadWidth = width;
	this->mPadHeight = height;
	
	this->Alloc ();
}

//----------------------------------------------------------------//
void USImage::Init ( USData& data , u32 transform ) {

	void* bytes;
	u32 size;
	data.Lock ( &bytes, &size );

	this->Init ( bytes, size, transform );
	
	data.Unlock ();
}

//----------------------------------------------------------------//
void USImage::Init ( cc8* filename, u32 transform ) {

	this->Release ();
	if ( !USFileSys::CheckFileExists ( filename )) return;
	
	USFileStream stream;
	stream.OpenRead ( filename );
	
	u32 size = stream.GetLength ();
	void* buffer = malloc ( size );
	stream.ReadBytes ( buffer, size );
	
	stream.Close ();
	
	this->Init ( buffer, size, transform );
	free ( buffer );
}

//----------------------------------------------------------------//
void USImage::Init ( const void* buffer, u32 size, u32 transform ) {

	this->Release ();
	if ( size < 8 ) return;
	
	int isPng = ( png_sig_cmp (( png_bytep )buffer, 0, 8 ) == 0 );
	
	if ( isPng ) {
		USByteStream stream;
		stream.SetBuffer (( void* )buffer, size );
		this->InitWithPng ( stream, transform );
	}
}

//----------------------------------------------------------------//
void USImage::InitWithBitmap ( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt ) {

	this->InitWithBitmap ( bitmap, width, height, colorFmt, true );
}

//----------------------------------------------------------------//
void USImage::InitWithBitmap ( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt, bool copy ) {

	this->Release ();

	if ( !bitmap ) return;

	this->mPixelFormat = USPixel::TRUECOLOR;
	this->mColorFormat = colorFmt;

	this->SetDimensions ( width, height, false );
	
	if ( copy ) {
		this->Alloc ();
		u32 size = this->GetBitmapSize ();
		memcpy ( this->mBitmap, bitmap, size );
	}
	else {
		this->mData = ( void* )bitmap;
		this->mBitmap = this->mData;
		this->mPalette = 0;
	}
}

//----------------------------------------------------------------//
void USImage::InitWithPng ( USStream& stream, u32 transform ) {

	png_structp png = png_create_read_struct ( PNG_LIBPNG_VER_STRING, 0, _pngError, 0 );
	if ( !png ) return;

	png_infop pngInfo = png_create_info_struct ( png );
	if ( pngInfo ) {
		png_set_read_fn ( png, &stream, _pngRead );
		this->InitWithPng ( png, pngInfo, transform );
	}

	png_destroy_read_struct ( &png, &pngInfo, NULL );
}

//----------------------------------------------------------------//
void USImage::InitWithPng ( void* pngParam, void* pngInfoParam, u32 transform ) {
	
	png_structp png = ( png_structp )pngParam;
	png_infop pngInfo = ( png_infop )pngInfoParam;
	
	if ( !( png && pngInfo )) return;
	
	png_uint_32 width;
	png_uint_32 height;
	int bitDepth;
	int pngColorType;
	int interlaceType;
	
	int paletteSize = 0;
	png_colorp palette;
	
	int transSize = 0;
	png_bytep trans;
	
	png_read_info ( png, pngInfo );
	png_get_IHDR ( png, pngInfo, &width, &height, &bitDepth, &pngColorType, &interlaceType, 0, 0 );
	png_get_PLTE ( png, pngInfo, &palette, &paletteSize );
	png_get_tRNS ( png, pngInfo, &trans, &transSize, 0 );
	
	// we don't handle interlaced pngs at the moment
	if ( interlaceType != PNG_INTERLACE_NONE ) return;
	
	// no fat palettes
	if ( paletteSize > 256 ) return;
	
	// set the dimensions, and padding (if any )
	this->SetDimensions ( width, height, ( transform & USImageTransform::POW_TWO ));
	
	// now guess the format and color type, according to the png
	USPixel::Format pixelFormat;
	USColor::Format colorFormat;	
	
	switch ( pngColorType ) {
		
		case PNG_COLOR_TYPE_PALETTE:
			pixelFormat = ( paletteSize > 16 ) ? USPixel::INDEX_8 : USPixel::INDEX_4;
			colorFormat = ( transSize ) ? USColor::RGBA_8888 : USColor::RGB_888;
			break;
		
		case PNG_COLOR_TYPE_RGB:
			pixelFormat = USPixel::TRUECOLOR;
			colorFormat = USColor::RGB_888;
			break;
		
		case PNG_COLOR_TYPE_RGB_ALPHA:
			pixelFormat = USPixel::TRUECOLOR;
			colorFormat = USColor::RGBA_8888;
			break;
		
		default: return; // unsupported format
	}
	
	// override the image settings
	this->mPixelFormat = ( transform & USImageTransform::TRUECOLOR ) ? USPixel::TRUECOLOR : pixelFormat;
	
	this->mColorFormat = colorFormat;
	
	if (( transform & USImageTransform::QUANTIZE ) && ( USColor::GetDepth ( colorFormat ) > 16 )) {
		
		switch ( colorFormat ) {
			case USColor::RGB_888:
				this->mColorFormat = USColor::RGB_565;
				break;
			case USColor::RGBA_8888:
				this->mColorFormat = USColor::RGBA_4444;
				break;
			default:
				break;
		}
	}
	
	if ( this->mPixelFormat == USPixel::TRUECOLOR ) {
		
		// expand lower bit depths to 8 bits per pixel
		if ( bitDepth < 8 ) {
			png_set_packing ( png );
		}
		
		// reduce higher bit depths to 8 bits per pixel
		if ( bitDepth == 16 ) {
			png_set_strip_16 ( png );
		}
		
		if ( paletteSize ) {
			png_set_expand ( png );
		}
		
		// ah, yes...
		png_read_update_info ( png, pngInfo );
		
		this->Alloc ();
		
		if ( this->mColorFormat == colorFormat ) {
			
			if ( this->GetRowSize () < png_get_rowbytes ( png, pngInfo )) return;
			
			for ( u32 y = 0; y < height; ++y ) {
				void* row = this->GetRow ( y );
				png_read_row ( png, ( png_bytep )row, 0 );
				
				if ( transform & USImageTransform::PREMULTIPLY_ALPHA ) {
					USColor::PremultiplyAlpha ( row, this->mColorFormat, width );
				}
			}
		}
		else {
			
			u32 srcRowSize = ( u32 )png_get_rowbytes ( png, pngInfo );
			
			assert ( width <= 1024 ); // handle this better later
			assert ( srcRowSize <= ( 1024 * sizeof ( u32 ))); // handle this better later
			
			u32 srcRow [ 1024 ];
			
			for ( u32 y = 0; y < height; ++y ) {
				png_read_row ( png, ( png_bytep )srcRow, 0 );
				void* destRow = this->GetRow ( y );
				USColor::Convert ( destRow, this->mColorFormat, srcRow, colorFormat, width );
				
				if ( transform & USImageTransform::PREMULTIPLY_ALPHA ) {
					USColor::PremultiplyAlpha ( destRow, this->mColorFormat, width );
				}
			}
		}
	}
	else {
	
		u32 rowsize = this->GetRowSize ();
		if ( rowsize < png_get_rowbytes ( png, pngInfo )) return;
		this->Alloc ();
		
		u32 colorSize = USColor::GetSize ( this->mColorFormat );
		
		// copy the color values
		for ( int i = 0; i < paletteSize; ++i ) {
			u8* color = ( u8* )(( u32 )this->mPalette + ( i * colorSize ));
			color [ 0 ] = palette [ i ].red;
			color [ 1 ] = palette [ i ].green;
			color [ 2 ] = palette [ i ].blue;
		}
		
		// copy the trans values
		for ( int i = 0; i < transSize; ++i ) {
			u8* color = ( u8* )(( u32 )this->mPalette + ( i * colorSize ));
			color [ 3 ] = trans [ i ];
		}
		
		// copy the rows
		for ( u32 y = 0; y < height; ++y ) {
			void* row = ( void* )(( u32 )this->mBitmap  + ( rowsize * y ));
			png_read_row ( png, ( png_bytep )row, 0 );
		}
	}
}

//----------------------------------------------------------------//
bool USImage::IsOK () {

	return ( this->mBitmap != 0 );
}

//----------------------------------------------------------------//
bool USImage::IsPadded () {

	return (( this->mWidth != this->mPadWidth ) || ( this->mHeight != this->mPadHeight ));
}

//----------------------------------------------------------------//
void USImage::Release () {

	if ( this->mData ) {
		free ( this->mData );
	}
	
	this->Surrender ();
}

//----------------------------------------------------------------//
void USImage::SetColor ( u32 x, u32 y, u32 color ) {

	if ( this->mPixelFormat != USPixel::TRUECOLOR ) return;
	
	if ( y > this->mHeight ) return;
	if ( x > this->mWidth ) return;

	u32 pixel = USColor::ConvertFromRGBA ( color, this->mColorFormat );
	this->SetPixel ( x, y, pixel );
}

//----------------------------------------------------------------//
void USImage::SetDimensions ( u32 width, u32 height, bool padToPow2 ) {

	this->mWidth = width;
	this->mHeight = height;
	
	if ( padToPow2 ) {
		this->mPadWidth = this->GetMinPowerOfTwo ( width );
		this->mPadHeight = this->GetMinPowerOfTwo ( height );
	}
	else {
		this->mPadWidth = width;
		this->mPadHeight = height;
	}
}

//----------------------------------------------------------------//
void USImage::SetPixel ( u32 x, u32 y, u32 pixel ) {

	if ( y > this->mHeight ) return;
	if ( x > this->mWidth ) return;
	
	void* row = this->GetRow ( y );

	u32 pixelDepth = USPixel::GetDepth ( this->mPixelFormat, this->mColorFormat );
	u32 pixelMask = USPixel::GetMask ( this->mPixelFormat, this->mColorFormat );
	
	if ( pixelDepth == 4 ) {
		u8* address = ( u8* )(( u32 )row + ( x >> 0x01 ));
		( *address ) = ( *address ) | ( u8 )(( pixel & pixelMask ) << (( x & 0x01 ) ? 0x04 : 0x00 ));
	}
	else {
		u32 size = ( pixelDepth >> 3 );
		u8* stream = ( u8* )(( u32 )row + ( x * size ));
		USPixel::WritePixel ( stream, pixel, size );
	}
}

//----------------------------------------------------------------//
void USImage::Surrender () {

	mColorFormat = USColor::CLR_FMT_UNKNOWN;
	mPixelFormat = USPixel::PXL_FMT_UNKNOWN;

	this->mWidth = 0;
	this->mHeight = 0;
	
	this->mPadWidth = 0;
	this->mPadHeight = 0;
	
	this->mData = 0;
	this->mBitmap = 0;
	this->mPalette = 0;
}

//----------------------------------------------------------------//
void USImage::Transform ( u32 transform ) {
	UNUSED ( transform );
}

//----------------------------------------------------------------//
USImage::USImage () :
	mPixelFormat ( USPixel::PXL_FMT_UNKNOWN ),
	mColorFormat ( USColor::CLR_FMT_UNKNOWN ),
	mWidth ( 0 ),
	mHeight ( 0 ),
	mData ( 0 ),
	mPalette ( 0 ),
	mBitmap ( 0 ) {
}

//----------------------------------------------------------------//
USImage::~USImage	() {

	this->Release ();
}

