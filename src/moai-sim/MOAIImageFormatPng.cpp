// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBPNG

#include <moai-sim/MOAIImageFormatPng.h>
#include <moai-sim/MOAITextureBase.h>

#include <png.h>

//================================================================//
// libpng callbacks
//================================================================//

//----------------------------------------------------------------//
static void _pngError ( png_structp png, png_const_charp err ) {
	UNUSED ( png );
	UNUSED ( err );
}

//----------------------------------------------------------------//
static void _pngFlush ( png_structp png ) {

	ZLStream* stream = ( ZLStream* )png_get_io_ptr ( png );
	stream->Flush ();
}

//----------------------------------------------------------------//
static void _pngRead ( png_structp png, png_bytep buffer, png_size_t size ) {

	ZLStream* stream = ( ZLStream* )png_get_io_ptr ( png );
	stream->ReadBytes ( buffer, ( u32 )size );
}

//----------------------------------------------------------------//
static void _pngWrite ( png_structp png, png_bytep buffer, png_size_t size ) {

	ZLStream* stream = ( ZLStream* )png_get_io_ptr ( png );
	stream->WriteBytes ( buffer, ( u32 )size );
}

//================================================================//
// MOAIImageFormatPng
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageFormatPng::CheckHeader ( const void* buffer ) {

	static u8 magic [] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A }; // <?> P N G <CR><LF><SUB><LF>
	return ( memcmp ( buffer, magic, 8 ) == 0 );
}

//----------------------------------------------------------------//
bool MOAIImageFormatPng::CreateTexture ( MOAITextureBase& texture, const void* data, size_t size ) {
	UNUSED ( texture );
	UNUSED ( data );
	UNUSED ( size );
	
	return false;
}

//----------------------------------------------------------------//
size_t MOAIImageFormatPng::GetHeaderSize () {

	return HEADER_SIZE;
}

//----------------------------------------------------------------//
MOAIImageFormatPng::MOAIImageFormatPng () {
}

//----------------------------------------------------------------//
MOAIImageFormatPng::~MOAIImageFormatPng () {
}

//----------------------------------------------------------------//
bool MOAIImageFormatPng::ReadImage ( MOAIImage& image, ZLStream& stream, u32 transform ) {

	png_structp png = png_create_read_struct ( PNG_LIBPNG_VER_STRING, 0, _pngError, 0 );
	if ( !png ) return false;

	png_infop pngInfo = png_create_info_struct ( png );
	if ( pngInfo ) {
		png_set_read_fn ( png, &stream, _pngRead );
		this->ReadImagePng ( image, png, pngInfo, transform );
	}

	png_destroy_read_struct ( &png, &pngInfo, NULL );
    return true;
}

//----------------------------------------------------------------//
void MOAIImageFormatPng::ReadImagePng ( MOAIImage& image, void* pngParam, void* pngInfoParam, u32 transform ) {
	
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
	int passes = png_set_interlace_handling ( png );
	
	// no fat palettes
	if ( paletteSize > 256 ) return;
	
	// set the dimensions, and padding (if any )
	bool isPadded = this->SetDimensions ( image, width, height, transform );
	
	// now guess the format and color type, according to the png
	MOAIImage::PixelFormat pngPixelFormat;
	ZLColor::ColorFormat pngColorFormat;
	
	switch ( pngColorType ) {
		
		case PNG_COLOR_TYPE_GRAY:
			pngPixelFormat = MOAIImage::TRUECOLOR;
			pngColorFormat = ZLColor::A_8;
			break;
		
		case PNG_COLOR_TYPE_PALETTE:
			pngPixelFormat = ( paletteSize > 16 ) ? MOAIImage::INDEX_8 : MOAIImage::INDEX_4;
			pngColorFormat = ( transSize ) ? ZLColor::RGBA_8888 : ZLColor::RGB_888;
			break;
		
		case PNG_COLOR_TYPE_RGB:
			pngPixelFormat = MOAIImage::TRUECOLOR;
			pngColorFormat = ZLColor::RGB_888;
			break;
		
		case PNG_COLOR_TYPE_RGB_ALPHA:
			pngPixelFormat = MOAIImage::TRUECOLOR;
			pngColorFormat = ZLColor::RGBA_8888;
			break;
		
		default: return; // unsupported format
	}
	
	// override the image settings
	this->SetPixelFormat ( image, ( transform & MOAIImageTransform::TRUECOLOR ) ? MOAIImage::TRUECOLOR : pngPixelFormat );
	this->SetColorFormat ( image, pngColorFormat );
	
	if (( transform & MOAIImageTransform::QUANTIZE ) && ( ZLColor::GetDepthInBits ( pngColorFormat ) > 16 )) {
		
		switch ( pngColorFormat ) {
			case ZLColor::RGB_888:
				this->SetColorFormat ( image, ZLColor::RGB_565 );
				break;
			case ZLColor::RGBA_8888:
				this->SetColorFormat ( image, ZLColor::RGBA_4444 );
				break;
			default:
				break;
		}
	}
	
	if ( image.GetPixelFormat () == MOAIImage::TRUECOLOR ) {
		
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
		
		this->Alloc ( image );
		
		if ( isPadded ) {
			image.ClearBitmap ();
		}
		
		if ( image.GetColorFormat () == pngColorFormat ) {
			
			if ( image.GetRowSize () < png_get_rowbytes ( png, pngInfo )) return;
			
			for ( int i = 0; i < passes; ++i ) {
				for ( u32 y = 0; y < height; ++y ) {
					void* row = this->GetRowAddr ( image, y );
					png_read_row ( png, ( png_bytep )row, 0 );
				}
			}
			
			if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
				for ( u32 y = 0; y < height; ++y ) {
					void* row = this->GetRowAddr ( image, y );
					ZLColor::PremultiplyAlpha ( row, image.GetColorFormat (), width );
				}
			}
		}
		else {
			
			u32 srcRowSize = ( u32 )png_get_rowbytes ( png, pngInfo );
			
			if ( passes > 1 ) {
				
				u32 srcBuffSize = srcRowSize * height;
				void* srcBuff = malloc ( srcBuffSize );
				
				for ( int i = 0; i < passes; ++i ) {
					for ( u32 y = 0; y < height; ++y ) {
						void* srcRow = ( void* )(( size_t )srcBuff + ( srcRowSize * y ));
						png_read_row ( png, ( png_bytep )srcRow, 0 );
					}
				}
				
				for ( u32 y = 0; y < height; ++y ) {
					void* srcRow = ( void* )(( size_t )srcBuff + ( srcRowSize * y ));
					void* destRow = this->GetRowAddr ( image, y );
					ZLColor::Convert ( destRow, image.GetColorFormat (), srcRow, pngColorFormat, width );
					
					if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
						ZLColor::PremultiplyAlpha ( destRow, image.GetColorFormat (), width );
					}
				}
				free ( srcBuff );
			}
			else {
				
				void* srcRow = malloc ( srcRowSize );
				
				for ( u32 y = 0; y < height; ++y ) {
					png_read_row ( png, ( png_bytep )srcRow, 0 );
					void* destRow = this->GetRowAddr ( image, y );
					ZLColor::Convert ( destRow, image.GetColorFormat (), srcRow, pngColorFormat, width );
					
					if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
						ZLColor::PremultiplyAlpha ( destRow, image.GetColorFormat (), width );
					}
				}
				free ( srcRow );
			}
		}
	}
	else {
	
		u32 rowsize = image.GetRowSize ();
		if ( rowsize < png_get_rowbytes ( png, pngInfo )) return;
		
		this->Alloc ( image );
		if ( isPadded ) {
			image.ClearBitmap ();
		}

		// copy the color values
		for ( int i = 0; i < paletteSize; ++i ) {
		
			int r = palette [ i ].red;
			int g = palette [ i ].green;
			int b = palette [ i ].blue;
			int a = i < transSize ? trans [ i ] : 0xff;
			
			if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
				r = ( r * a ) >> 8;
				g = ( g * a ) >> 8;
				b = ( b * a ) >> 8;
			}
			
			image.SetPaletteColor ( i, ZLColor::PackRGBA ( r, g, b, a ));
		}
		
		// copy the rows
		for ( int i = 0; i < passes; ++i ) {
			for ( u32 y = 0; y < height; ++y ) {
				void* row = this->GetRowAddr ( image, y );
				png_read_row ( png, ( png_bytep )row, 0 );
			}
		}
	}
}

//----------------------------------------------------------------//
bool MOAIImageFormatPng::WriteImage ( const MOAIImage& image, ZLStream& stream ) {

	png_structp png_ptr;
	png_infop info_ptr;
	
	png_ptr = png_create_write_struct ( PNG_LIBPNG_VER_STRING, 0, _pngError, 0 );
	assert ( png_ptr ); // TODO

	info_ptr = png_create_info_struct ( png_ptr );
	assert ( png_ptr ); // TODO

	png_set_write_fn ( png_ptr, &stream, _pngWrite, _pngFlush );

	int bitDepth = 0;
	int colorType = 0;
	
	switch ( image.GetColorFormat ()) {
		
		case ZLColor::A_8:
			
			bitDepth = 8;
			colorType = PNG_COLOR_TYPE_GRAY;
			break;
		
		case ZLColor::RGB_888:
		
			bitDepth = 8;
			colorType = PNG_COLOR_TYPE_RGB;
			break;
		
		case ZLColor::RGBA_4444:
		
			bitDepth = 4;
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
		
		case ZLColor::RGBA_8888:
		
			bitDepth = 8;
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
		
		// TODO: support these formats
		case ZLColor::RGB_565:
		case ZLColor::RGBA_5551:
		default:
			assert ( false ); // TODO
	};
	
	// Set the image information here.  Width and height are up to 2^31,
	// bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	// the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	// PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	// or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	// PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	// currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	png_set_IHDR (
		png_ptr,
		info_ptr,
		image.GetWidth (),
		image.GetHeight (),
		bitDepth,
		colorType,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE
	);

	// Set the palette if there is one.  REQUIRED for indexed-color images
	//png_colorp palette;
	//palette = ( png_colorp )png_malloc ( png_ptr, PNG_MAX_PALETTE_LENGTH * png_sizeof ( png_color ));
	
	// ... Set palette colors...
	//png_set_PLTE ( png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH );

	// Write the file header information.
	png_write_info(png_ptr, info_ptr);

	// Flip BGR pixels to RGB
	//png_set_bgr(png_ptr);

	for ( u32 y = 0; y < image.GetHeight (); y++ ) {
		png_bytep row = ( png_bytep )this->GetRowAddr ( image, y );
		png_write_row ( png_ptr, row );
	}

	png_write_end(png_ptr, info_ptr);

	//if ( palette ) {
	//	png_free ( png_ptr, palette );
	//}

	//if ( trans ) {
	//	png_free ( png_ptr, trans );
	//}

	png_destroy_write_struct ( &png_ptr, &info_ptr );
	
	return true;
}

#endif
