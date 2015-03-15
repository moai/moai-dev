// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIBitmapFontReader.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGlyphCache.h>
#include <moai-sim/MOAIImageTexture.h>

//================================================================//
// MOAIBitmapFontPage
//================================================================//

//----------------------------------------------------------------//
void MOAIBitmapFontPage::GetGlyphBounds ( MOAIBitmapGlyph& bitmapGlyph, ZLIntRect& frame ) {

	bitmapGlyph.mIsWhitespace = true;
	bitmapGlyph.mSrcRect = frame;

	ZLIntRect bounds;

	bounds.mXMin = frame.mXMax;
	bounds.mXMax = frame.mXMin - 1;

	bounds.mYMin = frame.mYMax;
	bounds.mYMax = frame.mYMin - 1;

	for ( int y = frame.mYMin; y < frame.mYMax; ++y ) {
		for ( int x = frame.mXMin; x < frame.mXMax; ++x ) {
			
			u32 color = this->GetRGBA ( x, y );
			if ( !( color & 0xff000000 )) continue;
			
			if ( x < bounds.mXMin ) bounds.mXMin = x;
			if ( x >= bounds.mXMax ) bounds.mXMax = x + 1;
			
			if ( y < bounds.mYMin ) bounds.mYMin = y;
			if ( y >= bounds.mYMax ) bounds.mYMax = y + 1;
		}
	}
	
	if (( bounds.mXMin < bounds.mXMax ) && ( bounds.mYMin < bounds.mYMax )) {
		bitmapGlyph.mIsWhitespace = false;
		bitmapGlyph.mSrcRect = bounds;
	}
}

//----------------------------------------------------------------//
ZLIntRect MOAIBitmapFontPage::GetGlyphFrame ( u32 x, u32 y, u32 maskColor ) {

	ZLIntRect rect;
	rect.Init ( 0, 0, 0, 0 );

	int width = this->mImage.GetWidth ();
	int height = this->mImage.GetHeight ();

	int left = x + 1;
	
	int right = left;
	for ( ; right < width; ++right ) {
		if ( maskColor == this->GetRGB ( right, y )) break;
	}
	if ( right == width ) return rect;

	int top = y;
	for ( ; top > 0; --top ) {
		if ( maskColor == this->GetRGB ( left, top )) break;
	}
	if ( top == 0 ) return rect;
	
	int bottom = y;
	for ( ; bottom < height; ++bottom ) {
		if ( maskColor == this->GetRGB ( left, bottom )) break;
	}
	if ( bottom == height ) return rect;
	
	rect.mXMin = left;
	rect.mXMax = right;
	
	rect.mYMin = top + 1;
	rect.mYMax = bottom;
	
	return rect;
}

//----------------------------------------------------------------//
u32 MOAIBitmapFontPage::GetRGB ( u32 x, u32 y ) {

	return this->mImage.GetColor ( x, y ) & 0x00ffffff;
}

//----------------------------------------------------------------//
u32 MOAIBitmapFontPage::GetRGBA ( u32 x, u32 y ) {

	return this->mImage.GetColor ( x, y );
}

//----------------------------------------------------------------//
MOAIBitmapFontPage::MOAIBitmapFontPage () :
	mBase ( 0 ),
	mHeight ( 0 ) {
}

//----------------------------------------------------------------//
MOAIBitmapFontPage::~MOAIBitmapFontPage () {
}

//----------------------------------------------------------------//
void MOAIBitmapFontPage::RipBitmap ( cc8* filename, cc8* charCodes ) {

	this->mBitmapGlyphs.clear ();

	this->mImage.Load ( filename );
	if ( !this->mImage.IsOK ()) return;

	this->mHeight = 0;
	this->mBase = 0;

	u32 width = this->mImage.GetWidth ();
	u32 height = this->mImage.GetHeight ();
	u32 bgColor = ( u32 )this->GetRGB ( 0, 0 );

	int idx = 0;

	for ( u32 y = 0; y < height; ++y ) {
		
		u32 color = this->GetRGB ( 0, y );
		if ( color != bgColor ) {
		
			u32 maskColor = color;
			
			for ( u32 x = 1; x < width; ++x ) {
	
				u32 colorRgb = this->GetRGB ( x, y );
				if ( colorRgb == maskColor ) {
					
					ZLIntRect frame = this->GetGlyphFrame ( x, y, maskColor );

					if ( !frame.Width ()) continue;
					if ( !frame.Height ()) continue;
					
					u32 c = moai_u8_nextchar ( charCodes, &idx );
					if ( c == 0 ) {
						y = height;
						break;
					}
					
					MOAIBitmapGlyph& bitmapGlyph = this->mBitmapGlyphs [ c ];
					
					this->GetGlyphBounds ( bitmapGlyph, frame );
					bitmapGlyph.mBase = y - bitmapGlyph.mSrcRect.mYMin;
					
					if ( !bitmapGlyph.mIsWhitespace ) {
					
						int glyphHeight = bitmapGlyph.mSrcRect.Height ();
						if (( u32 )glyphHeight > this->mHeight ) {
							this->mHeight = glyphHeight;
						}
						
						int base = y - bitmapGlyph.mSrcRect.mYMin;
						if (( u32 )base > this->mBase ) {
							this->mBase = base;
						}
					}
				}
			}
		}
	}
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	loadPage
	@text	Rips a set of glyphs from a bitmap and associates them with a size.

	@in		MOAIBitmapFontReader self
	@in		string filename			Filename of the image containing the bitmap font.
	@in		string charCodes		A string which defines the characters found in the bitmap
	@in		number points			The point size to be associated with the glyphs ripped from the bitmap.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
*/
int MOAIBitmapFontReader::_loadPage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBitmapFontReader, "USSN" )
	
	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	cc8* charCodes	= state.GetValue < cc8* >( 3, "" );
	float points	= state.GetValue < float >( 4, 0 );
	float dpi		= state.GetValue < float >( 5, DPI );
	
	float size = POINTS_TO_PIXELS ( points, dpi );
	
	self->LoadPage ( filename, size, charCodes );
	return 0;
}

//================================================================//
// MOAIBitmapFontReader
//================================================================//

//----------------------------------------------------------------//
int MOAIBitmapFontReader::GetFaceMetrics ( MOAIFontFaceMetrics& faceMetrics  ) {

	if ( !this->mCurrentPage ) return FONT_ERROR;

	faceMetrics.mAscent = ( float )this->mCurrentPage->mBase;
	faceMetrics.mHeight = ( float )this->mCurrentPage->mHeight;
	
	return OK;
}

//----------------------------------------------------------------//
int MOAIBitmapFontReader::GetGlyphMetrics ( MOAIGlyphMetrics& glyphMetrics ) {

	if ( !this->mCurrentGlyph ) return FONT_ERROR;
	
	MOAIBitmapGlyph& bitmapGlyph = *this->mCurrentGlyph;
	
	const int width = bitmapGlyph.mSrcRect.Width ();
	const int height = bitmapGlyph.mSrcRect.Height ();
	
	glyphMetrics.mWidth = ( float ) width;
	glyphMetrics.mHeight = ( float ) height;
	glyphMetrics.mAdvanceX = glyphMetrics.mWidth;
	glyphMetrics.mBearingX = 0.0f;
	glyphMetrics.mBearingY = ( float )bitmapGlyph.mBase;
	
	if ( bitmapGlyph.mIsWhitespace ) {
		glyphMetrics.mWidth = 0.0f;
	}
	
	return OK;
}

//----------------------------------------------------------------//
void MOAIBitmapFontReader::LoadPage ( cc8* filename, float size, cc8* charCodes ) {

	MOAIBitmapFontPage& page = this->mPages [ size ];
	page.RipBitmap ( filename, charCodes );
}

//----------------------------------------------------------------//
MOAIBitmapFontReader::MOAIBitmapFontReader () :
	mCurrentPage ( 0 ),
	mCurrentPageSize ( 0.0f ),
	mCurrentGlyph ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFontReader )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBitmapFontReader::~MOAIBitmapFontReader () {
}

//----------------------------------------------------------------//
void MOAIBitmapFontReader::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIFontReader::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBitmapFontReader::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIFontReader::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "loadPage",				_loadPage },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
int MOAIBitmapFontReader::RenderGlyph ( MOAIImage& image, float x, float y, const ZLColorBlendFunc& blendFunc ) {
	UNUSED ( blendFunc );

	if ( !this->mCurrentGlyph ) return FONT_ERROR;
	
	MOAIBitmapGlyph& bitmapGlyph = *this->mCurrentGlyph;

	ZLIntRect destRect;
	destRect.mXMin = ( int )x;
	destRect.mYMin = ( int )( y -= bitmapGlyph.mBase );
	destRect.mXMax = destRect.mXMin + ( int )bitmapGlyph.mSrcRect.Width ();
	destRect.mYMax = destRect.mYMin + ( int )bitmapGlyph.mSrcRect.Height ();
	
	y -= bitmapGlyph.mBase;
	
	image.CopyRect (
		this->mCurrentPage->mImage,
		bitmapGlyph.mSrcRect,
		destRect,
		MOAIImage::FILTER_LINEAR,
		blendFunc
	);
}

//----------------------------------------------------------------//
int MOAIBitmapFontReader::SelectFace ( float size ) {

	if ( this->mCurrentPageSize != size ) {
	
		this->mCurrentPage = 0;
		this->mCurrentGlyph = 0;
	
		if ( this->mPages.contains ( size )) {
			this->mCurrentPage = &this->mPages [ size ];
			return OK;
		}
	}
	return FONT_ERROR;
}

//----------------------------------------------------------------//
int MOAIBitmapFontReader::SelectGlyph ( u32 c ) {

	if ( !this->mCurrentPage ) return FONT_ERROR;

	if ( this->mCurrentPage->mBitmapGlyphs.contains ( c )) {
		this->mCurrentGlyph = &this->mCurrentPage->mBitmapGlyphs [ c ];
		return OK;
	}
	this->mCurrentGlyph = 0;
	return FONT_ERROR;
}

//----------------------------------------------------------------//
void MOAIBitmapFontReader::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIFontReader::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIBitmapFontReader::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIFontReader::SerializeOut ( state, serializer );
}
