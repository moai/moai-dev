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
				
				float glyphRight = glyph->mBearingX + glyph->mWidth;
				float glyphBottom = pen.mY + this->mDeck->mHeight;
				
				// handle new token
				if ( !tokenSize ) {
					nextIdx = this->mIdx - 1;
					tokenStart = layout.mSprites.GetTop ();

					if (( glyphRight > width ) || ( glyphBottom > height )) {
						more = false;
						continue;
					}
					
					tokenRect.Init ( pen.mX, pen.mY, pen.mX, glyphBottom );
					tokenAscent = this->mDeck->mAscent;
				}
				
				// check for overrun
				
				bool overrun = ( pen.mX + glyphRight ) > width;
				acceptLine = ( lineSize && overrun );
				
				if ( acceptLine || !overrun ) {
					layout.PushGlyph ( glyph, 0, pen.mX, pen.mY, 0xffffffff );
					tokenRect.mXMax = pen.mX + glyphRight;
					tokenSize++;
				}
			}
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
	
	if ( !layout.Size ()) {
		layout.mMore = false;
		return idx;
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
