// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-core/ZLImageFormat.h>

//================================================================//
// ZLImageFormat
//================================================================//

//----------------------------------------------------------------//
void ZLImageFormat::Alloc ( ZLImage& image ) const {

	image.Alloc ();
}

//----------------------------------------------------------------//
bool ZLImageFormat::CheckHeader ( const void* buffer ) {
	UNUSED ( buffer );

	return false;
}

//----------------------------------------------------------------//
const void* ZLImageFormat::GetBitmap ( ZLImage& image ) const {

	return image.mBitmap.GetConstBuffer ();
}

//----------------------------------------------------------------//
void* ZLImageFormat::GetBitmapMutable ( ZLImage& image ) const {

	return image.mBitmap.Invalidate ();
}

//----------------------------------------------------------------//
size_t ZLImageFormat::GetHeaderSize () {

	return 0;
}

//----------------------------------------------------------------//
size_t ZLImageFormat::GetHeaderGuessSize () {

	return 0;
}

//----------------------------------------------------------------//
const void* ZLImageFormat::GetRowAddr ( const ZLImage& image, u32 y ) const {

	return image.GetRowAddr ( y );
}

//----------------------------------------------------------------//
void* ZLImageFormat::GetRowAddrMutable ( ZLImage& image, u32 y ) const {

	return image.GetRowAddrMutable ( y );
}

//----------------------------------------------------------------//
bool ZLImageFormat::GuessHeader ( const void* buffer ) {
	UNUSED ( buffer );

	return false;
}

//----------------------------------------------------------------//
ZLImageFormat::ZLImageFormat () {

	RTTI_SINGLE ( ZLImageFormat, RTTIBase )
}

//----------------------------------------------------------------//
ZLImageFormat::~ZLImageFormat () {
}

//----------------------------------------------------------------//
void ZLImageFormat::SetColorFormat ( ZLImage& image, ZLColor::ColorFormat colorFormat ) const {

	image.SetColorFormat ( colorFormat );
}

//----------------------------------------------------------------//
bool ZLImageFormat::SetDimensions ( ZLImage& image, u32 width, u32 height, u32 transform ) const {

	// set the dimensions, and padding (if any )
	bool isPadded = false;
	
	if ( transform & ZLImageTransform::POW_TWO ) {
	
		image.SetWidth ( ZLImage::GetMinPowerOfTwo ( width ));
		image.SetHeight ( ZLImage::GetMinPowerOfTwo ( height ));
		isPadded = true;
	}
	else {
		image.SetWidth ( width );
		image.SetHeight ( height );
	}
	return isPadded;
}

//----------------------------------------------------------------//
void ZLImageFormat::SetPixelFormat ( ZLImage& image, ZLImage::PixelFormat pixelFormat ) const {

	image.SetPixelFormat ( pixelFormat );
}

//----------------------------------------------------------------//
bool ZLImageFormat::ReadImage ( ZLImage& image, ZLStream& stream, u32 transform ) {
	UNUSED ( image );
	UNUSED ( stream );
	UNUSED ( transform );
	return false;
}

//----------------------------------------------------------------//
bool ZLImageFormat::WriteImage ( const ZLImage& image, ZLStream& stream ) {
	UNUSED ( image );
	UNUSED ( stream );
	return false;
}
