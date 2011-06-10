// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFtFontRipper.h>
#include <moaicore/MOAILogMessages.h>
#include <contrib/utf8.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_FREETYPE

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

//================================================================//
// RenderParams
//================================================================//
class RenderParams {
public:

	USImage*	mImage;
	int			mPenX;
	int			mPenY;
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static void		_initializeImage	( USImage& image, cc8* chars, FT_Face face );
static void		_renderSpan			( const int y, const int count, const FT_Span* const spans, void* const user );

//----------------------------------------------------------------//
static void _initializeImage ( USImage& image, cc8* chars, FT_Face face ) {

	int max = 1024;

	int yMin = FT_MulFix ( face->bbox.yMin, face->size->metrics.y_scale ) >> 6;
	int yMax = FT_MulFix ( face->bbox.yMax, face->size->metrics.y_scale ) >> 6;
	
	int faceHeight = yMax - yMin;
	int yStep = faceHeight + 1;

	int height = 1;
	while ( height < faceHeight ) {
		height = height << 1;
	}
	int width = height;

	int x = 0;
	int y = yStep;

	for ( u32 i = 0; chars [ i ]; ++i ) {
		
		char c = chars [ i ];
		
		u32 index = FT_Get_Char_Index ( face, c );
		FT_Load_Glyph ( face, index, FT_LOAD_NO_BITMAP );
		
		if ( face->glyph->format == FT_GLYPH_FORMAT_OUTLINE ) {
			
			int glyphWidth = face->glyph->metrics.width >> 6;
			if ( !glyphWidth ) continue;
			
			x += glyphWidth + 1;
			if ( x > width ) {
				
				// try to embiggen width
				if ( width < max ) {
					width = width << 1;
				}
				else {
					
					// new row
					x = glyphWidth;
					y += yStep;
					
					if (( y > height ) && ( height < max )) {
						height = height << 1;
					}
				}
			}
		}
	}
	
	image.Init ( width, height, USColor::A_8, USPixel::TRUECOLOR );
	image.ClearBitmap ();
}

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
// MOAIFtFontRipper
//================================================================//

//----------------------------------------------------------------//
void MOAIFtFontRipper::RipFromTTF ( cc8* filename, USFont& font, USImage& image, cc8* chars, float points, u32 dpi ) {

	USLeanArray < char > validChars;
	validChars.Init ( strlen ( chars ) + 1 );
	u32 validLength = 0;

	typedef STLList < USGlyph >::iterator GlyphIt;
	STLList < USGlyph > glyphs;

	FT_Library library;
	FT_Init_FreeType( &library );

	FT_Face face;
	if (FT_New_Face( library, filename, 0, &face )) {
		FT_Done_FreeType ( library );
		fprintf(stderr, "Error loading font: %s\n", filename);
		return;
	}
	
	FT_Set_Char_Size ( face, 0, ( u32 )( points * 64.0f ), dpi, dpi );
	float pixelSize = face->size->metrics.y_ppem;
	
	int yMin = FT_MulFix ( face->bbox.yMin, face->size->metrics.y_scale ) >> 6;
	int yMax = FT_MulFix ( face->bbox.yMax, face->size->metrics.y_scale ) >> 6;
	
	int faceHeight = yMax - yMin;
	
	_initializeImage ( image, chars, face );
	
	u32 imageWidth	= image.GetWidth ();
	//u32 imageHeight	= image.GetHeight ();
	
	float scale = 1.0f / pixelSize;
	float uScale = 1.0f / image.GetWidth ();
	float vScale = 1.0f / image.GetHeight ();
	
	float lineSpacing = ( float )( face->size->metrics.height >> 6 ) * scale;
	
	u32 x = 0;
	u32 y = 0;
	
	int i = 0;
	while ( chars [ i ]) {
		
		u32 charBase = i;
		u32 c = u8_nextchar( chars, &i );
		u32 charSize = i - charBase;
		
		u32 index = FT_Get_Char_Index ( face, c );
		FT_Load_Glyph ( face, index, FT_LOAD_NO_BITMAP );
		
		if ( face->glyph->format == FT_GLYPH_FORMAT_OUTLINE ) {
			
			int glyphWidth = face->glyph->metrics.width >> 6;
			//int glyphHeight = face->glyph->metrics.height >> 6;
			
			int advanceX = face->glyph->metrics.horiAdvance >> 6;
			int bearingX = face->glyph->metrics.horiBearingX >> 6;
			
			if (( x + glyphWidth + 1 ) > imageWidth ) {
				x = 0;
				y += faceHeight + 1;
			}
			
			if ( glyphWidth == 0 ) {
				if ( advanceX ) {
					USGlyph glyph;
					glyph.SetCode ( c );
					glyph.SetAdvanceX (( float )advanceX * scale );
					glyphs.push_back ( glyph );
					
					memcpy ( &validChars [ validLength ], &chars [ charBase ], charSize );
					validLength += charSize;
				}
				continue;
			}
			
			RenderParams render;
			
			render.mImage = &image;
			render.mPenX = x - bearingX;
			render.mPenY = y + yMax;
			
			FT_Raster_Params params;
			memset ( &params, 0, sizeof ( params ));
			params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
			params.gray_spans = _renderSpan;
			params.user = &render;

			FT_Outline_Render ( library, &face->glyph->outline, &params );
			
			USRect uvRect;
			uvRect.mXMin = ( float )x * uScale;
			uvRect.mYMin = ( float )y * vScale;
			uvRect.mXMax = uvRect.mXMin + ( glyphWidth * uScale );
			uvRect.mYMax = uvRect.mYMin + ( faceHeight * vScale );
			
			USGlyph glyph;
			glyph.SetCode ( c );
			glyph.SetUVRect ( uvRect );
			glyph.SetScreenRect (( float )glyphWidth * scale, ( float )faceHeight * scale, 0.0f );
			glyph.SetAdvanceX (( float )advanceX * scale );
			glyph.SetBearingX (( float )bearingX * scale );
			
			glyphs.push_back ( glyph );
			
			x += glyphWidth + 1;
			
			memcpy ( &validChars [ validLength ], &chars [ charBase ], charSize );
			validLength += charSize;
		}
	}
	
	if ( !validLength ) return;
	validChars [ validLength ] = 0;
	
	font.Init ( validChars.Data ());
	font.SetScale ( pixelSize );
	font.SetLineSpacing ( lineSpacing );
	
	USLeanArray < USKernVec > kernTable;
	kernTable.Init ( font.Size ());
	
	bool hasKerning = ( FT_HAS_KERNING ( face ) != 0 );
	
	GlyphIt glyphIt = glyphs.begin ();
	for ( i = 0; glyphIt != glyphs.end (); ++glyphIt, ++i ) {
		USGlyph& glyph = *glyphIt;
		
		if ( hasKerning ) {
			
			u32 kernTableSize = 0;
			
			GlyphIt glyphIt2 = glyphs.begin ();
			for ( ; glyphIt2 != glyphs.end (); ++glyphIt2 ) {
				USGlyph& glyph2 = *glyphIt2;
				
				u32 idx0 = glyph.GetCode ();
				u32 idx1 = glyph2.GetCode ();
				
				if ( idx0 == idx1 ) continue;
				
				FT_Vector  delta;
				FT_Get_Kerning ( face, idx0, idx1, FT_KERNING_DEFAULT, &delta );
				int kernX = delta.x >> 6;
				int kernY = delta.y >> 6;
				
				if ( kernX || kernY ) {
					
					//printf ( "X Kerning %d->%d: ( %d, %d )\n", ( int )idx0, ( int )idx1, kernX, kernY );
					
					USKernVec& kernVec = kernTable [ kernTableSize++ ];
					
					kernVec.mName = idx1;
					kernVec.mX = ( float )( kernX * scale );
					kernVec.mY = ( float )( kernY * scale );
				}
			}
			
			if ( kernTableSize ) {
				
				glyph.ReserveKernTable ( kernTableSize );
				for ( u32 j = 0; j < kernTableSize; ++j ) {
					glyph.SetKernVec ( j, kernTable [ j ]);
				}
			}
		}
		font.SetGlyph ( glyph );
	}
	
	FT_Done_Face ( face );
	FT_Done_FreeType ( library );
}

#endif