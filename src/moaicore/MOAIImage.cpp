// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIImage.h>

//================================================================//
// local
//================================================================//

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

//================================================================//
// MOAIImage
//================================================================//

//----------------------------------------------------------------//
MOAIImage::MOAIImage () {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIImage::~MOAIImage () {
}

//----------------------------------------------------------------//
void MOAIImage::RegisterLuaClass ( USLuaState& state ) {
	
	state.SetField ( -1, "POW_TWO", ( u32 )USImageTransform::POW_TWO );
	state.SetField ( -1, "QUANTIZE", ( u32 )USImageTransform::QUANTIZE );
	state.SetField ( -1, "TRUECOLOR", ( u32 )USImageTransform::TRUECOLOR );
	state.SetField ( -1, "PREMULTIPLY_ALPHA", ( u32 )USImageTransform::PREMULTIPLY_ALPHA );
	
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
		{ "convertColors",		_convertColors },
		{ "copy",				_copy },
		{ "getColor32",			_getColor32 },
		{ "getSize",			_getSize },
		{ "init",				_init },
		{ "load",				_load },
		{ "padToPow2",			_padToPow2 },
		{ "resizeCanvas",		_resizeCanvas },
		{ "setColor32",			_setColor32 },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
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
STLString MOAIImage::ToString () {

	STLString repr;

	return repr;
}
