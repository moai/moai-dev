// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIFreeTypeFontReader.h>
#include <moaicore/MOAIGlyphCacheBase.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAILogMessages.h>

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
// MOAIFreeTypeFontReader
//================================================================//

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::CloseFont () {

	assert ( this->mFace );

	FT_Done_Face ( this->mFace );
	FT_Done_FreeType ( this->mLibrary );
	
	this->mFace = 0;
	this->mLibrary = 0;
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::GetFaceMetrics ( MOAIGlyphSet& glyphSet ) {

	glyphSet.SetAscent (( float )( this->mFace->size->metrics.ascender >> 6 ));
	glyphSet.SetHeight (( float )( this->mFace->size->metrics.height >> 6 ));
}

//----------------------------------------------------------------//
bool MOAIFreeTypeFontReader::GetKernVec ( MOAIGlyph& glyph0, MOAIGlyph& glyph1, MOAIKernVec& kernVec ) {

	kernVec.mX = 0.0f;
	kernVec.mY = 0.0f;

	FT_Vector  delta;
	FT_Get_Kerning ( this->mFace, glyph0.mCode, glyph1.mCode, FT_KERNING_DEFAULT, &delta );
		
	if ( delta.x || delta.y ) {
		kernVec.mName = glyph1.mCode;
		kernVec.mX = ( float )( delta.x >> 6 );
		kernVec.mY = ( float )( delta.y >> 6 );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIFreeTypeFontReader::HasKerning () {

	return ( FT_HAS_KERNING ( this->mFace ) != 0 );
}

//----------------------------------------------------------------//
MOAIFreeTypeFontReader::MOAIFreeTypeFontReader () :
	mLibrary ( 0 ),
	mFace ( 0 ),
	mFaceHeight ( 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFontReader )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFreeTypeFontReader::~MOAIFreeTypeFontReader () {
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::OpenFont ( MOAIFont& font ) {

	assert ( !this->mLibrary );

	cc8* filename = font.GetFilename ();

	FT_Init_FreeType( &this->mLibrary );

	if ( FT_New_Face( this->mLibrary, filename, 0, &this->mFace )) {
		FT_Done_FreeType ( this->mLibrary );
		fprintf ( stderr, "Error loading font: %s\n", filename );
		return;
	}
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIFontReader::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIFontReader::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::RenderGlyph ( MOAIFont& font, MOAIGlyph& glyph ) {

	MOAIGlyphCacheBase* glyphCache = font.GetCache ();
	bool useCache = glyphCache && glyphCache->IsDynamic ();

	FT_Face face = this->mFace;

	u32 index = FT_Get_Char_Index ( face, glyph.mCode );
	FT_Load_Glyph ( face, index, FT_LOAD_NO_BITMAP );
	
	// bail if glyph has no outline we can render
	if ( face->glyph->format!= FT_GLYPH_FORMAT_OUTLINE ) return;

	// set up the render params in case they are needed
	RenderParams render;
	FT_Raster_Params params;
	memset ( &params, 0, sizeof ( params ));
	params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans = _renderSpan;
	params.user = &render;
	
	int glyphWidth = face->glyph->metrics.width >> 6;
	int advanceX = face->glyph->metrics.horiAdvance >> 6;
	int bearingX = face->glyph->metrics.horiBearingX >> 6;
	int bearingY = face->glyph->metrics.horiBearingY >> 6;
	
	glyph.mWidth = ( float )glyphWidth;
	glyph.mHeight = this->mFaceHeight;
	glyph.mAdvanceX = ( float )advanceX;
	glyph.mBearingX = ( float )bearingX;
	glyph.mBearingY = ( float )bearingY;
	
	// place and render the glyph
	if ( useCache ) {
		glyphCache->PlaceGlyph ( font, glyph );
		
		MOAIImage* image = glyphCache->GetGlyphImage ( glyph );
		if ( image ) {
			
			render.mImage = image;
			render.mPenX = glyph.mSrcX - bearingX;
			render.mPenY = glyph.mSrcY + bearingY;
			
			FT_Outline_Render ( this->mLibrary, &face->glyph->outline, &params );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::SetFaceSize ( float size ) {

	FT_Set_Char_Size ( this->mFace, 0, ( u32 )( size * 64.0f ), DPI, DPI );

	int yMin = FT_MulFix ( this->mFace->bbox.yMin, this->mFace->size->metrics.y_scale ) >> 6;
	int yMax = FT_MulFix ( this->mFace->bbox.yMax, this->mFace->size->metrics.y_scale ) >> 6;
		
	this->mFaceHeight = ( float )( yMax - yMin );
}

#endif
