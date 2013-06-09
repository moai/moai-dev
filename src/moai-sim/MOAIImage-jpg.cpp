// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIImage.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBJPG

extern "C" {
	#include <jinclude.h>
	#include <jpeglib.h>
	#include <jerror.h>
}

#define JPG_BUFFER_SIZE 2048

//void write_JPEG_file ( char* filename, int quality ) {
//
//	struct jpeg_compress_struct cinfo;
//	struct jpeg_error_mgr jerr;
//
//	FILE * outfile;
//	void* row_pointer;
//	int row_stride;
//
//
//	cinfo.err = jpeg_std_error(&jerr);
//	jpeg_create_compress(&cinfo);
//
//	if ((outfile = fopen(filename, "wb")) == NULL) {
//		fprintf(stderr, "can't open %s\n", filename);
//		exit(1);
//	}
//	jpeg_stdio_dest(&cinfo, outfile);
//
//	cinfo.image_width = image_width;
//	cinfo.image_height = image_height;
//	cinfo.input_components = 3;
//	cinfo.in_color_space = JCS_RGB;
//
//	jpeg_set_defaults(&cinfo);
//	jpeg_set_quality(&cinfo, quality, TRUE );
//
//	jpeg_start_compress(&cinfo, TRUE);
//
//	row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */
//
//	while (cinfo.next_scanline < cinfo.image_height) {
//		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
//		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
//	}
//
//	jpeg_finish_compress(&cinfo);
//	fclose(outfile);
//
//	jpeg_destroy_compress(&cinfo);
//}

//================================================================//
// libpng callbacks
//================================================================//

////----------------------------------------------------------------//
//static void _pngError ( png_structp png, png_const_charp err ) {
//	UNUSED ( png );
//	UNUSED ( err );
//}
//
////----------------------------------------------------------------//
//static void _pngFlush ( png_structp png ) {
//
//	ZLStream* stream = ( ZLStream* )png_get_io_ptr ( png );
//	stream->Flush ();
//}
//
////----------------------------------------------------------------//
//static void _pngRead ( png_structp png, png_bytep buffer, png_size_t size ) {
//
//	ZLStream* stream = ( ZLStream* )png_get_io_ptr ( png );
//	stream->ReadBytes ( buffer, ( u32 )size );
//}
//
////----------------------------------------------------------------//
//static void _pngWrite ( png_structp png, png_bytep buffer, png_size_t size ) {
//
//	ZLStream* stream = ( ZLStream* )png_get_io_ptr ( png );
//	stream->WriteBytes ( buffer, ( u32 )size );
//}

//================================================================//
// jpeg_stream_source
//================================================================//
typedef struct jpeg_usstream_source {
	jpeg_source_mgr		pub;
	ZLStream*			stream;
	void*				buffer;
} jpeg_usstream_source;

//----------------------------------------------------------------//
static boolean _jpgFillInputBuffer ( j_decompress_ptr cinfo ) {
	
	jpeg_usstream_source* src = ( jpeg_usstream_source* )cinfo->src;
	src->pub.bytes_in_buffer = src->stream->ReadBytes ( src->buffer, JPG_BUFFER_SIZE );
	src->pub.next_input_byte = ( JOCTET* )src->buffer;
	return TRUE;
}

//----------------------------------------------------------------//
static void _jpgInitSource ( j_decompress_ptr cinfo ) {

	jpeg_usstream_source* src = ( jpeg_usstream_source* )cinfo->src;
	src->buffer = malloc ( JPG_BUFFER_SIZE );
	src->pub.bytes_in_buffer = 0;
}

//----------------------------------------------------------------//
static void _jpgSkipInputData ( j_decompress_ptr cinfo, long num_bytes ) {
	
	jpeg_usstream_source* src = ( jpeg_usstream_source* )cinfo->src;
	
	if ( num_bytes <= ( long )src->pub.bytes_in_buffer ) {
		src->pub.bytes_in_buffer -= num_bytes;
		src->pub.next_input_byte = ( JOCTET* )(( uintptr )src->pub.next_input_byte + num_bytes );
	}
	else {
		src->stream->Seek ( num_bytes - src->pub.bytes_in_buffer, SEEK_CUR );
		src->pub.bytes_in_buffer = src->stream->ReadBytes ( src->buffer, JPG_BUFFER_SIZE );
		src->pub.next_input_byte = ( JOCTET* )src->buffer;
	}
}

//----------------------------------------------------------------//
static void _jpgTermSource ( j_decompress_ptr cinfo ) {

	jpeg_usstream_source* src = ( jpeg_usstream_source* )cinfo->src;
	free ( src->buffer );
}

//----------------------------------------------------------------//
static void set_jpeg_usstream_source ( j_decompress_ptr cinfo, ZLStream* stream ) {

	// affirm the src
	if ( cinfo->src == NULL ) {
		cinfo->src = ( struct jpeg_source_mgr* )(( *cinfo->mem->alloc_small )(( j_common_ptr )cinfo, JPOOL_PERMANENT, sizeof ( jpeg_usstream_source )));
	}

	jpeg_usstream_source* src = ( jpeg_usstream_source* )cinfo->src;
	
	// set up the callbacks
	src->pub.init_source = _jpgInitSource;
	src->pub.fill_input_buffer = _jpgFillInputBuffer;
	src->pub.skip_input_data = _jpgSkipInputData;
	src->pub.resync_to_restart = jpeg_resync_to_restart; // use default method
	src->pub.term_source = _jpgTermSource;
	
	src->pub.bytes_in_buffer = 0;
	src->pub.next_input_byte = 0;
	
	src->stream = stream;
}

//================================================================//
// MOAIImage-png
//================================================================//

//----------------------------------------------------------------//
void MOAIImage::LoadJpg ( ZLStream& stream, u32 transform ) {

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error ( &jerr );
	jpeg_create_decompress ( &cinfo );

	set_jpeg_usstream_source ( &cinfo, &stream );
	jpeg_read_header ( &cinfo, TRUE );
	
	jpeg_start_decompress ( &cinfo );
	
	this->LoadJpg ( &cinfo, transform );
	
	jpeg_finish_decompress ( &cinfo );
	jpeg_destroy_decompress ( &cinfo );
}

//----------------------------------------------------------------//
void MOAIImage::LoadJpg ( void* jpgInfoParam, u32 transform ) {
	
	jpeg_decompress_struct* cinfo = ( jpeg_decompress_struct* )jpgInfoParam;
	
	JDIMENSION width = cinfo->output_width;
	JDIMENSION height = cinfo->output_height;
	
	// set the dimensions, and padding (if any )
	bool isPadded = false;
	if ( transform & MOAIImageTransform::POW_TWO ) {
		this->mWidth = this->GetMinPowerOfTwo ( width );
		this->mHeight = this->GetMinPowerOfTwo ( height );
		isPadded = true;
	}
	else {
		this->mWidth = width;
		this->mHeight = height;
	}
	
	ZLColor::Format jpgColorFormat;
	
	switch ( cinfo->out_color_space ) {
		
		case JCS_GRAYSCALE:
			jpgColorFormat = ZLColor::A_8;
			break;
		
		case JCS_RGB:
			jpgColorFormat = ZLColor::RGB_888;
			break;
		
		default: return; // unsupported format
	}
	
	// override the image settings
	this->mPixelFormat = USPixel::TRUECOLOR;
	this->mColorFormat = jpgColorFormat;
	
	if (( transform & MOAIImageTransform::QUANTIZE ) && ( ZLColor::GetDepth ( jpgColorFormat ) > 16 )) {
		this->mColorFormat = ZLColor::RGB_565;
	}
		
	this->Alloc ();
	if ( isPadded ) {
		this->ClearBitmap ();
	}
	
	size_t srcRowSize = cinfo->output_width * cinfo->output_components;
	
	if ( this->mColorFormat == jpgColorFormat ) {
		
		if ( this->GetRowSize () < srcRowSize ) return;
		
		for ( u32 y = 0; y < height; ++y ) {
			JSAMPROW row = ( JSAMPROW )this->GetRowAddr ( y );
			jpeg_read_scanlines ( cinfo, &row, 1 );
		}
		
		if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
			for ( u32 y = 0; y < height; ++y ) {
				void* row = this->GetRowAddr ( y );
				ZLColor::PremultiplyAlpha ( row, this->mColorFormat, width );
			}
		}
	}
	else {
		
		void* rowBuffer = malloc ( srcRowSize );
		JSAMPROW samprow = ( JSAMPROW )rowBuffer;
		
		for ( u32 y = 0; y < height; ++y ) {
			jpeg_read_scanlines ( cinfo, &samprow, 1 );
			void* destRow = this->GetRowAddr ( y );
			ZLColor::Convert ( destRow, this->mColorFormat, rowBuffer, jpgColorFormat, width );
			
			if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
				ZLColor::PremultiplyAlpha ( destRow, this->mColorFormat, width );
			}
		}
	}
}

////----------------------------------------------------------------//
//bool MOAIImage::WritePNG ( ZLStream& stream ) {
//
//	png_structp png_ptr;
//	png_infop info_ptr;
//	
//	png_ptr = png_create_write_struct ( PNG_LIBPNG_VER_STRING, 0, _pngError, 0 );
//	assert ( png_ptr ); // TODO
//
//	info_ptr = png_create_info_struct ( png_ptr );
//	assert ( png_ptr ); // TODO
//
//	png_set_write_fn ( png_ptr, &stream, _pngWrite, _pngFlush );
//
//	int bitDepth = 0;
//	int colorType = 0;
//	
//	switch ( this->mColorFormat ) {
//		
//		case ZLColor::A_8:
//			
//			bitDepth = 8;
//			colorType = PNG_COLOR_TYPE_GRAY;
//			break;
//		
//		case ZLColor::RGB_888:
//		
//			bitDepth = 8;
//			colorType = PNG_COLOR_TYPE_RGB;
//			break;
//		
//		case ZLColor::RGBA_4444:
//		
//			bitDepth = 4;
//			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
//			break;
//		
//		case ZLColor::RGBA_8888:
//		
//			bitDepth = 8;
//			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
//			break;
//		
//		// TODO: support these formats
//		case ZLColor::RGB_565:
//		case ZLColor::RGBA_5551:
//		default:
//			assert ( false ); // TODO
//	};
//	
//	// Set the image information here.  Width and height are up to 2^31,
//	// bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
//	// the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
//	// PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
//	// or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
//	// PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
//	// currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
//	png_set_IHDR (
//		png_ptr,
//		info_ptr,
//		this->mWidth,
//		this->mHeight,
//		bitDepth,
//		colorType,
//		PNG_INTERLACE_NONE,
//		PNG_COMPRESSION_TYPE_BASE,
//		PNG_FILTER_TYPE_BASE
//	);
//
//	// Set the palette if there is one.  REQUIRED for indexed-color images
//	//png_colorp palette;
//	//palette = ( png_colorp )png_malloc ( png_ptr, PNG_MAX_PALETTE_LENGTH * png_sizeof ( png_color ));
//	
//	// ... Set palette colors...
//	//png_set_PLTE ( png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH );
//
//	// Write the file header information.
//	png_write_info(png_ptr, info_ptr);
//
//	// Flip BGR pixels to RGB
//	//png_set_bgr(png_ptr);
//
//	for ( u32 y = 0; y < this->mHeight; y++ ) {
//		png_bytep row = ( png_bytep )this->GetRowAddr ( y );
//		png_write_row ( png_ptr, row );
//	}
//
//	png_write_end(png_ptr, info_ptr);
//
//	//if ( palette ) {
//	//	png_free ( png_ptr, palette );
//	//}
//
//	//if ( trans ) {
//	//	png_free ( png_ptr, trans );
//	//}
//
//	png_destroy_write_struct ( &png_ptr, &info_ptr );
//	
//	return true;
//}

#endif