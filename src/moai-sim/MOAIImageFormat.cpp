// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIImageFormat.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// MOAIImageFormat
//================================================================//

//----------------------------------------------------------------//
void MOAIImageFormat::Alloc ( MOAIImage& image ) const {

	image.Alloc ();
}

//----------------------------------------------------------------//
void MOAIImageFormat::CleanupTexture ( MOAITextureBase& texture ) {

	texture.CleanupOnError ();
}

//----------------------------------------------------------------//
bool MOAIImageFormat::CreateTexture ( MOAITextureBase& texture, const void* data, size_t size ) {
	UNUSED ( texture );
	UNUSED ( data );
	UNUSED ( size );
	
	return false;
}

//----------------------------------------------------------------//
void* MOAIImageFormat::GetBitmap ( MOAIImage& image ) const {

	return image.mBitmap;
}

//----------------------------------------------------------------//
void* MOAIImageFormat::GetRowAddr ( MOAIImage& image, u32 y ) const {

	return image.GetRowAddr ( y );
}

//----------------------------------------------------------------//
const void* MOAIImageFormat::GetRowAddr ( const MOAIImage& image, u32 y ) const {

	return image.GetRowAddr ( y );
}

//----------------------------------------------------------------//
bool MOAIImageFormat::GetTextureInfo ( ZLStream& stream, MOAITextureInfo& info ) {
	UNUSED ( stream );
	UNUSED ( info );
	
	return false;
}

//----------------------------------------------------------------//
MOAIImageFormat::MOAIImageFormat () {
}

//----------------------------------------------------------------//
MOAIImageFormat::~MOAIImageFormat () {
}

//----------------------------------------------------------------//
void MOAIImageFormat::SetColorFormat ( MOAIImage& image, ZLColor::ColorFormat colorFormat ) const {

	image.SetColorFormat ( colorFormat );
}

//----------------------------------------------------------------//
bool MOAIImageFormat::SetDimensions ( MOAIImage& image, u32 width, u32 height, u32 transform ) const {

	// set the dimensions, and padding (if any )
	bool isPadded = false;
	
	if ( transform & MOAIImageTransform::POW_TWO ) {
	
		image.SetWidth ( MOAIImage::GetMinPowerOfTwo ( width ));
		image.SetHeight ( MOAIImage::GetMinPowerOfTwo ( height ));
		isPadded = true;
	}
	else {
		image.SetWidth ( width );
		image.SetHeight ( height );
	}
	return isPadded;
}

//----------------------------------------------------------------//
void MOAIImageFormat::SetPixelFormat ( MOAIImage& image, MOAIImage::PixelFormat pixelFormat ) const {

	image.SetPixelFormat ( pixelFormat );
}

//----------------------------------------------------------------//
void MOAIImageFormat::SetTextureID ( MOAITextureBase& texture, u32 glTexID, int internalFormat, int pixelType, size_t textureSize ) {

	texture.SetTextureID ( glTexID, internalFormat, pixelType, textureSize );
}

//----------------------------------------------------------------//
bool MOAIImageFormat::ReadImage ( MOAIImage& image, ZLStream& stream, u32 transform ) {
	UNUSED ( image );
	UNUSED ( stream );
	UNUSED ( transform );
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageFormat::WriteImage ( const MOAIImage& image, ZLStream& stream ) {
	UNUSED ( image );
	UNUSED ( stream );
	return false;
}
