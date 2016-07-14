// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-tga/MOAIImageFormatTga.h>

#include <tga_reader.h>

//================================================================//
// MOAIImageFormatTga
//================================================================//

//----------------------------------------------------------------//
bool MOAIImageFormatTga::CreateTexture ( MOAISingleTexture& texture, const void* data, size_t size ) {
	UNUSED ( texture );
	UNUSED ( data );
	UNUSED ( size );
	
	return false;
}

//----------------------------------------------------------------//
size_t MOAIImageFormatTga::GetHeaderGuessSize () {

	return 18;
}

//----------------------------------------------------------------//
bool MOAIImageFormatTga::GuessHeader ( const void* buffer ) {

	cc8* bytes = ( cc8* )buffer;

	uint	coMapType		= ( uint )bytes [ 1 ];
	uint	imgType			= ( uint )bytes [ 2 ];
	uint	index			= ( uint )bytes [ 3 ] + (( uint )bytes [ 4 ] * 256 );
	uint	length			= ( uint )bytes [ 5 ] + (( uint )bytes [ 6 ] * 256 );
	uint	colorSize		= ( uint )bytes [ 7 ];
	int		xOrg			= ( int )bytes [ 8 ] + (( int )bytes [ 9 ] * 256 );
	int		yOrg			= ( int )bytes [ 10 ] + (( int )bytes [ 11 ] * 256 );
	uint	width			= ( uint )bytes [ 12 ] + (( uint )bytes [ 13 ] * 256 );
	uint	height			= ( uint )bytes [ 14 ] + (( uint )bytes [ 15 ] * 256 );
	uint	pixelSize		= ( uint )bytes [ 16 ];
	//uint	attBits			= ( uint )bytes [ 17 ] & 0xf;
	uint	intrLve			= (( uint )bytes [ 17 ] & 0xc0 ) >> 6;

	if ( !(( coMapType == 0 ) || ( coMapType == 1 ))) return false;

	switch ( imgType ) {
	
		case 1: // Uncompressed, color-mapped images.
		case 2: // Uncompressed, RGB images.
		case 3: // Uncompressed, black and white images.
		case 9: // Runlength encoded color-mapped images.
		case 10: // Runlength encoded RGB images.
		case 11: // Compressed, black and white images.
		
		// unsupported
		//case 32: // Compressed color-mapped data, using Huffman, Delta, and runlength encoding.
		//case 33: // Compressed color-mapped data, using Huffman, Delta, and runlength encoding.  4-pass quadtree-type process.
			break;
		default:
			return false;
	}
	
	switch ( colorSize ) {
		case 0:
		case 15:
		case 16:
		case 24:
		case 32:
			break;
		default:
			return false;
	}
	
	switch ( pixelSize ) {
		case 8:
		case 16:
		case 24:
		case 32:
			break;
		default:
			return false;
	}

	switch ( intrLve ) {
		case 0: // non-interleaved.
		case 1: // two-way (even/odd) interleaving.
		case 2: // four way interleaving.
			break;
		default:
			return false;
	}

	if ( imgType == 1 || imgType == 9 ) {
		if ( coMapType != 1 || colorSize == 0 ) return false;
	}
	else {
		// TODO: ?
		//if ( coMapType != 0 || colorSize == 0 ) return false;
	}

	if ( coMapType != 0 && pixelSize > 16 ) return false;

	if (( index + length ) > 65535 ||
		( xOrg + width ) > 65535 ||
		( yOrg + height ) > 65535 ) return false;

	return true;
}

//----------------------------------------------------------------//
MOAIImageFormatTga::MOAIImageFormatTga () {
}

//----------------------------------------------------------------//
MOAIImageFormatTga::~MOAIImageFormatTga () {
}

//----------------------------------------------------------------//
bool MOAIImageFormatTga::ReadImage ( MOAIImage& image, ZLStream& stream, u32 transform ) {

	size_t bufferSize = stream.GetLength () - stream.GetCursor ();
	void* buffer = malloc ( bufferSize );
	stream.ReadBytes ( buffer, bufferSize );

	u32 width	= ( u32 )tgaGetWidth (( u8* )buffer );
	u32 height	= ( u32 )tgaGetHeight (( u8* )buffer );

	void* pixels = tgaRead (( u8* )buffer, TGA_READER_ABGR );

	free ( buffer );
	
	if ( pixels ) {

		image.Init ( pixels, width, height, ZLColor::RGBA_8888 );
		image.Transform ( transform );
		
		tgaFree ( pixels );
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIImageFormatTga::WriteImage ( const MOAIImage& image, ZLStream& stream ) {
	UNUSED ( image );
	UNUSED ( stream );

	return false;
}
