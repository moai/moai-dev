// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBWEBP

#include <moai-sim/MOAIImageFormatWebP.h>

extern "C" {
	#include "libwebp-0.4.1/src/webp/demux.h"
	#include "libwebp-0.4.1/src/webp/decode.h"
}

//================================================================//
// MOAIImageFormatWebP
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageFormatWebP::CheckHeader ( const void* buffer ) {

	return ((( u32* )buffer )[ 0 ] == *(( u32* )"RIFF" ) && (( u32* )buffer )[ 2 ] == *(( u32* )"WEBP" ));
}

//----------------------------------------------------------------//
bool MOAIImageFormatWebP::CreateTexture ( MOAITextureBase& texture, const void* data, size_t size ) {
	UNUSED ( texture );
	UNUSED ( data );
	UNUSED ( size );
	
	return false;
}

//----------------------------------------------------------------//
size_t MOAIImageFormatWebP::GetHeaderSize () {

	return HEADER_SIZE;
}

//----------------------------------------------------------------//
MOAIImageFormatWebP::MOAIImageFormatWebP () {
}

//----------------------------------------------------------------//
MOAIImageFormatWebP::~MOAIImageFormatWebP () {
}

//----------------------------------------------------------------//
bool MOAIImageFormatWebP::ReadImage ( MOAIImage& image, ZLStream& stream, u32 transform ) {

	// Copy data from stream to WebPData object
	WebPData data;
	data.size = stream.GetLength ();
	data.bytes = ( uint8_t* )malloc ( data.size );
	if ( !data.bytes ) return;
	stream.ReadBytes ( ( void* )data.bytes, data.size );

	// Create demuxer
	WebPDemuxer* demux = WebPDemux ( &data );

	// Read first frame (ignore any additional frames)
	WebPIterator iter;
	if ( WebPDemuxGetFrame ( demux, 1, &iter ) ) {
		this->ReadImageWebP ( image, iter.fragment.bytes, iter.fragment.size, iter.width, iter.height, iter.has_alpha != 0, transform );
	}

	// Clean up
	WebPDemuxReleaseIterator ( &iter );
	WebPDemuxDelete ( demux );
	WebPDataClear ( &data );
}

//----------------------------------------------------------------//
void MOAIImageFormatWebP::ReadImageWebP ( MOAIImage& image, u8 const* data, size_t dataSize, int width, int height, bool hasAlpha, u32 transform ) {

	// Pad to POT dimensions, if requested
	bool isPadded = this->SetDimensions ( image, width, height, transform );

	// Set pixel and color format
	this->SetPixelFormat ( image, MOAIImage::TRUECOLOR );
	bool quantize = ( transform & MOAIImageTransform::QUANTIZE ) != 0;
	if ( hasAlpha ) {
		this->SetColorFormat ( image, quantize ? ZLColor::RGBA_4444 : ZLColor::RGBA_8888 );
	}
	else {
		this->SetColorFormat ( image, quantize ? ZLColor::RGB_565 : ZLColor::RGB_888 );
	}

	// Initialize image buffer
	this->Alloc ( image );
	
	if ( isPadded ) {
		image.ClearBitmap ();
	}

	// Create decoder configuration
	WebPDecoderConfig config;
	if ( !WebPInitDecoderConfig ( &config ) ) return;
	bool premultiply = (transform & MOAIImageTransform::PREMULTIPLY_ALPHA) != 0;
	switch ( image.GetColorFormat ()) {
		case ZLColor::RGBA_8888:
			config.output.colorspace = premultiply ? MODE_rgbA : MODE_RGBA;
			break;
		case ZLColor::RGBA_4444:
			config.output.colorspace = premultiply ? MODE_rgbA_4444 : MODE_RGBA_4444;
			break;
		case ZLColor::RGB_888:
			config.output.colorspace = MODE_RGB;
			break;
		case ZLColor::RGB_565:
			config.output.colorspace = MODE_RGB_565;
			break;
	}
	config.output.u.RGBA.rgba = ( u8* )this->GetBitmap ( image );
	config.output.u.RGBA.stride = image.GetRowSize ();
	config.output.u.RGBA.size = image.GetBitmapSize ();
	config.output.is_external_memory = true;

	// Decode the image
	WebPDecode ( data, dataSize, &config );

	// Clean up
	WebPFreeDecBuffer ( &config.output );
}

//----------------------------------------------------------------//
bool MOAIImageFormatWebP::WriteImage ( const MOAIImage& image, ZLStream& stream ) {
	UNUSED ( image );
	UNUSED ( stream );
	return false;
}

#endif