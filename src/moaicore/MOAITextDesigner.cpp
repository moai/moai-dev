// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAITextBox.h>
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
int MOAITextDesigner::Layout ( cc8* str, int idx, const USRect& frame, u32 hAlign, u32 vAlign, MOAITextStyleMap& styleMap, MOAITextLayout& layout ) {
	
	if ( !styleMap.Size ()) return idx;
	
	this->mStr = str;
	this->mIdx = idx;
	this->mStyleMap = &styleMap;
	this->mStyleSpan = 0;
	
	int nextIdx = idx;
	
	float width = frame.Width ();
	float height = frame.Height ();
	
	u32 lineStart = 0;
	u32 lineSize = 0;
	
	u32 tokenStart = 0;
	u32 tokenSize = 0;
	
	USRect lineRect;
	lineRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	USRect tokenRect;
	tokenRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	float tokenAscent = 0.0f;
	float lineAscent = 0.0f;

	USVec2D pen;
	pen.Init ( 0.0f, 0.0f );
	
	const MOAIGlyph* glyph = 0;
	const MOAIGlyph* prevGlyph = 0;
	
	layout.mMore = true;
	
	bool more = true;
	while ( more ) {
	
		u32 c = this->NextChar ();
		bool acceptToken = false;
		bool acceptLine = false;
		
		if ( c == 0 ) {
			layout.mMore = false;
			acceptToken = true;
			acceptLine = true;
			more = false;
		}
		else {
		
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
					acceptToken = true;
				}
			}
			else {
				
				// handle new token
				if ( !tokenSize ) {
					nextIdx = this->mIdx - 1;
					tokenStart = layout.mSprites.GetTop ();
					tokenRect.Init ( pen.mX, pen.mY, pen.mX, pen.mY + this->mDeck->mHeight );
					tokenAscent = this->mDeck->mAscent;
				}
				tokenSize++;
				
				// check for overrun
				float glyphRight = glyph->mBearingX + glyph->mWidth;
				
				if (( pen.mX + glyphRight ) > width ) {
					acceptLine = true;
				}

				// push the glyph
				layout.PushGlyph ( glyph, 0, pen.mX, pen.mY, 0xffffffff );
				tokenRect.mXMax = pen.mX + glyphRight;
			}
			
			// advance the pen
			pen.mX += glyph->mAdvanceX;
		}
		
		if ( acceptToken ) {
			
			lineRect.Grow ( tokenRect );
			lineSize += tokenSize;
			lineAscent = tokenAscent > lineAscent ? tokenAscent : lineAscent;
			tokenSize = 0;
		}
		
		if ( acceptLine ) {
			
			layout.PushLine ( lineStart, lineSize, lineRect, lineAscent );
			
			// end line
			pen.mX -= tokenRect.mXMin;
			pen.mY += lineRect.Height ();
			lineRect.Init ( 0.0f, pen.mY, 0.0f, pen.mY );
			
			// next line
			lineStart = tokenStart;
			lineSize = 0;
			lineAscent = 0.0f;
			
			// slide the current token (if any) back to the origin
			for ( u32 i = 0; i < tokenSize; ++i ) {
				MOAITextSprite& sprite = layout.mSprites [ tokenStart + i ];
				sprite.mX -= tokenRect.mXMin;
				sprite.mY = pen.mY;
			}
			
			tokenRect.Init ( 0.0f, pen.mY, tokenRect.Width (), pen.mY + tokenRect.Height ());
		}
		
		if ( tokenRect.mYMax > height ) {
			layout.mSprites.SetTop ( layout.mSprites.GetTop () - tokenSize );
			more = false;
		}
	}
	
	float yOff = frame.mYMin;
	float layoutHeight = lineRect.mYMax;

	switch ( vAlign ) {
		
		case MOAITextBox::CENTER_JUSTIFY:
			yOff = ( yOff + ( height * 0.5f )) - ( layoutHeight * 0.5f );
			
		case MOAITextBox::LEFT_JUSTIFY:
			break;

		case MOAITextBox::RIGHT_JUSTIFY:
			yOff = frame.mYMax - layoutHeight;
	}
	
	u32 totalLines = layout.mLines.GetTop ();
	for ( u32 i = 0; i < totalLines; ++i ) {
		MOAITextLine& line = layout.mLines [ i ];
		
		float xOff = frame.mXMin;
		float lineWidth = line.mRect.Width ();
		
		switch ( hAlign ) {
		
			case MOAITextBox::CENTER_JUSTIFY:
				xOff = ( xOff + ( width * 0.5f )) - ( lineWidth * 0.5f );
				
			case MOAITextBox::LEFT_JUSTIFY:
				break;

			case MOAITextBox::RIGHT_JUSTIFY:
				xOff = frame.mXMax - lineWidth;
		}
		
		line.mRect.Offset ( xOff, yOff );
		float spriteYOff = yOff + line.mAscent;
		
		for ( u32 j = 0; j < line.mSize; ++j ) {
			
			MOAITextSprite& sprite = layout.mSprites [ line.mStart + j ];
			
			sprite.mX += xOff;
			sprite.mY += spriteYOff;
		}
	}
	
	return nextIdx;
}

//----------------------------------------------------------------//
MOAITextDesigner::MOAITextDesigner () {
}

//----------------------------------------------------------------//
MOAITextDesigner::~MOAITextDesigner () {
}

//----------------------------------------------------------------//
u32 MOAITextDesigner::NextChar () {

	bool newSpan = false;

	if ( !this->mStyleSpan ) {
		this->mStyleSpan = &this->mStyleMap->Elem ( 0 );
		this->mSpanIdx = 0;
		newSpan = true;
	}

	if ( this->mIdx >= this->mStyleSpan->mTop ) {
		
		this->mStyleSpan = 0;
		
		u32 totalStyles = this->mStyleMap->GetTop ();
		for ( this->mSpanIdx++; this->mSpanIdx < totalStyles; this->mSpanIdx++ ) {
			MOAITextStyleSpan& styleSpan = this->mStyleMap->Elem ( this->mSpanIdx );
			
			if ( this->mIdx < styleSpan.mTop ) {
				this->mStyleSpan = &styleSpan;
				newSpan = true;
				break;
			}
		}
	}
	
	if ( this->mStyleSpan ) {
	
		if ( newSpan ) {
		
			if ( this->mIdx < this->mStyleSpan->mBase ) {
				this->mIdx = this->mStyleSpan->mBase;
			}
		
			MOAITextStyle* style = this->mStyleSpan->mStyle;
			assert ( style );
			
			MOAIFont* font = style->mFont;
			assert ( this->mStyleSpan->mStyle->mFont );
			
			this->mDeck = font->GetGlyphDeck ( style->mSize );
		}
	
		u32 c = u8_nextchar ( this->mStr, &this->mIdx );
		return c;
	}
	return 0;
}
