// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <png.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIImage.h>

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

	USStream* stream = ( USStream* )png_get_io_ptr ( png );
	stream->Flush ();
}

//----------------------------------------------------------------//
static void _pngRead ( png_structp png, png_bytep buffer, png_size_t size ) {

	USStream* stream = ( USStream* )png_get_io_ptr ( png );
	stream->ReadBytes ( buffer, ( u32 )size );
}

//----------------------------------------------------------------//
static void _pngWrite ( png_structp png, png_bytep buffer, png_size_t size ) {

	USStream* stream = ( USStream* )png_get_io_ptr ( png );
	stream->WriteBytes ( buffer, ( u32 )size );
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	bleedRect
	@text	'Bleeds' the interior of the rectangle out by one pixel.

	@in		MOAIImage self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIImage::_bleedRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	int xMin	= state.GetValue < int >( 2, 0 );
	int yMin	= state.GetValue < int >( 3, 0 );
	int xMax	= state.GetValue < int >( 4, 0 );
	int yMax	= state.GetValue < int >( 5, 0 );
	
	self->BleedRect ( xMin, yMin, xMax, yMax );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	convertColors
	@text	Return a copy of the image with a new color format. Not
			all provided formats are supported by OpenGL.

	@in		MOAIImage self
	@in		number colorFmt		One of MOAIImage.COLOR_FMT_A_8, MOAIImage.COLOR_FMT_RGB_888, MOAIImage.COLOR_FMT_RGB_565,
								MOAIImage.COLOR_FMT_RGBA_5551, MOAIImage.COLOR_FMT_RGBA_4444, COLOR_FMT_RGBA_8888
	@out	MOAIImage image		Copy of the image initialized with given format.
*/
int MOAIImage::_convertColors ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UN" )
	
	u32 colorFmt = state.GetValue < u32 >( 2, USColor::RGBA_8888 );
	
	MOAIImage* image = new MOAIImage ();
	image->ConvertColors ( *self, ( USColor::Format )colorFmt );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	copy
	@text	Copies an image.

	@in		MOAIImage self
	@out	MOAIImage image		Copy of the image.
*/
int MOAIImage::_copy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	MOAIImage* image = new MOAIImage ();
	image->Copy ( *self );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	copyBits
	@text	Copy a section of one image to another.

	@in		MOAIImage self
	@in		MOAIImage source	Source image.
	@in		number srcX			X location in source image.
	@in		number srcY			Y location in source image.
	@in		number destX		X location in destination image.
	@in		number destY		Y location in destination image.
	@in		number width		Width of section to copy.
	@in		number height		Height of section to copy.
	@out	nil
*/
int MOAIImage::_copyBits ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UUNNNNNN" )
	
	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2 );
	if ( !image ) {
		return 0;
	}
	
	int srcX	= state.GetValue < int >( 3, 0 );
	int srcY	= state.GetValue < int >( 4, 0 );
	
	int destX	= state.GetValue < int >( 5, 0 );
	int destY	= state.GetValue < int >( 6, 0 );
	
	int width	= state.GetValue < int >( 7, 0 );
	int height	= state.GetValue < int >( 8, 0 );
	
	self->CopyBits ( *image, srcX, srcY, destX, destY, width, height );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getColor32
	@text	Returns a 32-bit packed RGBA value from the image for a
			given pixel coordinate.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@out	number color
*/
int MOAIImage::_getColor32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )
	
	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );

	u32 color = self->GetColor ( x, y );
	lua_pushnumber ( state, color );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getFormat
	@text	Returns the color format of the image.

	@in		MOAIImage self
	@out	number colorFormat
*/
int MOAIImage::_getFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	lua_pushnumber ( state, self->GetColorFormat ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRGBA
	@text	Returns an RGBA color as four floating point values.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAIImage::_getRGBA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )
	
	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );

	u32 color = self->GetColor ( x, y );
	
	USColorVec colorVec;
	colorVec.SetRGBA ( color );
	
	lua_pushnumber ( state, colorVec.mR );
	lua_pushnumber ( state, colorVec.mG );
	lua_pushnumber ( state, colorVec.mB );
	lua_pushnumber ( state, colorVec.mA );
	
	return 4;
}

//----------------------------------------------------------------//
/**	@name	getSize
	@text	Returns the width and height of the image.

	@in		MOAIImage self
	@out	number width
	@out	number height
*/
int MOAIImage::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	u32 width	= self->GetWidth ();
	u32 height	= self->GetHeight ();

	lua_pushnumber ( state, width );
	lua_pushnumber ( state, height );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initializes the image with a width, height and color format.

	@in		MOAIImage self
	@in		number width
	@in		number height
	@opt	colorFmt		One of MOAIImage.COLOR_FMT_A_8, MOAIImage.COLOR_FMT_RGB_888, MOAIImage.COLOR_FMT_RGB_565,
							MOAIImage.COLOR_FMT_RGBA_5551, MOAIImage.COLOR_FMT_RGBA_4444, COLOR_FMT_RGBA_8888
	@out	nil
*/
int MOAIImage::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )

	u32 width		= state.GetValue < u32 >( 2, 0 );
	u32 height		= state.GetValue < u32 >( 3, 0 );
	u32 colorFmt	= state.GetValue < u32 >( 4, USColor::RGBA_8888 );

	self->Init ( width, height, ( USColor::Format )colorFmt, USPixel::TRUECOLOR );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads an image from a PNG.

	@in		MOAIImage self
	@in		string filename
	@opt	number transform	One of MOAIImage.POW_TWO, One of MOAIImage.QUANTIZE,
								One of MOAIImage.TRUECOLOR, One of MOAIImage.PREMULTIPLY_ALPHA
	@out	nil
*/
int MOAIImage::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "US" )

	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	u32 transform	= state.GetValue < u32 >( 3, 0 );

	self->Load ( filename, transform );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	copy
	@text	Copies an image and returns a new image padded to the next
			power of 2 along each dimension. Original image will be
			in the upper left hand corner of the new image.

	@in		MOAIImage self
	@out	MOAIImage image		Copy of the image padded to the next nearest power
								of two along each dimension.
*/
int MOAIImage::_padToPow2 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "U" )
	
	MOAIImage* image = new MOAIImage ();
	image->PadToPow2 ( *self );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	resizeCanvas
	@text	Copies the image to a canvas with a new size. If the canvas
			is larger than the original image, the exta pixels will be
			initialized with 0. Pass in a new frame or just a new width
			and height. Negative values are permitted for the frame.

	@overload

		@in		MOAIImage self
		@in		number width		New width of the image.
		@in		number height		New height of the image.
		@out	MOAIImage image

	@overload

		@in		MOAIImage self
		@in		number xMin
		@in		number yMin
		@in		number xMax
		@in		number yMax
		@out	MOAIImage image
*/
int MOAIImage::_resizeCanvas ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNN" )
	
	USIntRect rect;
	
	if ( state.CheckParams ( 4, "NN" )) {
		rect.mXMin	= state.GetValue < int >( 2, 0 );
		rect.mYMin	= state.GetValue < int >( 3, 0 );
		rect.mXMax	= state.GetValue < int >( 4, 0 );
		rect.mYMax	= state.GetValue < int >( 5, 0 );
	}
	else {
		rect.mXMin	= 0;
		rect.mYMin	= 0;
		rect.mXMax	= state.GetValue < int >( 2, 0 );
		rect.mYMax	= state.GetValue < int >( 3, 0 );
	}
	
	MOAIImage* image = new MOAIImage ();
	image->ResizeCanvas ( *self, rect );
	image->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setColor32
	@text	Sets 32-bit the packed RGBA value for a given pixel
			coordinate. Parameter will be converted to the native format
			of the image.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@in		number color
	@out	nil
*/
int MOAIImage::_setColor32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNN" )

	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );
	u32 color	= state.GetValue < u32 >( 4, 0 );

	self->SetColor ( x, y, color );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRGBA
	@text	Sets a color using RGBA floating point values.

	@in		MOAIImage self
	@in		number x
	@in		number y
	@in		number r
	@in		number g
	@in		number b
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIImage::_setRGBA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "UNNNNN" )

	u32 x		= state.GetValue < u32 >( 2, 0 );
	u32 y		= state.GetValue < u32 >( 3, 0 );
	
	float r		= state.GetValue < float >( 4, 0.0f );
	float g		= state.GetValue < float >( 5, 0.0f );
	float b		= state.GetValue < float >( 6, 0.0f );
	float a		= state.GetValue < float >( 7, 1.0f );

	self->SetColor ( x, y, USColor::PackRGBA ( r, g, b, a ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	writePNG
	@text	Write image to a PNG file.

	@in		MOAIImage self
	@in		string filename
	@out	nil
*/
int MOAIImage::_writePNG ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIImage, "US" )
	
	cc8* filename = state.GetValue < cc8* >( 2, "" );
	
	USFileStream stream;
	stream.OpenWrite ( filename );
	self->WritePNG ( stream );
	
	return 0;
}

//================================================================//
// MOAIImage
//================================================================//

//----------------------------------------------------------------//
void MOAIImage::Alloc () {

	if ( this->mData ) {
		free ( this->mData );
	}

	u32 paletteSize = this->GetPaletteSize ();
	u32 bitmapSize = this->GetBitmapSize ();

	this->mData = malloc ( paletteSize + bitmapSize );
	this->mBitmap = ( void* )(( uintptr )this->mData + paletteSize );
	
	if ( paletteSize ) {
		this->mPalette = this->mData;
		memset ( this->mPalette, 0, paletteSize );
	}
	else {
		this->mPalette = 0;
	}
}

//----------------------------------------------------------------//
void MOAIImage::BleedRect ( int xMin, int yMin, int xMax, int yMax ) {

	float pixelSize = USPixel::GetSize ( this->mPixelFormat, this->mColorFormat );
	if ( pixelSize == 0.5f ) {
		return; // TODO
	}

	int width = ( int )this->mWidth;
	int height = ( int )this->mHeight;

	xMin = ( xMin < 0 ) ? 0 : xMin;
	yMin = ( yMin < 0 ) ? 0 : yMin;
	
	xMax = ( xMax > width ) ? width : xMax;
	yMax = ( yMax > height ) ? height : yMax;

	if ( xMax <= xMin ) return;
	if ( yMax <= yMin ) return;

	if ( xMin >= width ) return;
	if ( yMin >= height ) return;
	
	if ( xMax <= 0 ) return;
	if ( yMax <= 0 ) return;
	
	if (( xMin > 0 ) || ( xMax < width )) {
		for ( int y = yMin; y < yMax; ++y ) {
			if ( xMin > 0 ) {
				this->SetPixel ( xMin - 1, y, this->GetPixel ( xMin, y ));
			}
			
			if ( xMax < width ) {
				this->SetPixel ( xMax, y, this->GetPixel ( xMax - 1, y ));
			}
		}
	}
	
	xMin = ( xMin > 1 ) ? xMin - 1 : 0;
	xMax = ( xMax < width ) ? xMax + 1 : width;
	
	u32 pixSize = ( u32 )pixelSize;
	u32 rowSize = this->GetRowSize ();
	u32 copySize = ( u32 )(( xMax - xMin ) * pixelSize );
	
	if ( yMin > 0 ) {
		void* srcRow = ( void* )(( uintptr )this->mBitmap + ( rowSize * yMin ) + ( xMin * pixSize ));
		void* destRow = ( void* )(( uintptr )this->mBitmap + ( rowSize * ( yMin - 1 )) + ( xMin * pixSize ));
		memcpy ( destRow, srcRow, copySize );
	}
	
	if ( yMax < height ) {
		void* srcRow = ( void* )(( uintptr )this->mBitmap + ( rowSize * ( yMax - 1 )) + ( xMin * pixSize ));
		void* destRow = ( void* )(( uintptr )this->mBitmap + ( rowSize * yMax ) + ( xMin * pixSize ));
		memcpy ( destRow, srcRow, copySize );
	}
}

//----------------------------------------------------------------//
void MOAIImage::Clear () {

	if ( this->mData ) {
		free ( this->mData );
	}
	
	this->mColorFormat	= USColor::CLR_FMT_UNKNOWN;
	this->mPixelFormat	= USPixel::PXL_FMT_UNKNOWN;

	this->mWidth		= 0;
	this->mHeight		= 0;
	
	this->mData			= 0;
	this->mBitmap		= 0;
	this->mPalette		= 0;
}

//----------------------------------------------------------------//
void MOAIImage::ClearBitmap () {

	if ( this->mBitmap ) {
		memset ( this->mBitmap, 0, this->GetBitmapSize ());
	}
}

//----------------------------------------------------------------//
void MOAIImage::ConvertColors ( const MOAIImage& image, USColor::Format colorFmt ) {
	
	if ( colorFmt == image.mColorFormat ) {
		this->Copy ( image );
	}
	else {
	
		this->Init ( image.mWidth, image.mHeight, colorFmt, image.mPixelFormat );
		
		if ( this->mPixelFormat == USPixel::TRUECOLOR ) {
			u32 total = this->mWidth * this->mHeight;
			USColor::Convert ( this->mBitmap, this->mColorFormat, image.mBitmap, image.mColorFormat, total );
		}
		else {
			u32 total = this->GetPaletteCount ();
			USColor::Convert ( this->mPalette, this->mColorFormat, image.mPalette, image.mColorFormat, total );
		}
	}
}

//----------------------------------------------------------------//
void MOAIImage::Copy ( const MOAIImage& image ) {

	this->Init ( image.mWidth, image.mHeight, image.mColorFormat, image.mPixelFormat );
	
	memcpy ( this->mData, image.mData, this->GetBitmapSize () + this->GetPaletteSize ());
}

//----------------------------------------------------------------//
void MOAIImage::CopyBits ( const MOAIImage& image, int srcX, int srcY, int destX, int destY, int width, int height ) {

	if ( !(( this->mPixelFormat == image.mPixelFormat ) && ( this->mColorFormat == image.mColorFormat ))) {
		return; // TODO
	}

	int srcXMax = srcX + width;
	int srcYMax = srcY + height;

	if ( srcX < 0 ) {
		destX -= srcX;
		width += srcX;
		srcX = 0;
	}
	
	if ( srcY < 0 ) {
		destY -= srcY;
		height += srcY;
		srcY = 0;
	}
	
	if ( srcXMax > ( int )image.mWidth ) {
		width -= srcXMax - ( int )image.mWidth;
	}
	
	if ( srcYMax > ( int )image.mHeight ) {
		height -= srcYMax - ( int )image.mHeight;
	}
	
	int destXMax = destX + width;
	int destYMax = destY + height;
	
	if ( destX < 0 ) {
		srcX -= destX;
		width += destX;
		destX = 0;
	}
	
	if ( destY < 0 ) {
		srcY -= destY;
		height += destY;
		destY = 0;
	}
	
	if ( destXMax > ( int )this->mWidth ) {
		width -= destXMax - ( int )this->mWidth;
	}
	
	if ( destYMax > ( int )this->mHeight ) {
		height -= destYMax - ( int )this->mHeight;
	}
	
	if ( width <= 0 ) return;
	if ( height <= 0 ) return;
	
	if ( srcX >= ( int )image.mWidth ) return;
	if ( srcY >= ( int )image.mHeight ) return;
	
	if ( destX >= ( int )this->mWidth ) return;
	if ( destY >= ( int )this->mHeight ) return;
	
	float pixelSize = USPixel::GetSize ( this->mPixelFormat, this->mColorFormat );
	
	if ( pixelSize == 0.5f ) {
		return; // TODO
	}
	else {
		
		u32 size = ( u32 )pixelSize;
		u32 srcRowSize = image.GetRowSize ();
		u32 destRowSize = this->GetRowSize ();
		
		width *= size;
		
		for ( int y = 0; y < height; ++y ) {
		
			const void* srcRow = ( const void* )(( uintptr )image.mBitmap + ( srcRowSize * ( y + srcY )) + ( srcX * size ));
			void* destRow = ( void* )(( uintptr )this->mBitmap + ( destRowSize * ( y + destY )) + ( destX * size ));
			
			memcpy ( destRow, srcRow, width );
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIImage::GetBitmapSize () const {

	return this->GetRowSize () * this->mHeight;
}

//----------------------------------------------------------------//
u32 MOAIImage::GetColor ( u32 i ) const {

	u32 colorDepth = USColor::GetDepth ( this->mColorFormat );

	u32 total = this->GetPaletteCount ();
	if ( i < total ) {
		
		u8* stream = ( u8* )(( uintptr )this->mPalette + (( u32 )( colorDepth >> 3 ) * i ));
		return USColor::ReadRGBA ( stream, this->mColorFormat );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIImage::GetColor ( u32 x, u32 y ) const {

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
u32 MOAIImage::GetDataSize () const {

	return this->GetPaletteSize () + this->GetBitmapSize ();
}

//----------------------------------------------------------------//
u32 MOAIImage::GetMinPowerOfTwo ( u32 size ) {

	u32 pow2 = 1;
	while ( pow2 < size ) pow2 = pow2 << 0x01;
	return pow2;
}

//----------------------------------------------------------------//
u32 MOAIImage::GetPaletteCount () const {

	return USPixel::GetPaletteCount ( this->mPixelFormat );
}

//----------------------------------------------------------------//
u32 MOAIImage::GetPaletteColor ( u32 idx ) const {

	u32 rgba = 0;
	u32 total = this->GetPaletteCount ();
	if ( idx < total ) {
		
		u32 colorDepth = USColor::GetDepth ( this->mColorFormat );
		
		u32 size = ( colorDepth >> 3 );
		u8* stream = ( u8* )(( uintptr )this->mPalette + ( idx * size ));
		u32 color = USPixel::ReadPixel ( stream, size );
		
		rgba = USColor::ConvertToRGBA ( color, this->mColorFormat );
	}
	return rgba;
}

//----------------------------------------------------------------//
u32 MOAIImage::GetPaletteSize () const {

	return USPixel::GetPaletteSize ( this->mPixelFormat, this->mColorFormat );
}

//----------------------------------------------------------------//
u32 MOAIImage::GetPixel ( u32 x, u32 y ) const {

	if ( y >= this->mHeight ) return 0;
	if ( x >= this->mWidth ) return 0;

	const void* row = this->GetRowAddr ( y );

	float pixelSize = USPixel::GetSize ( this->mPixelFormat, this->mColorFormat );
	u32 pixel;
	
	if ( pixelSize == 0.5f ) {
		u8* stream = ( u8* )(( uintptr )row + ( x >> 0x01 ));
		pixel = ( u32 )( *stream );
		pixel = (( pixel >> (( x & 0x01 ) ? 0x04 : 0x00 )) & 0x0F );
	}
	else {
		u32 size = ( u32 )pixelSize;
		u8* stream = ( u8* )(( uintptr )row + ( x * size ));
		pixel = USPixel::ReadPixel ( stream, size );
	}
	
	return pixel;
}

//----------------------------------------------------------------//
void* MOAIImage::GetRowAddr ( u32 y ) {

	return ( void* )(( uintptr )this->mBitmap + ( this->GetRowSize () * y ));
}

//----------------------------------------------------------------//
const void* MOAIImage::GetRowAddr ( u32 y ) const {

	return ( void* )(( uintptr )this->mBitmap + ( this->GetRowSize () * y ));
}

//----------------------------------------------------------------//
u32 MOAIImage::GetRowSize () const {

	u32 rowSize = ( this->mWidth * USPixel::GetDepth ( this->mPixelFormat, this->mColorFormat )) >> 3;
	return ( rowSize + ( rowSize & 0x01 ));
}

//----------------------------------------------------------------//
void MOAIImage::Init ( u32 width, u32 height, USColor::Format colorFmt, USPixel::Format pixelFmt ) {

	this->mColorFormat = colorFmt;
	this->mPixelFormat = pixelFmt;

	this->mWidth = width;
	this->mHeight = height;
	
	this->Alloc ();
	this->ClearBitmap ();
}

//----------------------------------------------------------------//
void MOAIImage::Init ( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt ) {

	this->Init ( bitmap, width, height, colorFmt, true );
}

//----------------------------------------------------------------//
void MOAIImage::Init ( const void* bitmap, u32 width, u32 height, USColor::Format colorFmt, bool copy ) {

	this->Clear ();

	if ( !bitmap ) return;

	this->mPixelFormat = USPixel::TRUECOLOR;
	this->mColorFormat = colorFmt;

	this->mWidth = width;
	this->mHeight = height;
	
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
void MOAIImage::Load ( USData& data, u32 transform ) {

	void* bytes;
	u32 size;
	data.Lock ( &bytes, &size );

	this->Load ( bytes, size, transform );
	
	data.Unlock ();
}

//----------------------------------------------------------------//
void MOAIImage::Load ( cc8* filename, u32 transform ) {

	this->Clear ();
	if ( !USFileSys::CheckFileExists ( filename )) return;
	
	USFileStream stream;
	stream.OpenRead ( filename );
	
	u32 size = stream.GetLength ();
	void* buffer = malloc ( size );
	stream.ReadBytes ( buffer, size );
	
	stream.Close ();
	
	this->Load ( buffer, size, transform );
	free ( buffer );
}

//----------------------------------------------------------------//
void MOAIImage::Load ( const void* buffer, u32 size, u32 transform ) {

	this->Clear ();
	if ( size < 8 ) return;
	
	int isPng = ( png_sig_cmp (( png_bytep )buffer, 0, 8 ) == 0 );
	
	if ( isPng ) {
		USByteStream stream;
		stream.SetBuffer (( void* )buffer, size );
		stream.SetLength ( size );
		this->LoadPng ( stream, transform );
	}
}

//----------------------------------------------------------------//
void MOAIImage::LoadPng ( USStream& stream, u32 transform ) {

	png_structp png = png_create_read_struct ( PNG_LIBPNG_VER_STRING, 0, _pngError, 0 );
	if ( !png ) return;

	png_infop pngInfo = png_create_info_struct ( png );
	if ( pngInfo ) {
		png_set_read_fn ( png, &stream, _pngRead );
		this->LoadPng ( png, pngInfo, transform );
	}

	png_destroy_read_struct ( &png, &pngInfo, NULL );
}

//----------------------------------------------------------------//
void MOAIImage::LoadPng ( void* pngParam, void* pngInfoParam, u32 transform ) {
	
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
	
	// now guess the format and color type, according to the png
	USPixel::Format pngPixelFormat;
	USColor::Format pngColorFormat;
	
	switch ( pngColorType ) {
		
		case PNG_COLOR_TYPE_GRAY:
			pngPixelFormat = USPixel::TRUECOLOR;
			pngColorFormat = USColor::A_8;
			break;
		
		case PNG_COLOR_TYPE_PALETTE:
			pngPixelFormat = ( paletteSize > 16 ) ? USPixel::INDEX_8 : USPixel::INDEX_4;
			pngColorFormat = ( transSize ) ? USColor::RGBA_8888 : USColor::RGB_888;
			break;
		
		case PNG_COLOR_TYPE_RGB:
			pngPixelFormat = USPixel::TRUECOLOR;
			pngColorFormat = USColor::RGB_888;
			break;
		
		case PNG_COLOR_TYPE_RGB_ALPHA:
			pngPixelFormat = USPixel::TRUECOLOR;
			pngColorFormat = USColor::RGBA_8888;
			break;
		
		default: return; // unsupported format
	}
	
	// override the image settings
	this->mPixelFormat = ( transform & MOAIImageTransform::TRUECOLOR ) ? USPixel::TRUECOLOR : pngPixelFormat;
	this->mColorFormat = pngColorFormat;
	
	if (( transform & MOAIImageTransform::QUANTIZE ) && ( USColor::GetDepth ( pngColorFormat ) > 16 )) {
		
		switch ( pngColorFormat ) {
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
		if ( isPadded ) {
			this->ClearBitmap ();
		}
		
		if ( this->mColorFormat == pngColorFormat ) {
			
			if ( this->GetRowSize () < png_get_rowbytes ( png, pngInfo )) return;
			
			for ( int i = 0; i < passes; ++i ) {
				for ( u32 y = 0; y < height; ++y ) {
					void* row = this->GetRowAddr ( y );
					png_read_row ( png, ( png_bytep )row, 0 );
				}
			}
			
			if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
				for ( u32 y = 0; y < height; ++y ) {
					void* row = this->GetRowAddr ( y );
					USColor::PremultiplyAlpha ( row, this->mColorFormat, width );
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
						void* srcRow = ( void* )(( uintptr )srcBuff + ( srcRowSize * y ));
						png_read_row ( png, ( png_bytep )srcRow, 0 );
					}
				}
				
				for ( u32 y = 0; y < height; ++y ) {
					void* srcRow = ( void* )(( uintptr )srcBuff + ( srcRowSize * y ));
					void* destRow = this->GetRowAddr ( y );
					USColor::Convert ( destRow, this->mColorFormat, srcRow, pngColorFormat, width );
					
					if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
						USColor::PremultiplyAlpha ( destRow, this->mColorFormat, width );
					}
				}
				free ( srcBuff );
			}
			else {
				
				void* srcRow = malloc ( srcRowSize );
				
				for ( u32 y = 0; y < height; ++y ) {
					png_read_row ( png, ( png_bytep )srcRow, 0 );
					void* destRow = this->GetRowAddr ( y );
					USColor::Convert ( destRow, this->mColorFormat, srcRow, pngColorFormat, width );
					
					if ( transform & MOAIImageTransform::PREMULTIPLY_ALPHA ) {
						USColor::PremultiplyAlpha ( destRow, this->mColorFormat, width );
					}
				}
				free ( srcRow );
			}
		}
	}
	else {
	
		u32 rowsize = this->GetRowSize ();
		if ( rowsize < png_get_rowbytes ( png, pngInfo )) return;
		
		this->Alloc ();
		if ( isPadded ) {
			this->ClearBitmap ();
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
			
			this->SetPaletteColor ( i, USColor::PackRGBA ( r, g, b, a ));
		}
		
		// copy the rows
		for ( int i = 0; i < passes; ++i ) {
			for ( u32 y = 0; y < height; ++y ) {
				void* row = this->GetRowAddr ( y );
				png_read_row ( png, ( png_bytep )row, 0 );
			}
		}
	}
}

//----------------------------------------------------------------//
bool MOAIImage::IsOK () {

	return ( this->mBitmap != 0 );
}

//----------------------------------------------------------------//
MOAIImage::MOAIImage () :
	mPixelFormat ( USPixel::PXL_FMT_UNKNOWN ),
	mColorFormat ( USColor::CLR_FMT_UNKNOWN ),
	mWidth ( 0 ),
	mHeight ( 0 ),
	mData ( 0 ),
	mPalette ( 0 ),
	mBitmap ( 0 ) {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIImage::~MOAIImage () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIImage::PadToPow2 ( const MOAIImage& image ) {

	USIntRect canvas;
	canvas.mXMin = 0;
	canvas.mYMin = 0;
	canvas.mXMax = this->GetMinPowerOfTwo ( image.GetWidth ());
	canvas.mYMax = this->GetMinPowerOfTwo ( image.GetHeight ());
	
	this->ResizeCanvas ( image, canvas );
}

//----------------------------------------------------------------//
void MOAIImage::RegisterLuaClass ( USLuaState& state ) {
	
	state.SetField ( -1, "POW_TWO", ( u32 )MOAIImageTransform::POW_TWO );
	state.SetField ( -1, "QUANTIZE", ( u32 )MOAIImageTransform::QUANTIZE );
	state.SetField ( -1, "TRUECOLOR", ( u32 )MOAIImageTransform::TRUECOLOR );
	state.SetField ( -1, "PREMULTIPLY_ALPHA", ( u32 )MOAIImageTransform::PREMULTIPLY_ALPHA );
	
	state.SetField ( -1, "PIXEL_FMT_TRUECOLOR", ( u32 )USPixel::TRUECOLOR );
	state.SetField ( -1, "PIXEL_FMT_INDEX_4", ( u32 )USPixel::INDEX_4 );
	state.SetField ( -1, "PIXEL_FMT_INDEX_8", ( u32 )USPixel::INDEX_8 );
	
	state.SetField ( -1, "COLOR_FMT_A_8", ( u32 )USColor::A_8 );
	state.SetField ( -1, "COLOR_FMT_RGB_888", ( u32 )USColor::RGB_888 );
	state.SetField ( -1, "COLOR_FMT_RGB_565", ( u32 )USColor::RGB_565 );
	state.SetField ( -1, "COLOR_FMT_RGBA_5551", ( u32 )USColor::RGBA_5551 );
	state.SetField ( -1, "COLOR_FMT_RGBA_4444", ( u32 )USColor::RGBA_4444 );
	state.SetField ( -1, "COLOR_FMT_RGBA_8888", ( u32 )USColor::RGBA_8888 );
}

//----------------------------------------------------------------//
void MOAIImage::RegisterLuaFuncs ( USLuaState& state ) {
	UNUSED ( state );

	luaL_Reg regTable [] = {
		{ "bleedRect",			_bleedRect },
		{ "convertColors",		_convertColors },
		{ "copy",				_copy },
		{ "copyBits",			_copyBits },
		{ "getColor32",			_getColor32 },
		{ "getFormat",			_getFormat },
		{ "getRGBA",			_getRGBA },
		{ "getSize",			_getSize },
		{ "init",				_init },
		{ "load",				_load },
		{ "padToPow2",			_padToPow2 },
		{ "resizeCanvas",		_resizeCanvas },
		{ "setColor32",			_setColor32 },
		{ "setRGBA",			_setRGBA },
		{ "writePNG",			_writePNG },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIImage::ResizeCanvas ( const MOAIImage& image, USIntRect rect ) {

	assert ( image.mPixelFormat != USPixel::INDEX_4 ); // TODO: handle this edge case

	rect.Bless ();
	
	int width = rect.Width ();
	int height = rect.Height ();
	
	this->Init (( u32 )width, ( u32 )height, image.mColorFormat, image.mPixelFormat );
	
	USIntRect srcRect;
	srcRect.mXMin = -rect.mXMin;
	srcRect.mYMin = -rect.mYMin;
	srcRect.mXMax = ( int )image.GetWidth () + srcRect.mXMin;
	srcRect.mYMax = ( int )image.GetHeight () + srcRect.mYMin;
	
	rect.Offset ( -rect.mXMin, -rect.mYMin );
	
	if ( !srcRect.Overlap ( rect )) {
		this->ClearBitmap ();
		return;
	}
	
	u32 beginSpan = 0;
	u32 leftSize = 0;
	
	if ( srcRect.mXMin > 0 ) {
		beginSpan = srcRect.mXMin;
		leftSize = beginSpan;
	}
	
	u32 endSpan = width;
	u32 rightSize = 0;
	
	if ( srcRect.mXMax < width ) {
		endSpan = srcRect.mXMax;
		rightSize = width - endSpan;
	}
	
	u32 spanSize = endSpan - beginSpan;
	
	u32 pixSize = USPixel::GetDepth ( this->mPixelFormat, this->mColorFormat ) >> 3;
	u32 rowSize = this->GetRowSize ();
	
	leftSize *= pixSize;
	spanSize *= pixSize;
	rightSize *= pixSize;
	
	u32 srcRowSize = image.GetRowSize ();
	u32 srcRowXOff = srcRect.mXMin < 0 ? -srcRect.mXMin * pixSize : 0;
	
	for ( int y = 0; y < height; ++y ) {
	
		void* row = this->GetRowAddr ( y );
	
		if (( y < srcRect.mYMin ) || ( y >= srcRect.mYMax )) {
			memset ( row, 0, rowSize );
		}
		else {
		
			if ( leftSize ) {
				memset ( row, 0, leftSize );
				row = ( void* )(( uintptr )row + leftSize );
			}
			
			if ( spanSize ) {
			
				void* srcRow = ( void* )(( uintptr )image.mBitmap + ( srcRowSize * ( y - srcRect.mYMin )) + srcRowXOff );
				
				memcpy ( row, srcRow, spanSize );
				row = ( void* )(( uintptr )row + spanSize );
			}
			
			if ( rightSize ) {
				memset ( row, 0, rightSize );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIImage::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIImage::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIImage::SetColor ( u32 x, u32 y, u32 color ) {

	if ( this->mPixelFormat != USPixel::TRUECOLOR ) return;
	
	if ( y > this->mHeight ) return;
	if ( x > this->mWidth ) return;

	u32 pixel = USColor::ConvertFromRGBA ( color, this->mColorFormat );
	this->SetPixel ( x, y, pixel );
}

//----------------------------------------------------------------//
void MOAIImage::SetPaletteColor ( u32 idx, u32 rgba ) {

	u32 total = this->GetPaletteCount ();
	if ( idx < total ) {
		
		u32 color = USColor::ConvertFromRGBA ( rgba, this->mColorFormat );
		u32 colorDepth = USColor::GetDepth ( this->mColorFormat );
		
		u32 size = ( colorDepth >> 3 );
		u8* stream = ( u8* )(( uintptr )this->mPalette + ( idx * size ));
		USPixel::WritePixel ( stream, color, size );
	}
}

//----------------------------------------------------------------//
void MOAIImage::SetPixel ( u32 x, u32 y, u32 pixel ) {

	if ( y >= this->mHeight ) return;
	if ( x >= this->mWidth ) return;
	
	void* row = this->GetRowAddr ( y );

	u32 pixelDepth = USPixel::GetDepth ( this->mPixelFormat, this->mColorFormat );
	u32 pixelMask = USPixel::GetMask ( this->mPixelFormat, this->mColorFormat );
	
	if ( pixelDepth == 4 ) {
		u8* address = ( u8* )(( uintptr )row + ( x >> 0x01 ));
		( *address ) = ( *address ) | ( u8 )(( pixel & pixelMask ) << (( x & 0x01 ) ? 0x04 : 0x00 ));
	}
	else {
		u32 size = ( pixelDepth >> 3 );
		u8* stream = ( u8* )(( uintptr )row + ( x * size ));
		USPixel::WritePixel ( stream, pixel, size );
	}
}

//----------------------------------------------------------------//
void MOAIImage::Take ( MOAIImage& image ) {

	this->Clear ();

	this->mColorFormat	= image.mColorFormat;
	this->mPixelFormat	= image.mPixelFormat;

	this->mWidth		= image.mWidth;
	this->mHeight		= image.mHeight;
	
	this->mData			= image.mData;
	this->mBitmap		= image.mBitmap;
	this->mPalette		= image.mPalette;

	// kill the data before clear
	image.mData = 0;
	image.Clear ();
}

//----------------------------------------------------------------//
void MOAIImage::Transform ( u32 transform ) {
	UNUSED ( transform );
}

//----------------------------------------------------------------//
bool MOAIImage::WritePNG ( USStream& stream ) {

	png_structp png_ptr;
	png_infop info_ptr;
	
	png_ptr = png_create_write_struct ( PNG_LIBPNG_VER_STRING, 0, _pngError, 0 );
	assert ( png_ptr ); // TODO

	info_ptr = png_create_info_struct ( png_ptr );
	assert ( png_ptr ); // TODO

	png_set_write_fn ( png_ptr, &stream, _pngWrite, _pngFlush );

	int bitDepth = 0;
	int colorType = 0;
	
	switch ( this->mColorFormat ) {
		
		case USColor::A_8:
			
			bitDepth = 8;
			colorType = PNG_COLOR_TYPE_GRAY;
			break;
		
		case USColor::RGB_888:
		
			bitDepth = 8;
			colorType = PNG_COLOR_TYPE_RGB;
			break;
		
		case USColor::RGBA_4444:
		
			bitDepth = 4;
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
		
		case USColor::RGBA_8888:
		
			bitDepth = 8;
			colorType = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
		
		// TODO: support these formats
		case USColor::RGB_565:
		case USColor::RGBA_5551:
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
		this->mWidth,
		this->mHeight,
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

	for ( u32 y = 0; y < this->mHeight; y++ ) {
		png_bytep row = ( png_bytep )this->GetRowAddr ( y );
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