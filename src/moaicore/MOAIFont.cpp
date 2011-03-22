// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIData.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIFtFontRipper.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( size )getPointSize ( self )</tt>\n
	\n
	Returns the default size of a font for use with the MOAITextbox:setTextSize function.
	@param self (in)
	@param size (out) The default point size of the font.
*/
int MOAIFont::_getScale ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIFont* self = state.GetLuaData < MOAIFont >( 1 );
	if ( !self ) return 0;
	
	lua_pushnumber ( state, self->GetScale ());
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>load ( self, imagefile|data, charCodes )</tt>\n
	\n
	Loads an image to use as a font.
	@param self (in)
	@param imagefile Imagefile to be loaded.  Must be a .bmp, .png, or .jpg.
	@param data A MOAIData object containing image data.  Must be one of the filetypes above.
	@param charCodes A string that sets the default character assignments of the font.  For example, if A and B are the first letters in the font image, the first characters of the string would be "AB" and so forth.
*/
int MOAIFont::_load ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIFont* self = state.GetLuaData < MOAIFont >( 1 );
	if ( !self ) return 0;

	STLString charCodes = state.GetValue ( 3, "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,?!" );

	MOAIData *data = state.GetLuaData < MOAIData >( 2 );
	if ( data ) {

		self->LoadFont ( *data, charCodes );
	}
	else {

		STLString imageFile = state.GetValue ( 2, "" );
		self->LoadFont ( imageFile, charCodes );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() loadFromTTF ( self, filename, charcodes, points, dpi )</tt>\n
\n
	Attempts to load glyphs contained in a TTF into a texture for use as a bitmap font.
	Texture size is presently limited to 1024x1024. Unicode support coming Soon(tm).
	@param self (in)
	@param filename (in) Name of a .ttf font file.
	@param charcodes (in) List of glyphs to be rendered to texture.
	@param points (in) Point size to be rendered.
	@param dpi (in) Device DPI (points per inch of device screen).
*/
int MOAIFont::_loadFromTTF ( lua_State* L ) {
	LUA_SETUP ( MOAIFont, "USSN" )

	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	cc8* charCodes	= state.GetValue < cc8* >( 3, "" );
	float points	= state.GetValue < float >( 4, 0 );
	u32 dpi			= state.GetValue < u32 >( 5, 72 );

	if ( points && dpi ) {
		self->LoadFontFromTTF ( filename, charCodes, points, dpi );
	}
	return 0;
}

//================================================================//
// MOAIFont
//================================================================//

//----------------------------------------------------------------//
USFont* MOAIFont::Bind () {
	
	USDrawBuffer& drawbuffer = USDrawBuffer::Get ();
	
	if ( drawbuffer.BindTexture ( &this->mTexture )) {
		return this;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIFont::MOAIFont () {
	
	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {
}

//----------------------------------------------------------------//
void MOAIFont::LoadFont ( MOAIData& fontImageData, cc8* charCodes ) {

	USImage image;
	USFontRipper ripper;
	ripper.RipAndReturn ( fontImageData, *this, image, charCodes );
	this->mTexture.Init ( image );
	this->mTexture.SetFilter ( GL_LINEAR, GL_LINEAR );
}

//----------------------------------------------------------------//
void MOAIFont::LoadFont ( cc8* fontImageFileName, cc8* charCodes ) {

	USImage image;
	USFontRipper ripper;
	ripper.RipAndReturn ( fontImageFileName, *this, image, charCodes );
	this->mTexture.Init ( image );
	this->mTexture.SetFilter ( GL_LINEAR, GL_LINEAR );
}

//----------------------------------------------------------------//
void MOAIFont::LoadFontFromTTF ( cc8* filename, cc8* charCodes, float points, u32 dpi ) {

	#if USE_FREETYPE
		USImage image;
		MOAIFtFontRipper::RipFromTTF ( filename, *this, image, charCodes, points, dpi );
		this->mTexture.Init ( image );
		this->mTexture.SetFilter ( GL_NEAREST, GL_NEAREST );
	#else
		UNUSED ( filename );
		UNUSED ( charCodes );
		UNUSED ( points );
		UNUSED ( dpi );
	#endif
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaFuncs ( USLuaState& state ) {
	
	LuaReg regTable [] = {
		{ "getScale",			_getScale },
		{ "load",				_load },
		{ "loadFromTTF",		_loadFromTTF },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIFont::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mTexture )

	return repr;
}
