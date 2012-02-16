// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAITextLayout.h>
#include <moaicore/MOAITextDesigner.h>
#include <moaicore/MOAITextStyle.h>
#include <moaicore/MOAITextStyleMap.h>

//================================================================//
// MOAITextDesigner
//================================================================//

//----------------------------------------------------------------//
//void MOAITextDesigner::FlushLine () {
//
//	if ( this->mLineCount >= this->mTotalLines ) return;
//
//	float x = this->mFrame.mXMin;
//	float width = this->mFrame.Width ();
//	
//	float lineWidth = this->mRightToLeft ? -this->mLineXMax : this->mLineXMax;
//	float lineHeight = this->mFont->GetLineSpacing () * this->mPoints * this->mLineSpacing;
//
//	switch ( this->mJustify ) {
//		
//		case CENTER_JUSTIFY:
//			x = ( x + ( width * 0.5f )) - ( lineWidth * 0.5f );
//			
//		case LEFT_JUSTIFY:
//			break;
//
//		case RIGHT_JUSTIFY:
//			x = this->mFrame.mXMax - lineWidth;
//	}
//	
//	MOAIAnimCurve* curve = 0;
//	if ( this->mCurves ) {
//		curve = this->mCurves [ this->mLineCount % this->mTotalCurves ];
//	}
//	
//	if ( this->mRightToLeft ) {
//		x -= this->mLineXMax;
//	}
//	
//	for ( u32 i = this->mLineBottom; i < this->mLineTop; ++i ) {
//		
//		MOAITextSprite& sprite = ( *this->mLayout )[ i ];
//		
//		float yOff = 0.0f;
//		if ( curve ) {
//			yOff = curve->GetFloatValue ( sprite.mX / width );
//		}
//
//		sprite.mX += x;
//		sprite.mY = this->mPen.mY + yOff;
//	}
//	
//	this->mLineBottom = this->mLineTop;
//	this->mLineXMax = 0.0f;
//	this->mPen.mY += lineHeight;
//	this->mLineCount++;
//	this->mGlyph = 0;
//	
//	// move token to origin
//	u32 tokenTop = this->mLayout->GetTop ();
//	for ( u32 i = this->mLineTop; i < tokenTop; ++i ) {
//		
//		MOAITextSprite& sprite = ( *this->mLayout )[ i ];
//		sprite.mX -= this->mTokenXMin;
//	}
//
//	this->mPen.mX -= this->mTokenXMin;
//	this->mTokenXMax -= this->mTokenXMin;
//	this->mTokenXMin = 0.0f;
//}

//----------------------------------------------------------------//
//void MOAITextDesigner::FlushToken () {
//	
//	float width = this->mFrame.Width ();
//	float tokenMax = this->mRightToLeft ? -this->mTokenXMax : this->mTokenXMax;
//	
//	if (( this->mLineXMax != 0.0f ) && ( tokenMax >= width )) {
//		this->FlushLine ();
//	}
//	
//	if ( this->mLineCount < this->mTotalLines ) {
//		this->mLineTop = this->mLayout->GetTop ();
//		this->mLineXMax = this->mTokenXMax;
//	}
//}

//----------------------------------------------------------------//
void MOAITextDesigner::Layout ( cc8* str, MOAITextStyleMap& styleMap, MOAITextLayout& layout ) {
	
	float width = 10000.0f;
	
	this->mStr = str;
	
	this->mSpanIdx = 0;
	this->mStyleMap = &styleMap;
	this->mStyleSpan = &this->mStyleMap->Elem ( 0 );
	this->mIdx = this->mStyleSpan->mBase;
	
	u32 lineStart = 0;
	u32 lineSize = 0;
	
	u32 tokenStart = 0;
	u32 tokenSize = 0;
	
	USRect lineRect;
	lineRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	USRect growRect;
	growRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->UpdateStyle ();

	USVec2D pen;
	pen.Init ( 0.0f, 0.0f );
	
	const MOAIGlyph* glyph = 0;
	const MOAIGlyph* prevGlyph = 0;
	
	bool more = true;
	while ( more ) {
	
		u32 c = this->NextChar ();
		
		if ( c == 0 ) {
			// end line
			lineRect = growRect;
			break;
		}
		
		prevGlyph = glyph;
		glyph = this->mDeck->GetGlyph ( c );
		if ( !glyph ) continue;
		if ( glyph->mAdvanceX == 0.0f ) continue;
		
		// apply kerning
		if ( prevGlyph ) {
			MOAIKernVec kernVec = prevGlyph->GetKerning ( glyph->mCode );
			pen.mX += kernVec.mX;
		}
		
		bool isWhitespace = MOAIFont::IsWhitespace ( c );
		
		if ( isWhitespace ) {
		
			if ( tokenSize ) {
				// accept token
				lineRect = growRect;
				lineSize += tokenSize;
				tokenSize = 0;
			}
		}
		else {
			
			// check for overrun
			float glyphRight = glyph->mBearingX + glyph->mWidth;
			
			if (( pen.mX + glyphRight ) > width ) {
				
				// end line
				pen.mX = 0.0f;
				growRect.mXMax = 0.0f;
				
				// next line
				lineStart = layout.GetTop ();
				lineSize = 0;
			}
			
			if ( !tokenSize ) {
				tokenStart = layout.GetTop ();
			}
			tokenSize++;
			
			// push the glyph
			layout.PushGlyph ( glyph, 0, pen.mX, 0.0f, 0xffffffff );
			growRect.mXMax = pen.mX + glyphRight;
		}
		
		// advance the pen
		pen.mX += glyph->mAdvanceX;
	}
	
	printf ( "\n" );
}

//----------------------------------------------------------------//
MOAITextDesigner::MOAITextDesigner () {
}

//----------------------------------------------------------------//
MOAITextDesigner::~MOAITextDesigner () {
}

//----------------------------------------------------------------//
u32 MOAITextDesigner::NextChar () {

	if ( !this->mStyleSpan ) return 0;

	if ( this->mIdx >= this->mStyleSpan->mTop ) {
		
		this->mSpanIdx++;
		this->mStyleSpan = 0;
		
		if ( this->mSpanIdx < this->mStyleMap->GetTop ()) {
			this->mStyleSpan = &this->mStyleMap->Elem ( this->mSpanIdx );
			this->mIdx = this->mStyleSpan->mBase;
			this->UpdateStyle ();
		}
	}

	u32 c = u8_nextchar ( this->mStr, &this->mIdx );
	return c;
}

//----------------------------------------------------------------//
void MOAITextDesigner::UpdateStyle () {

	assert ( this->mStyleSpan );
	
	MOAITextStyle* style = this->mStyleSpan->mStyle;
	assert ( style );
	
	MOAIFont* font = style->mFont;
	assert ( this->mStyleSpan->mStyle->mFont );
	
	this->mDeck = font->GetGlyphDeck ( style->mPoints );
}