// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIImage.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBWEBP

extern "C" {
#include "libwebp-0.4.1/src/webp/demux.h"
#include "libwebp-0.4.1/src/webp/decode.h"
}

//================================================================//
// MOAIImage-webp
//================================================================//

//----------------------------------------------------------------//
bool MOAIImage::IsWebP ( ZLStream& stream ) {

	u32 buffer[3];
	u32 size = stream.PeekBytes ( buffer, 12 );
	if ( size < 12 ) return false;

	return ( buffer[0] == *( ( u32* )"RIFF" ) && buffer[2] == *( ( u32* )"WEBP" ) );
}

//----------------------------------------------------------------//
void MOAIImage::LoadWebP ( ZLStream& stream, u32 transform ) {
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
		this->LoadWebP ( iter.fragment.bytes, iter.fragment.size, iter.width, iter.height, iter.has_alpha != 0, transform );
	}

	// Clean up
	WebPDemuxReleaseIterator ( &iter );
	WebPDemuxDelete ( demux );
	WebPDataClear ( &data );
}

void MOAIImage::LoadWebP ( u8 const* data, size_t dataSize, int width, int height, bool hasAlpha, u32 transform ) {
	// Pad to POT dimensions, if requested
	bool isPadded = false;
	if ( transform & MOAIImageTransform::POW_TWO ) {
		this->mWidth = this->GetMinPowerOfTwo ( width );
		this->mHeight = this->GetMinPowerOfTwo ( height );
		isPadded = this->mWidth != width || this->mHeight != height;
	}
	else {
		this->mWidth = width;
		this->mHeight = height;
	}

	// Set pixel and color format
	this->mPixelFormat = USPixel::TRUECOLOR;
	bool quantize = (transform & MOAIImageTransform::QUANTIZE) != 0;
	if ( hasAlpha ) {
		this->mColorFormat = quantize ? ZLColor::RGBA_4444 : ZLColor::RGBA_8888;
	}
	else {
		this->mColorFormat = quantize ? ZLColor::RGB_565 : ZLColor::RGB_888;
	}

	// Initialize image buffer
	this->Alloc ();
	if ( isPadded ) {
		this->ClearBitmap ();
	}

	// Create decoder configuration
	WebPDecoderConfig config;
	if ( !WebPInitDecoderConfig ( &config ) ) return;
	bool premultiply = (transform & MOAIImageTransform::PREMULTIPLY_ALPHA) != 0;
	switch ( this->mColorFormat ) {
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
	config.output.u.RGBA.rgba = ( u8* ) this->mData;
	config.output.u.RGBA.stride = this->GetRowSize ();
	config.output.u.RGBA.size = this->GetBitmapSize ();
	config.output.is_external_memory = true;

	// Decode the image
	WebPDecode ( data, dataSize, &config );

	// Clean up
	WebPFreeDecBuffer ( &config.output );
}

#endif