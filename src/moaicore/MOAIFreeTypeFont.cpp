// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIBitmapFontRipper.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIFreeTypeFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGlyphCache.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITextureBase.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_FREETYPE

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

	int line = render->mPenY - y;
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
// MOAIFreeTypeFont
//================================================================//

//----------------------------------------------------------------//
// iterate through the pending glyphs in each set and attempt to
// update them to match target - i.e. metrics or metrics and bitmap
void MOAIFreeTypeFont::BuildKerning ( FT_Face face, MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs ) {

	MOAIKernVec kernTable [ MAX_KERN_TABLE_SIZE ];

	// iterate over the orignal glyphs and add kerning info for new glyphs
	for ( MOAIGlyph* glyphIt = glyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;
		
		u32 kernTableSize = 0;
		u32 oldTableSize = glyph.mKernTable.Size ();
		
		// iterate over just the new glyphs; check each one against olf glyphs for kerning info
		for ( MOAIGlyph* glyphIt2 = pendingGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			// skip if glyph is already in old glyph's kerning table
			// may happen if glyphs are purged and then re-added
			bool unknown = true;
			for ( u32 i = 0; i < oldTableSize; ++i ) {
				if ( glyph.mKernTable [ i ].mName == glyph2.mCode ) {
					unknown = false;
					break;
				}
			} 
			
			if ( unknown ) {
				FT_Vector  delta;
				FT_Get_Kerning ( face, glyph.mCode, glyph2.mCode, FT_KERNING_DEFAULT, &delta );
				
				if ( delta.x || delta.y ) {
					assert ( kernTableSize < MAX_KERN_TABLE_SIZE );
					MOAIKernVec& kernVec = kernTable [ kernTableSize++ ];
					kernVec.mName = glyph2.mCode;
					kernVec.mX = ( float )( delta.x >> 6 );
					kernVec.mY = ( float )( delta.y >> 6 );
				}
			}
		}
		
		// resize the old kerning table and copy in the new kern vecs (if any)
		if ( kernTableSize ) {
			glyph.mKernTable.Resize ( oldTableSize + kernTableSize );
			memcpy ( &glyph.mKernTable [ oldTableSize ], kernTable, sizeof ( MOAIKernVec ) * kernTableSize );
		}
	}
	
	// iterate over the new glyphs and add kerning info for all glyphs
	for ( MOAIGlyph* glyphIt = pendingGlyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;
		
		u32 kernTableSize = 0;
		
		// iterate over the original glyphs
		for ( MOAIGlyph* glyphIt2 = glyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;

			FT_Vector  delta;
			FT_Get_Kerning ( face, glyph.mCode, glyph2.mCode, FT_KERNING_DEFAULT, &delta );
			
			if ( delta.x || delta.y ) {
				assert ( kernTableSize < MAX_KERN_TABLE_SIZE );
				MOAIKernVec& kernVec = kernTable [ kernTableSize++ ];
				kernVec.mName = glyph2.mCode;
				kernVec.mX = ( float )( delta.x >> 6 );
				kernVec.mY = ( float )( delta.y >> 6 );
			}
		}
		
		// iterate over the new glyphs
		for ( MOAIGlyph* glyphIt2 = pendingGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;
			
			FT_Vector  delta;
			FT_Get_Kerning ( face, glyph.mCode, glyph2.mCode, FT_KERNING_DEFAULT, &delta );
			
			if ( delta.x || delta.y ) {
				assert ( kernTableSize < MAX_KERN_TABLE_SIZE );
				MOAIKernVec& kernVec = kernTable [ kernTableSize++ ];
				kernVec.mName = glyph2.mCode;
				kernVec.mX = ( float )( delta.x >> 6 );
				kernVec.mY = ( float )( delta.y >> 6 );
			}
		}
		
		// init the kern table
		if ( kernTableSize ) {
			glyph.mKernTable.Init ( kernTableSize );
			memcpy ( glyph.mKernTable, kernTable, sizeof ( MOAIKernVec ) * kernTableSize );
		}
	}
}

//----------------------------------------------------------------//
MOAIFreeTypeFont::MOAIFreeTypeFont () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFont )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFreeTypeFont::~MOAIFreeTypeFont () {
}

//----------------------------------------------------------------//
// iterate through the pending glyphs in each set and attempt to
// update them to match target - i.e. metrics or metrics and bitmap
void MOAIFreeTypeFont::ProcessGlyphs () {
	
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
		
		// save pointers to the two glyph lists
		MOAIGlyph* glyphs = glyphDeck.mGlyphs;
		MOAIGlyph* pendingGlyphs = glyphDeck.mPending;
		
		// all pending glyphs will be moved to the processed glyphs list
		// so clear the pending glyphs list
		glyphDeck.mPending = 0;
		
		// if no pending glyphs, move on to the next deck
		if ( !pendingGlyphs ) continue;
		
		// get the face metrics
		FT_Set_Char_Size ( face, 0, ( u32 )( glyphDeck.mPoints * 64.0f ), DPI, DPI );
		
		int yMin = FT_MulFix ( face->bbox.yMin, face->size->metrics.y_scale ) >> 6;
		int yMax = FT_MulFix ( face->bbox.yMax, face->size->metrics.y_scale ) >> 6;
		
		int faceHeight = yMax - yMin;
		
		glyphDeck.mAscent = ( float )( face->size->metrics.ascender >> 6 );
		glyphDeck.mHeight = ( float )( face->size->metrics.height >> 6 );
		
		// build kerning tables (if face has kerning info)
		if ( this->mBuildKerningTables && FT_HAS_KERNING ( face )) {
			this->BuildKerning ( face, glyphs, pendingGlyphs );
		}
		
		//----------------------------------------------------------------//
		// render the new glyphs and move them to the processed list
		for ( MOAIGlyph* glyphIt = pendingGlyphs; glyphIt; ) {
			MOAIGlyph& glyph = *glyphIt;
			glyphIt = glyphIt->mNext;
			
			// move the glyph into the processed glyphs list
			glyph.mNext = glyphDeck.mGlyphs;
			glyphDeck.mGlyphs = &glyph;
			
			u32 index = FT_Get_Char_Index ( face, glyph.mCode );
			FT_Load_Glyph ( face, index, FT_LOAD_NO_BITMAP );
			
			// bail if glyph has no outline we can render
			if ( face->glyph->format!= FT_GLYPH_FORMAT_OUTLINE ) continue;
			
			int glyphWidth = face->glyph->metrics.width >> 6;
			int advanceX = face->glyph->metrics.horiAdvance >> 6;
			int bearingX = face->glyph->metrics.horiBearingX >> 6;
			int bearingY = face->glyph->metrics.horiBearingY >> 6;
			
			glyph.mWidth = ( float )glyphWidth;
			glyph.mHeight = ( float )faceHeight;
			glyph.mAdvanceX = ( float )advanceX;
			glyph.mBearingX = ( float )bearingX;
			glyph.mBearingY = ( float )bearingY;
			
			// place and render the glyph
			if ( this->mGlyphCache ) {
			
				this->mGlyphCache->PlaceGlyph ( glyph );
				
				MOAIImage* image = this->mGlyphCache->GetGlyphImage ( glyph );
				if ( image ) {
					
					render.mImage = image;
					render.mPenX = glyph.mSrcX - bearingX;
					render.mPenY = glyph.mSrcY + bearingY;
					
					FT_Outline_Render ( library, &face->glyph->outline, &params );
					// TODO: need to invalidate glyph rect
				}
			}
		}
	}
	
	FT_Done_Face ( face );
	FT_Done_FreeType ( library );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFont::RebuildKerning () {

	if ( !this->mGlyphDecks.size ()) return;

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
		this->RebuildKerning ( face, glyphDeck );
	}
	
	FT_Done_Face ( face );
	FT_Done_FreeType ( library );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFont::RebuildKerning ( float points ) {

	if ( !this->mGlyphDecks.contains ( points )) return;
	
	FT_Library library;
	FT_Init_FreeType( &library );

	FT_Face face;
	if ( FT_New_Face( library, this->mFilename, 0, &face )) {
		FT_Done_FreeType ( library );
		fprintf ( stderr, "Error loading font: %s\n", this->mFilename );
		return;
	}
	
	MOAIGlyphDeck& glyphDeck = this->mGlyphDecks [ points ];
	this->RebuildKerning ( face, glyphDeck );
	
	FT_Done_Face ( face );
	FT_Done_FreeType ( library );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFont::RebuildKerning ( FT_Face face, MOAIGlyphDeck& glyphDeck ) {
	
	if ( !FT_HAS_KERNING ( face )) return;
	
	// get the face metrics
	FT_Set_Char_Size ( face, 0, ( u32 )( glyphDeck.mPoints * 64.0f ), DPI, DPI );

	MOAIKernVec kernTable [ MAX_KERN_TABLE_SIZE ];

	// iterate over the orignal glyphs and add kerning info for new glyphs
	for ( MOAIGlyph* glyphIt = glyphDeck.mGlyphs; glyphIt; glyphIt = glyphIt->mNext ) {
		MOAIGlyph& glyph = *glyphIt;
		
		u32 kernTableSize = 0;
		
		// iterate over just the new glyphs; check each one against olf glyphs for kerning info
		for ( MOAIGlyph* glyphIt2 = glyphDeck.mGlyphs; glyphIt2; glyphIt2 = glyphIt2->mNext ) {
			MOAIGlyph& glyph2 = *glyphIt2;

			FT_Vector  delta;
			FT_Get_Kerning ( face, glyph.mCode, glyph2.mCode, FT_KERNING_DEFAULT, &delta );
			
			if ( delta.x || delta.y ) {
				assert ( kernTableSize < MAX_KERN_TABLE_SIZE );
				MOAIKernVec& kernVec = kernTable [ kernTableSize++ ];
				kernVec.mName = glyph2.mCode;
				kernVec.mX = ( float )( delta.x >> 6 );
				kernVec.mY = ( float )( delta.y >> 6 );
			}
		}
		
		// init (or clear) the kern table
		glyph.mKernTable.Resize ( kernTableSize );
		
		// copy in the new kern vecs (if any)
		if ( kernTableSize ) {
			memcpy ( glyph.mKernTable, kernTable, sizeof ( MOAIKernVec ) * kernTableSize );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFreeTypeFont::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIFont::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFont::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIFont::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFont::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIFont::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFont::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIFont::SerializeOut ( state, serializer );
}

#endif
