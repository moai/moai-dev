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
void MOAITextDesigner::AcceptLine () {

	this->mTextBox->PushLine ( this->mLineSpriteID, this->mLineSize, this->mLineRect, this->mLineAscent );
			
	// end line
	this->mPen.mY += this->mLineRect.Height () + this->mTextBox->mLineSpacing;
	this->mLineRect.Init ( 0.0f, this->mPen.mY, 0.0f, this->mPen.mY );
	
	// next line
	this->mLineIdx = this->mTokenIdx;
	this->mLineSpriteID = this->mTokenSpriteID;
	this->mLineSize = 0;
	this->mLineAscent = 0.0f;
	
	this->mPrevGlyph = 0;
	
	if ( this->mTokenSize ){
		
		// slide the current token (if any) back to the origin
		for ( u32 i = 0; i < this->mTokenSize; ++i ) {
			MOAITextSprite& sprite = this->mTextBox->mSprites [ this->mTokenSpriteID + i ];
			sprite.mX -= this->mTokenRect.mXMin;
			sprite.mY = this->mPen.mY;
		}
		
		this->mPen.mX -= this->mTokenRect.mXMin;
		this->mTokenRect.Init ( 0.0f, this->mPen.mY, this->mTokenRect.Width (), this->mPen.mY + this->mTokenRect.Height ());
	}
	else {
		this->mPen.mX = 0.0f;
		this->mTokenRect.Init ( 0.0f, this->mPen.mY, 0.0f, this->mPen.mY + this->mDeck->mHeight );
	}
}

//----------------------------------------------------------------//
void MOAITextDesigner::AcceptToken () {

	this->mLineRect.Grow ( this->mTokenRect );
	this->mLineSize += this->mTokenSize;
	this->mLineAscent = this->mTokenAscent > this->mLineAscent ? this->mTokenAscent : this->mLineAscent;
	this->mTokenSize = 0;
}

//----------------------------------------------------------------//
void MOAITextDesigner::BuildLayout () {
	
	if ( !this->mTextBox ) return;
	
	bool more = true;
	while ( more ) {
	
		u32 c = this->NextChar ();
		
		float scale = this->mTextBox->mGlyphScale * ( this->mStyle ? this->mStyle->mScale : 1.0f );
		
		if ( MOAIFont::IsControl ( c )) {
		
			if ( c == '\n' ) {
				
				this->mTokenIdx = this->mPrevIdx;
				this->mTokenSpriteID = this->mTextBox->mSprites.GetTop ();
				
				if ( !this->mTokenRect.Height ()) {
					this->mTokenRect.mYMax += this->mDeck->mHeight * scale;
				}
				
				this->AcceptToken ();
				this->AcceptLine ();
			}
			else if ( c == 0 ) {
				this->mTextBox->mMore = false;
				this->mTokenIdx = this->mPrevIdx;
				this->mTokenSpriteID = this->mTextBox->mSprites.GetTop ();
				more = false;
				
				this->AcceptToken ();
				this->AcceptLine ();
			}
		}
		else {
			
			this->mGlyph = this->mDeck->GetGlyph ( c );
			
			if ( !this->mGlyph ) continue;
			if ( this->mGlyph->mAdvanceX == 0.0f ) continue;
			
			// apply kerning
			if ( this->mPrevGlyph ) {
				MOAIKernVec kernVec = this->mPrevGlyph->GetKerning ( this->mGlyph->mCode );
				this->mPen.mX += kernVec.mX * scale;
			}
			
			this->mPrevGlyph = this->mGlyph;
			
			if ( MOAIFont::IsWhitespace ( c )) {
				if ( this->mTokenSize ) {
					this->AcceptToken ();
				}
			}
			else {
				
				float glyphBottom = this->mPen.mY + ( this->mDeck->mHeight * scale );
				
				// handle new token
				if ( !this->mTokenSize ) {
					this->mTokenIdx = this->mPrevIdx;
					this->mTokenSpriteID = this->mTextBox->mSprites.GetTop ();
					this->mTokenRect.Init ( this->mPen.mX, this->mPen.mY, this->mPen.mX, glyphBottom );
					this->mTokenAscent = this->mDeck->mAscent * scale;
				}
				
				// check for overrun
				float glyphRight = this->mPen.mX + (( this->mGlyph->mBearingX + this->mGlyph->mWidth ) * scale );
				bool overrun = glyphRight > this->mWidth;
				bool acceptLine = ( this->mLineSize && overrun );
				
				if ( acceptLine || !overrun ) {
					this->mTextBox->PushSprite ( this->mPrevIdx, *this->mGlyph, *this->mStyle, this->mPen.mX, this->mPen.mY, scale );
					this->mTokenRect.mXMax = glyphRight;
					this->mTokenSize++;
				}
				
				if ( acceptLine ) {
					this->AcceptLine ();
				}
				
				if ( this->mTextBox->mWordBreak == MOAITextBox::WORD_BREAK_CHAR ) {
					this->AcceptToken ();
				}
			}
			
			this->mPen.mX += this->mGlyph->mAdvanceX * scale;
		}
		
		// if we overrun this->mHeight, then back up to the start of the current line
		if ( this->mTokenRect.mYMax > this->mHeight ) {
			this->mTextBox->mSprites.SetTop ( this->mLineSpriteID );
			
			// if we're ending on an empty line (i.e. a newline) then throw it away
			// else back up so the next page will start on the line
			if ( this->mLineSize ) {
				this->mTextBox->mNextPageIdx = this->mLineIdx;
			}
			else if ( this->mTokenSize ) {
				this->mTextBox->mNextPageIdx = this->mPrevIdx;
			}
			else {
				this->mTextBox->mNextPageIdx = this->mIdx;
			}
			
			more = false;
		}
	}
	
	bool hasSprites = ( this->mTextBox->mSprites.GetTop () > 0 );
	
	float yOff = this->mTextBox->mFrame.mYMin;
	float layoutHeight = this->mLineRect.mYMax;

	switch ( this->mTextBox->mVAlign ) {
		
		case MOAITextBox::CENTER_JUSTIFY:
			yOff = ( yOff + ( this->mHeight * 0.5f )) - ( layoutHeight * 0.5f );
			
		case MOAITextBox::LEFT_JUSTIFY:
			break;

		case MOAITextBox::RIGHT_JUSTIFY:
			yOff = this->mTextBox->mFrame.mYMax - layoutHeight;
	}
	
	u32 totalLines = this->mTextBox->mLines.GetTop ();
	for ( u32 i = 0; i < totalLines; ++i ) {
		MOAITextLine& line = this->mTextBox->mLines [ i ];
		
		float xOff = this->mTextBox->mFrame.mXMin;
		float lineWidth = line.mRect.Width ();
		
		switch ( this->mTextBox->mHAlign ) {
		
			case MOAITextBox::CENTER_JUSTIFY:
				xOff = ( xOff + ( this->mWidth * 0.5f )) - ( lineWidth * 0.5f );
				
			case MOAITextBox::LEFT_JUSTIFY:
				break;

			case MOAITextBox::RIGHT_JUSTIFY:
				xOff = this->mTextBox->mFrame.mXMax - lineWidth;
		}
		
		line.mRect.Offset ( xOff, yOff );
		
		if ( hasSprites ) {
		
			float spriteYOff = yOff + line.mAscent;
			
			MOAIAnimCurve* curve = 0;
			if ( this->mTextBox->mCurves ) {
				curve = this->mTextBox->mCurves [ i % this->mTextBox->mCurves.Size ()];
			}
			
			for ( u32 j = 0; j < line.mSize; ++j ) {	
				MOAITextSprite& sprite = this->mTextBox->mSprites [ line.mStart + j ];
				
				sprite.mX += xOff;
				
				if ( curve ) {
					sprite.mY += spriteYOff + curve->GetValue (( sprite.mX - this->mTextBox->mFrame.mXMin ) / this->mWidth );
				}
				else {
					sprite.mY += spriteYOff;
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAITextDesigner::Init ( MOAITextBox& textBox ) {
	
	this->mTextBox = 0;
	
	if ( !textBox.mStyleMap.GetTop ()) return;
	
	this->mTextBox = &textBox;
	
	this->mStr = this->mTextBox->mText;
	this->mIdx = this->mTextBox->mCurrentPageIdx;
	this->mPrevIdx = this->mIdx;
	this->mStyleSpan = 0;
	this->mStyle = 0;
	
	this->mTokenIdx = this->mIdx;
	this->mLineIdx = this->mIdx;
	
	this->mWidth = this->mTextBox->mFrame.Width ();
	this->mHeight = this->mTextBox->mFrame.Height ();
	
	this->mLineSpriteID = 0;
	this->mLineSize = 0;
	
	this->mTokenSpriteID = 0;
	this->mTokenSize = 0;
	
	this->mLineRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mTokenRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mTokenAscent = 0.0f;
	this->mLineAscent = 0.0f;

	this->mPen.Init ( 0.0f, 0.0f );
	
	this->mGlyph = 0;
	this->mPrevGlyph = 0;
	
	this->mTextBox->mMore = true;
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
		this->mStyleSpan = &this->mTextBox->mStyleMap.Elem ( 0 );
		this->mSpanIdx = 0;
		newSpan = true;
	}

	if ( this->mIdx >= this->mStyleSpan->mTop ) {
		
		this->mStyleSpan = 0;
		
		u32 totalStyles = this->mTextBox->mStyleMap.GetTop ();
		for ( this->mSpanIdx++; this->mSpanIdx < totalStyles; this->mSpanIdx++ ) {
			MOAITextStyleSpan& styleSpan = this->mTextBox->mStyleMap.Elem ( this->mSpanIdx );
			
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
			
			this->mDeck = font->GetGlyphSet ( this->mStyle->mSize );
		}
		
		this->mPrevIdx = this->mIdx;
		u32 c = u8_nextchar ( this->mStr, &this->mIdx );
		return c;
	}
	return 0;
}
