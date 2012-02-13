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
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITexture.h>

#include <contrib/utf8.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#define MAX_TEXTURE_SIZE 1024
#define DPI 300

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
// MOAIFont
//================================================================//
	
//----------------------------------------------------------------//
void MOAIFont::AffirmGlyphs ( float points, cc8* chars, int size ) {

	MOAIGlyphDeck& glyphDeck = this->mGlyphDecks [ points ];
	glyphDeck.mPoints = points;
	
	int i = 0;
	while ( chars [ i ] && ( i < size )) {
		u32 c = u8_nextchar ( chars, &i );
		glyphDeck.AffirmGlyph ( c );
	}
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
void MOAIFont::Init ( cc8* filename ) {

	this->mFilename = filename;
}

//----------------------------------------------------------------//
void MOAIFont::LoadGlyphMetrics () {
}

//----------------------------------------------------------------//
MOAIGlyph* MOAIFont::GetGlyphForChar ( u32 c ) {

	return 0;
}

//----------------------------------------------------------------//
MOAIFont::MOAIFont () :
	mPages ( 0 ) {
}

//----------------------------------------------------------------//
MOAIFont::~MOAIFont () {
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
		float pixelSize = face->size->metrics.y_ppem;
		
		int yMin = FT_MulFix ( face->bbox.yMin, face->size->metrics.y_scale ) >> 6;
		int yMax = FT_MulFix ( face->bbox.yMax, face->size->metrics.y_scale ) >> 6;
		
		int faceHeight = yMax - yMin;
		
		for ( ; glyphIt; glyphIt = glyphIt->mNext ) {
			MOAIGlyph& glyph = *glyphIt;
			
			u32 index = FT_Get_Char_Index ( face, glyph.mCode );
			FT_Load_Glyph ( face, index, FT_LOAD_NO_BITMAP );
			
			if ( face->glyph->format!= FT_GLYPH_FORMAT_OUTLINE ) continue;
			
			int glyphWidth = face->glyph->metrics.width >> 6;
			//int glyphHeight = face->glyph->metrics.height >> 6;
			
			int advanceX = face->glyph->metrics.horiAdvance >> 6;
			int bearingX = face->glyph->metrics.horiBearingX >> 6;
			
			glyph.mWidth = glyphWidth;
			glyph.mHeight = faceHeight;
			glyph.mAdvanceX = ( float )advanceX;
			glyph.mBearingX = ( float )bearingX;
			
			//this->Alloc ( glyph );
			//if ( glyph.mPage ) {
			//	
			//	render.mImage = &glyph.mPage->mImage;
			//	render.mPenX = glyph.mSrcX - bearingX;
			//	render.mPenY = glyph.mSrcY + yMax;

			//	glyph.mPage->AffirmCanvas ();
			//	FT_Outline_Render ( library, &face->glyph->outline, &params );
			//}
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
	
		sprintf ( buffer, "page%d.png", i );
		
		USFileStream stream;
		stream.OpenWrite ( buffer );
		page->mImage.WritePNG ( stream );
	}
}
