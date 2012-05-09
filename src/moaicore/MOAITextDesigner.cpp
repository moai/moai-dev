// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAITextBox.h>
#include <moaicore/MOAITextDesigner.h>
#include <moaicore/MOAITextStyle.h>

//================================================================//
// MOAITextDesigner
//================================================================//

//----------------------------------------------------------------//
void MOAITextDesigner::BuildLayout ( MOAITextBox& textBox ) {
	
	if ( !textBox.mStyleMap.GetTop ()) return;
	
	this->mStr = textBox.mText;
	this->mIdx = textBox.mCurrentPageIdx;
	this->mStyleSpan = 0;
	this->mStyle = 0;
	
	int tokenIdx = this->mIdx;
	int lineIdx = this->mIdx;
	
	float width = textBox.mFrame.Width ();
	float height = textBox.mFrame.Height ();
	
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
	
	MOAIGlyph* glyph = 0;
	MOAIGlyph* prevGlyph = 0;
	
	textBox.mMore = true;
	
	bool more = true;
	while ( more ) {
	
		u32 c = this->NextChar ( textBox );
		
		float scale = textBox.mGlyphScale * ( this->mStyle ? this->mStyle->mScale : 1.0f );
		
		bool acceptToken = false;
		bool acceptLine = false;
		
		if ( MOAIFont::IsControl ( c )) {
		
			if ( c == '\n' ) {
				
				tokenIdx = this->mIdx - 1;
				tokenStart = textBox.mSprites.GetTop ();
				acceptToken = true;
				acceptLine = true;
				
				if ( !tokenRect.Height ()) {
					tokenRect.mYMax += this->mDeck->mHeight * scale;
				}	
			}
			else if ( c == 0 ) {
				textBox.mMore = false;
				
				tokenIdx = this->mIdx - 1;
				tokenStart = textBox.mSprites.GetTop ();
				acceptToken = true;
				acceptLine = true;
				more = false;
			}
		}
		else {
			
			glyph = this->mDeck->GetGlyph ( c );
			
			if ( !glyph ) continue;
			if ( glyph->mAdvanceX == 0.0f ) continue;
			
			// apply kerning
			if ( prevGlyph ) {
				MOAIKernVec kernVec = prevGlyph->GetKerning ( glyph->mCode );
				pen.mX += kernVec.mX * scale;
			}
			
			prevGlyph = glyph;
			
			if ( MOAIFont::IsWhitespace ( c )) {
				if ( tokenSize ) {
					acceptToken = true;
				}
			}
			else {
				
				float glyphBottom = pen.mY + ( this->mDeck->mHeight * scale );
				
				// handle new token
				if ( !tokenSize ) {
					tokenIdx = this->mIdx - 1;
					tokenStart = textBox.mSprites.GetTop ();
					tokenRect.Init ( pen.mX, pen.mY, pen.mX, glyphBottom );
					tokenAscent = this->mDeck->mAscent * scale;
				}
				
				// check for overrun
				float glyphRight = pen.mX + (( glyph->mBearingX + glyph->mWidth ) * scale );
				bool overrun = glyphRight > width;
				acceptLine = ( lineSize && overrun );
				
				if ( acceptLine || !overrun ) {
					textBox.PushSprite ( this->mIdx - 1, *glyph, *this->mStyle, pen.mX, pen.mY, scale );
					tokenRect.mXMax = glyphRight;
					tokenSize++;
				}
			}
			
			pen.mX += glyph->mAdvanceX * scale;
		}
		
		if ( acceptToken ) {
			
			lineRect.Grow ( tokenRect );
			lineSize += tokenSize;
			lineAscent = tokenAscent > lineAscent ? tokenAscent : lineAscent;
			tokenSize = 0;
		}
		
		if ( acceptLine ) {
			
			textBox.PushLine ( lineStart, lineSize, lineRect, lineAscent );
			
			// end line
			pen.mY += lineRect.Height () + textBox.mLineSpacing;
			lineRect.Init ( 0.0f, pen.mY, 0.0f, pen.mY );
			
			// next line
			lineIdx = tokenIdx;
			lineStart = tokenStart;
			lineSize = 0;
			lineAscent = 0.0f;
			
			prevGlyph = 0;
			
			if ( tokenSize ){
				
				// slide the current token (if any) back to the origin
				for ( u32 i = 0; i < tokenSize; ++i ) {
					MOAITextSprite& sprite = textBox.mSprites [ tokenStart + i ];
					sprite.mX -= tokenRect.mXMin;
					sprite.mY = pen.mY;
				}
				
				pen.mX -= tokenRect.mXMin;
				tokenRect.Init ( 0.0f, pen.mY, tokenRect.Width (), pen.mY + tokenRect.Height ());
			}
			else {
				pen.mX = 0.0f;
				tokenRect.Init ( 0.0f, pen.mY, 0.0f, pen.mY + this->mDeck->mHeight );
			}
		}
		
		// if we overrun height, then back up to the start of the current line
		if ( tokenRect.mYMax > height ) {
			textBox.mSprites.SetTop ( lineStart );
			textBox.mNextPageIdx = lineIdx;
			more = false;
		}
	}
	
	if ( textBox.mSprites.GetTop () == 0 ) {
		textBox.mMore = false;
		return;
	}
		
	float yOff = textBox.mFrame.mYMin;
	float layoutHeight = lineRect.mYMax;

	switch ( textBox.mVAlign ) {
		
		case MOAITextBox::CENTER_JUSTIFY:
			yOff = ( yOff + ( height * 0.5f )) - ( layoutHeight * 0.5f );
			
		case MOAITextBox::LEFT_JUSTIFY:
			break;

		case MOAITextBox::RIGHT_JUSTIFY:
			yOff = textBox.mFrame.mYMax - layoutHeight;
	}
	
	u32 totalLines = textBox.mLines.GetTop ();
	for ( u32 i = 0; i < totalLines; ++i ) {
		MOAITextLine& line = textBox.mLines [ i ];
		
		float xOff = textBox.mFrame.mXMin;
		float lineWidth = line.mRect.Width ();
		
		switch ( textBox.mHAlign ) {
		
			case MOAITextBox::CENTER_JUSTIFY:
				xOff = ( xOff + ( width * 0.5f )) - ( lineWidth * 0.5f );
				
			case MOAITextBox::LEFT_JUSTIFY:
				break;

			case MOAITextBox::RIGHT_JUSTIFY:
				xOff = textBox.mFrame.mXMax - lineWidth;
		}
		
		line.mRect.Offset ( xOff, yOff );
		float spriteYOff = yOff + line.mAscent;
		
		MOAIAnimCurve* curve = 0;
		if ( textBox.mCurves ) {
			curve = textBox.mCurves [ i % textBox.mCurves.Size ()];
		}
		
		for ( u32 j = 0; j < line.mSize; ++j ) {
			
			MOAITextSprite& sprite = textBox.mSprites [ line.mStart + j ];
			
			sprite.mX += xOff;
			
			if ( curve ) {
				sprite.mY += spriteYOff + curve->GetFloatValue (( sprite.mX - textBox.mFrame.mXMin ) / width );
			}
			else {
				sprite.mY += spriteYOff;
			}
		}
	}
}

//----------------------------------------------------------------//
MOAITextDesigner::MOAITextDesigner () {
}

//----------------------------------------------------------------//
MOAITextDesigner::~MOAITextDesigner () {
}

//----------------------------------------------------------------//
u32 MOAITextDesigner::NextChar ( MOAITextBox& textBox ) {

	bool newSpan = false;

	if ( !this->mStyleSpan ) {
		this->mStyleSpan = &textBox.mStyleMap.Elem ( 0 );
		this->mSpanIdx = 0;
		newSpan = true;
	}

	if ( this->mIdx >= this->mStyleSpan->mTop ) {
		
		this->mStyleSpan = 0;
		
		u32 totalStyles = textBox.mStyleMap.GetTop ();
		for ( this->mSpanIdx++; this->mSpanIdx < totalStyles; this->mSpanIdx++ ) {
			MOAITextStyleSpan& styleSpan = textBox.mStyleMap.Elem ( this->mSpanIdx );
			
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
		
			this->mStyle = this->mStyleSpan->mStyle;
			assert ( this->mStyle );
			
			MOAIFont* font = this->mStyle->mFont;
			assert ( font );
			
			this->mDeck = font->GetGlyphDeck ( this->mStyle->mSize );
		}
	
		u32 c = u8_nextchar ( this->mStr, &this->mIdx );
		return c;
	}
	return 0;
}
