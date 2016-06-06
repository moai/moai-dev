// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#if MOAI_WITH_FREETYPE

#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIFreeTypeFontReader.h>
#include <moai-sim/MOAIGlyphCache.h>
#include <moai-sim/MOAIImageTexture.h>

#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_STROKER_H

//================================================================//
// RenderParams
//================================================================//
class RenderParams {
public:

	MOAIImage*	mImage;
	
	ZLColorBlendFunc	mBlendFunc;
	ZLColorVec			mPenColor;
	int					mPenX;
	int					mPenY;
};

//----------------------------------------------------------------//
static void _renderSpan ( const int y, const int count, const FT_Span* const spans, void* const user ) {

	if ( !user ) return;
	
	RenderParams* render = ( RenderParams* )user;

	int line = render->mPenY - y - 1;
	int offset = render->mPenX;

	MOAIImage* image = render->mImage;
	ZLColor::ColorFormat colorFormat = image->GetColorFormat ();
	ZLColorBlendFunc blendFunc = render->mBlendFunc;

	u32 penColor = render->mPenColor.PackRGBA ();

	for ( int i = 0; i < count; ++i ) {
		
		const FT_Span& span = spans [ i ];
		
		int x = offset + span.x;
		int len = span.len;
		
		u32 srcColor = ZLColor::Scale ( penColor, span.coverage );
		
		if ( blendFunc.mEquation == ZLColor::BLEND_EQ_NONE ) {
			
			u32 pixel = ZLColor::ConvertFromRGBA ( srcColor, colorFormat );
			for ( int j = 0; j < len; ++j ) {
				render->mImage->SetPixel ( x + j, line, pixel );
			}
		}
		else {
			for ( int j = 0; j < len; ++j ) {
				u32 dstColor = render->mImage->GetColor ( x + j, line );
				u32 blendColor = ZLColor::Blend ( srcColor, dstColor, blendFunc );
				u32 pixel = ZLColor::ConvertFromRGBA ( blendColor, colorFormat );
				render->mImage->SetPixel ( x + j, line, pixel );
			}
		}
	}
}

//================================================================//
// MOAIFreeTypeFontReader lua
//================================================================//

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::_enableAntiAliasing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFreeTypeFontReader, "U" )
	self->mAntiAlias = state.GetValue < bool >( 2, false );
	return 0;
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::_setPenColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFreeTypeFontReader, "U" )
	self->mPenColor = state.GetColor ( 2, 1.0f, 1.0f, 1.0f, 1.0f );
	return 0;
}

//================================================================//
// MOAIFreeTypeFontReader
//================================================================//

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::CloseFontFile () {

	if ( this->mFace ) {

		FT_Done_Face (( FT_Face )this->mFace );
		FT_Done_FreeType (( FT_Library )this->mLibrary );
	}
	
	this->mFace = 0;
	this->mLibrary = 0;
	
	this->mFaceSize = 0.0f;
	this->mGlyphCode = GLYPH_CODE_NULL;

	return ( int )OK;
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::GetFaceMetrics ( MOAIFontFaceMetrics& faceMetrics ) {

	FT_Face face = ( FT_Face )this->mFace;

	if ( !face ) return FONT_ERROR;

	faceMetrics.mAscent = ( float )( face->size->metrics.ascender >> 6 ); // div 64
	faceMetrics.mHeight = ( float )( face->size->metrics.height >> 6 ); // div 64
	
	return OK;
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::GetGlyphMetrics ( MOAIGlyphMetrics& glyphMetrics ) {

	FT_Face face = ( FT_Face )this->mFace;

	if ( !face ) return FONT_ERROR;
	if ( this->mGlyphCode == GLYPH_CODE_NULL ) return FONT_ERROR;

	glyphMetrics.mWidth			= ( float )( face->glyph->metrics.width >> 6 ); // div 64
	glyphMetrics.mHeight		= ( float )( face->glyph->metrics.height >> 6 ); // div 64
	glyphMetrics.mAdvanceX		= ( float )( face->glyph->metrics.horiAdvance >> 6 ); // div 64
	glyphMetrics.mBearingX		= ( float )( face->glyph->metrics.horiBearingX >> 6 ); // div 64
	glyphMetrics.mBearingY		= ( float )( face->glyph->metrics.horiBearingY >> 6 ); // div 64
	
	return OK;
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::GetKernVec ( u32 c, MOAIKernVec& kernVec ) {

	kernVec.mX = 0.0f;
	kernVec.mY = 0.0f;

	FT_Vector  delta;
	FT_Get_Kerning (( FT_Face )this->mFace, this->mGlyphCode, c, FT_KERNING_DEFAULT, &delta );
		
	if ( delta.x || delta.y ) {
		kernVec.mName = c;
		kernVec.mX = ( float )( delta.x >> 6 );
		kernVec.mY = ( float )( delta.y >> 6 );
		return OK;
	}
	return NO_KERNING;
}

//----------------------------------------------------------------//
bool MOAIFreeTypeFontReader::HasKerning () {

	FT_Face face = ( FT_Face )this->mFace;
	return ( FT_HAS_KERNING ( face ) != 0 );
}

//----------------------------------------------------------------//
MOAIFreeTypeFontReader::MOAIFreeTypeFontReader () :
	mGlyphCode ( GLYPH_CODE_NULL ),
	mFaceSize ( 0.0f ),
	mLibrary ( 0 ),
	mFace ( 0 ),
	mFaceHeight ( 0.0f ),
	mAntiAlias ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFontReader )
	RTTI_END
	
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIFreeTypeFontReader::~MOAIFreeTypeFontReader () {
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::OpenFontFile ( cc8* filename ) {

	this->CloseFontFile ();

	FT_Init_FreeType ( ( FT_Library* )&this->mLibrary );

	if ( FT_New_Face (( FT_Library )this->mLibrary, filename, 0, ( FT_Face* )&this->mFace )) {
		FT_Done_FreeType (( FT_Library )this->mLibrary );

		this->mFace = 0;
		this->mLibrary = 0;

		ZLLog_ErrorF ( ZLLog::CONSOLE, "FONT_ERROR loading font: %s\n", filename );
		return FONT_ERROR;
	}
	return OK;
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIFontReader::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIFontReader::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "enableAntiAliasing",		_enableAntiAliasing },
		{ "setPenColor",			_setPenColor },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::RenderGlyph ( MOAIImage& image, float x, float y, const ZLColorBlendFunc& blendFunc ) {

	if ( image.GetPixelFormat () != MOAIImage::TRUECOLOR ) return FONT_ERROR;

	if ( !this->mFace ) return FONT_ERROR;
	if ( this->mGlyphCode == GLYPH_CODE_NULL ) return FONT_ERROR;

	FT_Face face = ( FT_Face )this->mFace;
	
	// bail if glyph has no outline we can render
	if ( face->glyph->format!= FT_GLYPH_FORMAT_OUTLINE ) return FONT_ERROR;
	
	// TODO: not clear what is going on here - why are we checking mAntiAlias?
	if ( this->mAntiAlias ) {
	
		// set up the render params in case they are needed
		RenderParams render;
		FT_Raster_Params params;
		memset ( &params, 0, sizeof ( params ));
		params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
		params.gray_spans = _renderSpan;
		params.user = &render;
		
		render.mImage = &image;
		render.mBlendFunc = blendFunc;
		render.mPenColor = this->mPenColor;
		render.mPenX =  ( int )x;
		render.mPenY = ( int )y;
	
		FT_Outline_Render (( FT_Library )this->mLibrary, &face->glyph->outline, &params );
	}
	else {
		// completely different path to render monochrome
		// TODO: render monochrome glyph
		FT_Error error = FT_Render_Glyph ( face->glyph, FT_RENDER_MODE_MONO );
		if ( error == 0 ) {
			printf ( "OK!\n" );
		}
	}

	return OK;
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::SelectFace ( float size ) {

	if ( this->mFaceSize != size ) {

		FT_Face face = ( FT_Face )this->mFace;

		if ( !face ) return FONT_ERROR;

		FT_Set_Char_Size ( face, 0, ( u32 )( size * 64.0f ), DPI, DPI );

		int yMin = FT_MulFix ( face->bbox.yMin, face->size->metrics.y_scale ) >> 6; // div 64
		int yMax = FT_MulFix ( face->bbox.yMax, face->size->metrics.y_scale ) >> 6; // div 64
			
		this->mFaceHeight = ( float )( yMax - yMin );
		this->mFaceSize = size;
	}
	return OK;
}

//----------------------------------------------------------------//
int MOAIFreeTypeFontReader::SelectGlyph ( u32 c ) {

	if ( this->mGlyphCode != c ) {
	
		this->mGlyphCode = GLYPH_CODE_NULL;
	
		u32 index = FT_Get_Char_Index (( FT_Face )this->mFace, c );
		int result = FT_Load_Glyph (( FT_Face )this->mFace, index, FT_LOAD_NO_BITMAP );
		if ( result ) return FONT_ERROR;
		
		this->mGlyphCode = c;
	}
	return OK;
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIFontReader::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIFreeTypeFontReader::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIFontReader::SerializeOut ( state, serializer );
}

#endif
