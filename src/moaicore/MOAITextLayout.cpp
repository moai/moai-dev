// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USBinarySearch.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIQuadBrush.h>
#include <moaicore/MOAITextLayout.h>

//================================================================//
// MOAITextLayout
//================================================================//

//----------------------------------------------------------------//
void MOAITextLayout::Draw ( u32 reveal ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIQuadBrush::BindVertexFormat ( gfxDevice );

	USColorVec baseColor = gfxDevice.GetPenColor ();
	USColorVec blendColor;
	u32 rgba = 0xffffffff;

	u32 size = this->mSprites.GetTop ();
	for ( u32 i = 0; ( i < size ) && ( i < reveal ); ++i ) {
		const MOAITextSprite& sprite = this->mSprites [ i ];
		
		if ( sprite.mRGBA != rgba ) {
			rgba = sprite.mRGBA;
			
			blendColor.SetRGBA ( rgba );
			blendColor.Modulate ( baseColor );
			gfxDevice.SetPenColor ( blendColor );
		}
		sprite.mGlyph->Draw ( sprite.mX, sprite.mY );
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::DrawDebugBaselines () {
	
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	u32 totalLines = this->mLines.GetTop ();
	for ( u32 i = 0; i < totalLines; ++i ) {
		MOAITextLine& line = this->mLines [ i ];
		
		float y = line.mRect.mYMin + line.mAscent;
		debugLines.DrawLine ( line.mRect.mXMin, y, line.mRect.mXMax, y );
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::DrawDebugLayout () {
	
	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	u32 totalLines = this->mLines.GetTop ();
	for ( u32 i = 0; i < totalLines; ++i ) {
		MOAITextLine& line = this->mLines [ i ];
		debugLines.DrawRect ( line.mRect );
	}
}

//----------------------------------------------------------------//
bool MOAITextLayout::GetBoundsForRange ( u32 idx, u32 size, USRect& rect ) {

	bool result = false;
	//u32 end = idx + size;
	//float x = 0.0f;

	//// TODO: replace w/ binary search
	//u32 top = this->GetTop ();
	//for ( u32 i = 0; i < top; ++i ) {
	//	MOAITextSprite& sprite = ( *this )[ i ];
	//	
	//	if ( sprite.mIdx >= end ) break;

	//	if ( sprite.mIdx >= idx ) {
	//		
	//		const MOAIGlyph* glyph = sprite.mGlyph;
	//		
	//		if ( glyph->mWidth ) {
	//			
	//			USRect glyphRect = glyph->GetRect ( sprite.mPoints, sprite.mX, sprite.mY );
	//			
	//			if ( result ) {
	//				if ( sprite.mX < x ) break;
	//				rect.Grow ( glyphRect );
	//			}
	//			else {
	//				rect = glyphRect;
	//				result = true;
	//			}
	//			x = sprite.mX;
	//		}
	//	}
	//}
	return result;
}

//----------------------------------------------------------------//
bool MOAITextLayout::More ( u32 reveal ) {

	if ( reveal <= this->mSprites.GetTop ()) {
		return true;
	}
	return this->mMore;
}

//----------------------------------------------------------------//
void MOAITextLayout::PushGlyph ( const MOAIGlyph* glyph, u32 idx, float x, float y, u32 rgba ) {

	MOAITextSprite textSprite;
	
	textSprite.mGlyph		= glyph;
	textSprite.mIdx			= idx;
	textSprite.mX			= x;
	textSprite.mY			= y;
	textSprite.mRGBA		= rgba;

	this->mSprites.Push ( textSprite );
}

//----------------------------------------------------------------//
void MOAITextLayout::PushLine ( u32 start, u32 size, const USRect& rect, float ascent ) {

	MOAITextLine textLine;
	
	textLine.mStart			= start;
	textLine.mSize			= size;
	textLine.mRect			= rect;
	textLine.mAscent		= ascent;
	
	this->mLines.Push ( textLine );
}

//----------------------------------------------------------------//
void MOAITextLayout::Reset () {

	this->mMore = false;
	this->mLines.Reset ();
	this->mSprites.Reset ();
}

//----------------------------------------------------------------//
void MOAITextLayout::SetColorForRange ( u32 idx, u32 size, u32 rgba ) {

	u32 end = idx + size;

	// TODO: replace w/ binary search
	u32 top = this->mSprites.GetTop ();
	for ( u32 i = 0; i < top; ++i ) {
		MOAITextSprite& sprite = this->mSprites [ i ];
		
		if ( sprite.mIdx >= end ) break;

		if ( sprite.mIdx >= idx ) {
			sprite.mRGBA = rgba;
		}
	}
}

//----------------------------------------------------------------//
u32 MOAITextLayout::Size () {

	return this->mSprites.GetTop ();
}