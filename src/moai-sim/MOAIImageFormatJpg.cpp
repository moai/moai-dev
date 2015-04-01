// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBJPG

#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIImageFormatJpg.h>

extern "C" {
	#include <jinclude.h>
	#include <jpeglib.h>
	#include <jerror.h>
}

#define JPG_BUFFER_SIZE 2048

//================================================================//
// jpeg_usstream_source
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
		src->pub.next_input_byte = ( JOCTET* )(( size_t )src->pub.next_input_byte + num_bytes );
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
static void _set_jpeg_usstream_source ( j_decompress_ptr cinfo, ZLStream* stream ) {

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
// MOAIImageFormatJpg
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageFormatJpg::CheckHeader ( const void* buffer ) {

	static u8 magic [] = { 0xFF, 0xD8, 0xFF }; // <?> <?> <?> <?>
	return ( memcmp ( buffer, magic, 3 ) == 0 ) && ((( unsigned char* )buffer )[ 3 ] >= 0xe0 && (( unsigned char* )buffer )[ 3 ] <= 0xef );
}

//----------------------------------------------------------------//
bool MOAIImageFormatJpg::CreateTexture ( MOAITextureBase& texture, const void* data, size_t size ) {
	UNUSED ( texture );
	UNUSED ( data );
	UNUSED ( size );
	
	return false;
}

//----------------------------------------------------------------//
size_t MOAIImageFormatJpg::GetHeaderSize () {

	return HEADER_SIZE;
}

//----------------------------------------------------------------//
MOAIImageFormatJpg::MOAIImageFormatJpg () {
}

//----------------------------------------------------------------//
MOAIImageFormatJpg::~MOAIImageFormatJpg () {
}

//----------------------------------------------------------------//
bool MOAIImageFormatJpg::ReadImage ( MOAIImage& image, ZLStream& stream, u32 transform ) {

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error ( &jerr );
	jpeg_create_decompress ( &cinfo );

	_set_jpeg_usstream_source ( &cinfo, &stream );
	jpeg_read_header ( &cinfo, TRUE );
	
	jpeg_start_decompress ( &cinfo );
	
	this->ReadImageJpg ( image, &cinfo, transform );
	
	jpeg_finish_decompress ( &cinfo );
	jpeg_destroy_decompress ( &cinfo );

	return true;	// Don't really know if this worked, but we need to return something
}

//----------------------------------------------------------------//
void MOAIImageFormatJpg::ReadImageJpg ( MOAIImage& image, void* jpgInfoParam, u32 transform ) {
	
	jpeg_decompress_struct* cinfo = ( jpeg_decompress_struct* )jpgInfoParam;
	
	JDIMENSION width = cinfo->output_width;
	JDIMENSION height = cinfo->output_height;
	
	// set the dimensions, and padding (if any )
	bool isPadded = this->SetDimensions ( image, width, height, transform );
	
	ZLColor::ColorFormat jpgColorFormat;
	
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
	this->SetPixelFormat ( image, MOAIImage::TRUECOLOR );
	this->SetColorFormat ( image, jpgColorFormat );
	
	if (( transform & MOAIImageTransform::QUANTIZE ) && ( ZLColor::GetDepthInBits ( jpgColorFormat ) > 16 )) {
		this->SetColorFormat ( image, ZLColor::RGB_565 );
	}
		
	this->Alloc ( image );
	if ( isPadded ) {
		image.ClearBitmap ();
	}
	
	size_t srcRowSize = cinfo->output_width * cinfo->output_components;
	
	if ( image.GetColorFormat () == jpgColorFormat ) {
		
		if ( image.GetRowSize () < srcRowSize ) return;
		
		for ( u32 y = 0; y < height; ++y ) {
			JSAMPROW row = ( JSAMPROW )this->GetRowAddr ( image, y );
			jpeg_read_scanlines ( cinfo, &row, 1 );
		}
		
		if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
			for ( u32 y = 0; y < height; ++y ) {
				void* row = this->GetRowAddr ( image, y );
				ZLColor::PremultiplyAlpha ( row, image.GetColorFormat (), width );
			}
		}
	}
	else {
		
		void* rowBuffer = malloc ( srcRowSize );
		JSAMPROW samprow = ( JSAMPROW )rowBuffer;
		
		for ( u32 y = 0; y < height; ++y ) {
			jpeg_read_scanlines ( cinfo, &samprow, 1 );
			void* destRow = this->GetRowAddr ( image, y );
			ZLColor::Convert ( destRow, image.GetColorFormat (), rowBuffer, jpgColorFormat, width );
			
			if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
				ZLColor::PremultiplyAlpha ( destRow, image.GetColorFormat (), width );
			}
		}
	}
}

//----------------------------------------------------------------//
bool MOAIImageFormatJpg::WriteImage ( const MOAIImage& image, ZLStream& stream ) {
	UNUSED ( image );
	UNUSED ( stream );
	return false;
}

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

#endif
