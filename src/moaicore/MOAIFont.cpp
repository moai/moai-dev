// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIBitmapFontRipper.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIFreetypeFontRipper.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGlyphPage.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITextureBase.h>

#include <contrib/utf8.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#define MAX_TEXTURE_SIZE 1024
#define DPI 72

//================================================================//
// RenderParams
//================================================================//
class RenderParams {
public:

	MOAIImage*	mImage;
	int			mPenX;
	int			mPenY;
};

//----------------------------------------------------------------//
static void _renderSpan ( const int y, const int count, const FT_Span* const spans, void* const user ) {

	if ( !user ) return;
	
	RenderParams* render = ( RenderParams* )user;

	int line = render->mPenY - y - 1;
	int offset = render->mPenX;

	for ( int i = 0; i < count; ++i ) {
		
		const FT_Span& span = spans [ i ];
		
		int x = offset + span.x;
		int len = span.len;
		
		u32 alpha = ( u32 )span.coverage;
		
		for ( int j = 0; j < len; ++j ) {
			render->mImage->SetPixel ( x + j, line, alpha );
		}
	}
}

//================================================================//
// local
//================================================================//

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
	float dpi		= state.GetValue < float >( 4, 72.0f );
	
	float size = ( points * dpi ) / 72.0f;
	
	int idx = 0;
	while ( charCodes [ idx ]) {
		u32 c = u8_nextchar ( charCodes, &idx );
		self->AffirmGlyph ( points, c );
	}
	
	self->UpdateGlyphs ( MOAIGlyph::METRICS_AND_BITMAP );
	
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
	mPages ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFont::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "load",				_load },
		{ "preloadGlyphs",		_preloadGlyphs },
		{ "writePages",			_writePages },
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
// iterate through the pending glyphs in each set and attempt to
// update them to match target - i.e. metrics or metrics and bitmap
void MOAIFont::UpdateGlyphs ( u32 target ) {
	
	// set up the render params in case they are needed
	RenderParams render;
	FT_Raster_Params params;
	memset ( &params, 0, sizeof ( params ));
	params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans = _renderSpan;
	params.user = &render;
	
	FT_Library library;
	FT_Init_FreeType( &library );

	FT_Face face;
	if ( FT_New_Face( library, this->mFilename, 0, &face )) {
		FT_Done_FreeType ( library );
		fprintf ( stderr, "Error loading font: %s\n", this->mFilename );
		return;
	}
	
	GlyphDecksIt glyphDecksIt = this->mGlyphDecks.begin ();
	for ( ; glyphDecksIt != this->mGlyphDecks.end (); ++glyphDecksIt ) {
		MOAIGlyphDeck& glyphDeck = glyphDecksIt->second;
		
		MOAIGlyph* glyphIt = glyphDeck.mPending;
		glyphDeck.mPending = 0;
		
		if ( !glyphIt ) continue;
		
		FT_Set_Char_Size ( face, 0, ( u32 )( glyphDeck.mPoints * 64.0f ), DPI, DPI );
		
		int yMin = FT_MulFix ( face->bbox.yMin, face->size->metrics.y_scale ) >> 6;
		int yMax = FT_MulFix ( face->bbox.yMax, face->size->metrics.y_scale ) >> 6;
		
		int faceHeight = yMax - yMin;
		
		glyphDeck.mAscent = ( float )( face->size->metrics.ascender >> 6 );
		glyphDeck.mHeight = ( float )( face->size->metrics.height >> 6 );
		
		for ( ; glyphIt; glyphIt = glyphIt->mNext ) {
			MOAIGlyph& glyph = *glyphIt;
			
			glyph.mStatus = MOAIGlyph::NONE;
			
			u32 index = FT_Get_Char_Index ( face, glyph.mCode );
			FT_Load_Glyph ( face, index, FT_LOAD_NO_BITMAP );
			
			if ( face->glyph->format!= FT_GLYPH_FORMAT_OUTLINE ) continue;
			
			int glyphWidth = face->glyph->metrics.width >> 6;
			//int glyphHeight = face->glyph->metrics.height >> 6;
			
			int advanceX = face->glyph->metrics.horiAdvance >> 6;
			int bearingX = face->glyph->metrics.horiBearingX >> 6;
			int bearingY = face->glyph->metrics.horiBearingY >> 6;
			
			glyph.mWidth = ( float )glyphWidth;
			glyph.mHeight = ( float )faceHeight;
			glyph.mAdvanceX = ( float )advanceX;
			glyph.mBearingX = ( float )bearingX;
			glyph.mBearingY = ( float )bearingY;
			
			glyph.mStatus = MOAIGlyph::METRICS_ONLY;
			
			if ( target == MOAIGlyph::METRICS_AND_BITMAP ) {
				this->Alloc ( glyph );
				if ( glyph.mPage ) {
					
					glyph.mPage->AffirmCanvas ();
					
					render.mImage = glyph.mPage->mImageTexture;
					render.mPenX = glyph.mSrcX - bearingX;
					render.mPenY = glyph.mSrcY + yMax;

					FT_Outline_Render ( library, &face->glyph->outline, &params );
					
					glyph.mStatus = MOAIGlyph::METRICS_AND_BITMAP;
				}
			}
		}
	}
	
	FT_Done_Face ( face );
	FT_Done_FreeType ( library );
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
