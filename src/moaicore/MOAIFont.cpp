// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIBitmapFontRipper.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIFreetypeFontRipper.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITexture.h>

#define WIDE_ID_BIT			0x80000000
#define WIDE_ID_MASK		0x7fffffff
#define INVALID_ID			0xffffffff
#define INVALID_BYTE_ID		0xff

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getImage
	@text	Returns the image containing the font's glyphs. Only valid
			before font is used (i.e. immediately after ripping).

	@in		MOAIFont self
	@out	MOAIImage image
*/
int MOAIFont::_getImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	if ( self->mImage ) {
		self->mImage->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getLineScale
	@text	Returns the default size of a line (in pixels).

	@in		MOAIFont self
	@out	number lineScale		The default size of the line in pixels.
*/
int MOAIFont::_getLineScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	
	lua_pushnumber ( state, self->GetScale () * self->GetLineSpacing ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getScale
	@text	Returns the default size of this font for use with the MOAITextbox:setTextSize function.

	@in		MOAIFont self
	@out	number size				The default point size of the font.
*/
int MOAIFont::_getScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	
	lua_pushnumber ( state, self->GetScale ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTexture
	@text	Returns the font's texture.

	@in		MOAIFont self
	@out	MOAITexture texture
*/
int MOAIFont::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	if ( self->mTexture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
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
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
*/
int MOAIFont::_loadFromTTF ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "USSN" )

	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	cc8* charCodes	= state.GetValue < cc8* >( 3, "" );
	float points	= state.GetValue < float >( 4, 0 );
	u32 dpi			= state.GetValue < u32 >( 5, 72 );
	
	if (( points > 0.0f ) && dpi ) {
		self->LoadFontFromTTF ( filename, charCodes, points, dpi );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setImage
	@text	Set an image for the font to use.

	@in		MOAIFont self
	@opt	MOAIImage image		Default value is nil.
	@out	nil
*/
int MOAIFont::_setImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2 );
	self->SetImage ( image );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set a texture for the font to use.

	@in		MOAIFont self
	@opt	MOAITexture texture		Default value is nil.
	@out	nil
*/
int MOAIFont::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	MOAITexture* texture = state.GetLuaObject < MOAITexture >( 2 );
	self->SetTexture ( texture );
	
	return 0;
}

//================================================================//
// MOAIFont
//================================================================//

//----------------------------------------------------------------//
MOAIFont* MOAIFont::Bind () {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	if ( gfxDevice.SetTexture ( this->mTexture )) {
		this->mImage.Set ( *this, 0 );
		return this;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIFont::DrawGlyph ( u32 c, float points, float x, float y ) {

	const MOAIGlyph& glyph = this->GetGlyphForChar ( c );
	glyph.Draw ( points, x, y );
}

//----------------------------------------------------------------//
MOAIGlyph& MOAIFont::GetGlyphForChar ( u32 c ) {

	u32 id = this->GetIDForChar ( c );
	return this->GetGlyphForID ( id );
}

//----------------------------------------------------------------//
MOAIGlyph& MOAIFont::GetGlyphForID ( u32 id ) {

	if ( id == INVALID_ID ) {
		return this->mDummy;
	}

	if ( id & WIDE_ID_BIT ) {
		id = id & WIDE_ID_MASK;
		assert ( id < this->mWideGlyphs.Size ());
		return this->mWideGlyphs [ id ];
	}
	
	assert ( id < this->mByteGlyphs.Size ());
	return this->mByteGlyphs [ id ];
}

//----------------------------------------------------------------//
u32 MOAIFont::GetIDForChar ( u32 c ) {

	u32 id = INVALID_ID;

	if ( this->IsWideChar ( c )) {
		
		// TODO: replace sorted lookup w/ AVL tree
		u32 size = this->mWideGlyphMap.Size ();
		id = USBinarySearch < u32 >( this->mWideGlyphMap, c, size );
		if ( id < size ) {
			return id | WIDE_ID_BIT;
		}
	}
	else {
		if ( this->mByteGlyphMapBase <= c ) {
			c -= this->mByteGlyphMapBase;
			if ( c < this->mByteGlyphMap.Size ()) {
				id = this->mByteGlyphMap [ c ];
				if ( id == INVALID_BYTE_ID ) {
					id = INVALID_ID;
				}
			}
		}
	}
	return id;
}

//----------------------------------------------------------------//
void MOAIFont::Init ( cc8* charCodes ) {

	u32 byteCharTop = 0;
	u32 byteCharBase = 0x000000ff;
	u32 totalWideChars = 0;
	
	for ( int i = 0; charCodes [ i ]; ) {
		u32 c = u8_nextchar( charCodes, &i );
		
		if ( this->IsWideChar ( c )) {
			totalWideChars++;
		}
		else {
			
			if ( c < byteCharBase ) {
				byteCharBase = c;
			}
			if ( c > byteCharTop ) {
				byteCharTop = c;
			}
		}
	}
	
	byteCharTop += 1;
	u32 totalByteChars = ( byteCharBase < byteCharTop ) ? byteCharTop - byteCharBase : 0;
	
	this->mByteGlyphs.Init ( totalByteChars );
	this->mByteGlyphMap.Init ( totalByteChars );
	this->mByteGlyphMapBase = ( u8 )byteCharBase;
	
	this->mWideGlyphs.Init ( totalWideChars );
	this->mWideGlyphMap.Init ( totalWideChars );
	
	this->mByteGlyphMap.Fill ( INVALID_BYTE_ID );
	this->mWideGlyphMap.Fill ( INVALID_ID );
	
	u32 b = 0;
	u32 w = 0;
	for ( int i = 0; charCodes [ i ]; ) {
		
		u32 c = u8_nextchar( charCodes, &i );
		
		if ( this->IsWideChar ( c )) {
			this->mWideGlyphMap [ w++ ] = c;
		}
		else {
			this->mByteGlyphMap [ c - this->mByteGlyphMapBase ] = ( u8 )b++;
		}
	}
	RadixSort32 < u32 >( this->mWideGlyphMap, totalWideChars );
}

//----------------------------------------------------------------//
bool MOAIFont::IsWideChar ( u32 c ) {

	return ( c & 0xffffff00 ) != 0;
}

//----------------------------------------------------------------//
MOAIFont::MOAIFont () :
	mByteGlyphMapBase ( 0 ),
	mScale ( 1.0f ),
	mLineSpacing ( 1.0f ) {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {

	this->mTexture.Set ( *this, 0 );
	this->mImage.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIFont::LoadFont ( MOAIDataBuffer& fontImageData, cc8* charCodes ) {

	MOAIImage* image = new MOAIImage ();
	
	MOAIBitmapFontRipper ripper;
	ripper.RipAndReturn ( fontImageData, *this, *image, charCodes );
	this->SetImage ( image );
}

//----------------------------------------------------------------//
void MOAIFont::LoadFont ( cc8* fontImageFileName, cc8* charCodes ) {

	MOAIImage* image = new MOAIImage ();
	
	MOAIBitmapFontRipper ripper;
	ripper.RipAndReturn ( fontImageFileName, *this, *image, charCodes );
	this->SetImage ( image );
}

//----------------------------------------------------------------//
void MOAIFont::LoadFontFromTTF ( cc8* filename, cc8* charCodes, float points, u32 dpi ) {

	#if USE_FREETYPE
	
		MOAIImage* image = new MOAIImage ();
		MOAIFreetypeFontRipper::RipFromTTF ( filename, *this, *image, charCodes, points, dpi );
		this->SetImage ( image );
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
		{ "getImage",			_getImage },
		{ "getLineScale",		_getLineScale },
		{ "getScale",			_getScale },
		{ "getTexture",			_getTexture },
		{ "load",				_load },
		{ "loadFromTTF",		_loadFromTTF },
		{ "setImage",			_setImage },
		{ "setTexture",			_setTexture },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFont::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );
	
	if ( state.GetFieldWithType ( -1, "mByteGlyphs", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mByteGlyphs.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mByteGlyphs [ i ].SerializeIn ( state );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	if ( state.GetFieldWithType ( -1, "mByteGlyphMap", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mByteGlyphMap.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mByteGlyphMap [ i ] = state.GetValue < u8 >( -1, 0 );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	this->mByteGlyphMapBase		= state.GetField ( -1, "mByteGlyphMapBase", this->mByteGlyphMapBase );
	
	if ( state.GetFieldWithType ( -1, "mWideGlyphs", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mWideGlyphs.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mWideGlyphs [ i ].SerializeIn ( state );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	if ( state.GetFieldWithType ( -1, "mWideGlyphMap", LUA_TTABLE )) {
		
		u32 size = lua_objlen ( state, -1 );
		this->mWideGlyphMap.Init ( size );
		
		for ( u32 i = 0; i < size; ++i ) {
			state.GetField ( -1, i + 1 );
			this->mWideGlyphMap [ i ] = state.GetValue < u32 >( -1, 0 );
			state.Pop ( 1 );
		}
		state.Pop ( 1 );
	}
	
	this->mScale				= state.GetField ( -1, "mScale", this->mScale );
	this->mLineSpacing			= state.GetField ( -1, "mLineSpacing", this->mLineSpacing );
}

//----------------------------------------------------------------//
void MOAIFont::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( serializer );
	
	if ( this->mByteGlyphs.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mByteGlyphs.Size (); ++i ) {
			lua_pushnumber ( state, i + 1 );
			lua_newtable ( state );
			this->mByteGlyphs [ i ].SerializeOut ( state );
			lua_settable ( state, -3 );
		}
		lua_setfield ( state, -2, "mByteGlyphs" );
	}
	
	if ( this->mByteGlyphMap.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mByteGlyphMap.Size (); ++i ) {
			state.SetFieldByIndex ( -1, i + 1, this->mByteGlyphMap [ i ]);
		}
		lua_setfield ( state, -2, "mByteGlyphMap" );
	}

	state.SetField ( -1, "mByteGlyphMapBase", this->mByteGlyphMapBase );
	
	if ( this->mWideGlyphs.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mWideGlyphs.Size (); ++i ) {
			lua_pushnumber ( state, i + 1 );
			lua_newtable ( state );
			this->mWideGlyphs [ i ].SerializeOut ( state );
			lua_settable ( state, -3 );
		}
		lua_setfield ( state, -2, "mWideGlyphs" );
	}
	
	if ( this->mWideGlyphMap.Size ()) {
		lua_newtable ( state );
		for ( u32 i = 0; i < this->mWideGlyphMap.Size (); ++i ) {
			state.SetFieldByIndex ( -1, i + 1, this->mWideGlyphMap [ i ]);
		}
		lua_setfield ( state, -2, "mWideGlyphMap" );
	}
	
	state.SetField ( -1, "mScale", this->mScale );
	state.SetField ( -1, "mLineSpacing", this->mLineSpacing );
}

//----------------------------------------------------------------//
void MOAIFont::SetGlyph ( const MOAIGlyph& glyph ) {

	u32 id = this->GetIDForChar ( glyph.mCode );
	if ( id != INVALID_ID ) {
	
		if ( glyph.mAdvanceX > this->mDummy.mAdvanceX ) {
			this->mDummy.mAdvanceX = glyph.mAdvanceX;
		}
		this->GetGlyphForID ( id ) = glyph;
	}
}

//----------------------------------------------------------------//
u32 MOAIFont::Size () {

	return this->mByteGlyphs.Size () + this->mWideGlyphs.Size ();
}

//----------------------------------------------------------------//
void MOAIFont::SetImage ( MOAIImage* image ) {

	this->mImage.Set ( *this, image );
	this->mTexture.Set ( *this, 0 );
	
	if ( image && MOAIGfxDevice::Get ().GetHasContext ()) {
		this->mTexture.Set ( *this, new MOAITexture ());
		this->mTexture->Init ( *image, "'texture from font'" );
		this->mTexture->SetFilter ( GL_LINEAR, GL_LINEAR );
	}
}

//----------------------------------------------------------------//
void MOAIFont::SetTexture ( MOAITexture* texture ) {

	this->mImage.Set ( *this, 0 );
	this->mTexture.Set ( *this, texture );
}

