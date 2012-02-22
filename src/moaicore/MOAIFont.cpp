// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIBitmapFontRipper.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGlyphPage.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_getBuildKerningTables ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	state.Push ( self->mBuildKerningTables );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Sets the font file for use when loading glyphs.

	@in		MOAIFont self
	@in		string filename			The path to the TTF file to load.
	@out	nil
*/
int MOAIFont::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "US" )

	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	self->Init ( filename );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	preloadGlyphs
	@text	Loads and caches glyphs for quick access later.

	@in		MOAIFont self
	@in		string charCodes		A string which defines the characters found in the font.
	@in		number points			The point size to be rendered onto the internal texture.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
*/
int MOAIFont::_preloadGlyphs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "USN" )

	cc8* charCodes	= state.GetValue < cc8* >( 2, "" );
	float points	= state.GetValue < float >( 3, 0 );
	float dpi		= state.GetValue < float >( 4, DPI );
	
	float size = POINTS_TO_PIXELS ( points, dpi );
	
	int idx = 0;
	while ( charCodes [ idx ]) {
		u32 c = u8_nextchar ( charCodes, &idx );
		self->AffirmGlyph ( size, c );
	}
	
	self->ProcessGlyphs ();
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_rebuildKerningTables ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		
		float points	= state.GetValue < float >( 2, 0 );
		float dpi		= state.GetValue < float >( 3, DPI );
		
		float size = POINTS_TO_PIXELS ( points, dpi );
		self->RebuildKerning ( size );
	}
	else {
		self->RebuildKerning ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_setBuildKerningTables ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )
	self->mBuildKerningTables = state.GetValue < bool >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFont::_writePages ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFont, "U" )

	self->WritePages ();

	return 0;
}

//================================================================//
// MOAIFont
//================================================================//

//----------------------------------------------------------------//
void MOAIFont::AffirmGlyph ( float points, u32 c ) {

	MOAIGlyphDeck& glyphDeck = this->mGlyphDecks [ points ];
	glyphDeck.mPoints = points;
	glyphDeck.AffirmGlyph ( c );
}

//----------------------------------------------------------------//
MOAISpan < MOAIGlyph* >* MOAIFont::Alloc ( MOAIGlyph& glyph ) {
	
	glyph.mPage = 0;
	
	MOAIGlyphPage* page = this->mPages;
	for ( ; page; page = page->mNext ) {
	
		MOAISpan < MOAIGlyph* >* span = page->Alloc ( glyph );
		if ( span ) {
			return span;
		}
	}
	
	page = new MOAIGlyphPage ();
	page->mNext = this->mPages;
	this->mPages = page;
	
	return page->Alloc ( glyph );
}

//----------------------------------------------------------------//
MOAIGlyphDeck* MOAIFont::GetGlyphDeck ( float points ) {

	return &this->mGlyphDecks [ points ];
}

//----------------------------------------------------------------//
void MOAIFont::Init ( cc8* filename ) {

	this->mFilename = filename;
}

//----------------------------------------------------------------//
bool MOAIFont::IsWhitespace ( u32 c ) {

	if ( !c ) return true;
	if ( c == ' ' ) return true;
	if ( c == '\t' ) return true;
	if ( c == '\n' ) return true;
	
	return false;
}

//----------------------------------------------------------------//
MOAIFont::MOAIFont () :
	mPages ( 0 ),
	mBuildKerningTables ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {
}

//----------------------------------------------------------------//
void MOAIFont::ProcessGlyphs () {
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning () {
}

//----------------------------------------------------------------//
void MOAIFont::RebuildKerning ( float points ) {
	UNUSED ( points );
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getBuildKerningTables",		_getBuildKerningTables },
		{ "load",						_load },
		{ "preloadGlyphs",				_preloadGlyphs },	
		{ "rebuildKerningTables",		_rebuildKerningTables },
		{ "setBuildKerningTables",		_setBuildKerningTables },
		{ "writePages",					_writePages },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFont::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIFont::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIFont::WritePages () {
	
	char buffer [ 256 ];

	MOAIGlyphPage* page = this->mPages;
	for ( int i = 0; page; page = page->mNext, ++i ) {
		if ( page->mImageTexture ) {
			
			sprintf ( buffer, "page%d.png", i );
			
			USFileStream stream;
			stream.OpenWrite ( buffer );
			page->mImageTexture->WritePNG ( stream );
		}
	}
}
