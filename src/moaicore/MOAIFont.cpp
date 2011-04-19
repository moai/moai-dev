// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIFtFontRipper.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getScale
	@text	Returns the default size of this font for use with the MOAITextbox:setTextSize function.

	@in		MOAIFont self
	@out	number size				The default point size of the font.
*/
int MOAIFont::_getScale ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIFont* self = state.GetLuaObject < MOAIFont >( 1 );
	if ( !self ) return 0;
	
	lua_pushnumber ( state, self->GetScale ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Attempts to load glyphs from the specified image file or MOAIDataBuffer containing image data.

	@in		MOAIFont self
	@opt	string filename			A string indicating the path to an image file.
	@opt	MOAIDataBuffer data		A MOAIDataBuffer containing image data.  You must provide either a string or a MOAIDataBuffer, but not both.
	@in		string charCodes		A string which defines the characters found in the font.  For example if A and B are the first letters in the image, the first characters in the string would be "AB" and so forth.
	@out	nil
*/
int MOAIFont::_load ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIFont* self = state.GetLuaObject < MOAIFont >( 1 );
	if ( !self ) return 0;

	STLString charCodes = state.GetValue ( 3, "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.,?!" );

	MOAIDataBuffer *data = state.GetLuaObject < MOAIDataBuffer >( 2 );
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
/**	@name	loadFromTTF
	@text	Attempts to load glyphs contained in a TTF font file into an internal texture for use as a bitmap font.  Texture size is currently limited to 1024x1024.  Unicode characters are not yet supported.

	@in		MOAIFont self
	@in		string filename			The path to the TTF file to load.
	@in		string charCodes		A string which defines the characters found in the font.
	@in		number points			The point size to be rendered onto the internal texture.
	@in		number dpi				The device DPI (dots per inch of device screen).
	@out	nil
*/
int MOAIFont::_loadFromTTF ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "USSN" )

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
	
	if ( drawbuffer.SetTexture ( &this->mTexture )) {
		return this;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIFont::MOAIFont () {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {
}

//----------------------------------------------------------------//
void MOAIFont::LoadFont ( MOAIDataBuffer& fontImageData, cc8* charCodes ) {

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
		this->mTexture.SetFilter ( GL_LINEAR, GL_LINEAR );
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
	
	luaL_Reg regTable [] = {
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
