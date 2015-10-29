// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextDesigner.h>
#include <moai-sim/MOAITextDesignParser.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleCache.h>
#include <moai-sim/MOAITextStyleMap.h>

//================================================================//
// MOAITextDesignParser
//================================================================//

//----------------------------------------------------------------//
void MOAITextDesignParser::AcceptLine () {

	u32 totalLines = this->mLayout->mLines.GetTop ();

	this->mLayout->PushLine ( this->mLineSpriteID, this->mLineSize, this->mLineRect, this->mLineHeight, this->mLineAscent );
	
	float lineWidth = this->mLineRect.Width ();
	
	this->mLayoutWidth = lineWidth > this->mLayoutWidth ? lineWidth : this->mLayoutWidth;
	
	if ( totalLines == 0 ) {
		this->mLayoutHeightMin = 0.0f;
		this->mLayoutHeightMax = this->mLineHeight;
	}
	else {
		float min = this->mLayoutHeightMax + this->mDesigner->mLineSpacing;
		float max = min + this->mLineHeight;
		this->mLayoutHeightMin = min < this->mLayoutHeightMin ? min : this->mLayoutHeightMin;
		this->mLayoutHeightMax = max > this->mLayoutHeightMax ? max : this->mLayoutHeightMax;
	}
	
	// next line
	this->mLineSize = 0;
	this->mLineIdx = this->mTokenIdx;
	this->mLineSpriteID = this->mTokenSpriteID;
	this->mLineHeight = 0.0f;
	this->mLineAscent = 0.0f;
	
	this->mPrevGlyph = 0;
	
	if ( this->mTokenSize ){
		
		// slide the current token (if any) back to the origin
		for ( u32 i = 0; i < this->mTokenSize; ++i ) {
			MOAITextSprite& sprite = this->mLayout->mSprites [ this->mTokenSpriteID + i ];
			sprite.mPen.mX -= this->mTokenRect.mXMin;
		}
		
		this->mPenX -= this->mTokenRect.mXMin;
		this->mTokenRect.Init ( 0.0f, 0.0f, this->mTokenRect.Width (), this->mTokenRect.Height ());
	}
	else {
		this->mPenX = 0.0f;
	}
}

//----------------------------------------------------------------//
void MOAITextDesignParser::AcceptToken () {

	if ( this->mTokenSize ) {
	
		if ( this->mLineSize == 0 ) {
			this->mLineIdx = this->mTokenIdx;
			this->mLineSpriteID = this->mTokenSpriteID;
		}
		
		if ( this->mLineSize == 0 ) {
			this->mLineRect = this->mTokenRect;
		}
		else {
			this->mLineRect.Grow ( this->mTokenRect );
		}
		
		this->mLineSize += this->mTokenSize;
		this->mLineHeight = this->mTokenHeight > this->mLineHeight ? this->mTokenHeight : this->mLineHeight;
		this->mLineAscent = this->mTokenAscent > this->mLineAscent ? this->mTokenAscent : this->mLineAscent;
		
		this->mTokenSize = 0;
		this->mTokenIdx = this->mPrevIdx;
		this->mTokenSpriteID = this->mLayout->mSprites.GetTop ();
	}
}

//----------------------------------------------------------------//
void MOAITextDesignParser::Align () {

	ZLRect layoutBounds = this->mLayout->mLayoutBounds;
	ZLRect glyphBounds;
	
	glyphBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	bool hasSprites = ( this->mLayout->mSprites.GetTop () > 0 );
	
	u32 baseLine = this->mBaseLine;
	u32 totalLines = this->mLayout->mLines.GetTop ();

	bool limitWidth = this->mDesigner->mLimitWidth;
	bool limitHeight = this->mDesigner->mLimitHeight;

	float width = limitWidth ? this->mDesigner->mFrame.Width () : this->mLayoutWidth;
	float height = limitHeight ? this->mDesigner->mFrame.Height () : this->GetLayoutHeight ();

	float layoutHeight = this->GetLayoutHeight ();

	float xMin = limitWidth ? this->mDesigner->mFrame.mXMin : 0.0f;
	float xMax = xMin + width;
	
	float yMin = limitHeight ? this->mDesigner->mFrame.mYMin : 0.0f;
	float yMax = yMin + height;
	
	float localXOffset = -this->Snap ( xMin + ( width * 0.5f ), this->mDesigner->mHLineSnap );
	float localYOffset = -this->Snap ( yMin + ( height * 0.5f ), this->mDesigner->mVLineSnap );
	
	float penY = yMin;
	
	switch ( this->mDesigner->mVAlign ) {
		
		case MOAITextDesigner::CENTER_JUSTIFY:
			penY = yMin + ( height * 0.5f ) - ( layoutHeight * 0.5f );
			break;
			
		case MOAITextDesigner::TOP_JUSTIFY:
			penY = yMin;
			break;

		case MOAITextDesigner::BOTTOM_JUSTIFY:
			penY = yMax - layoutHeight;
			break;
		
		default:
			break;
	}

	MOAIAnimCurve** curves = this->mDesigner->mCurves;
	u32 totalCurves = this->mDesigner->mCurves.Size ();
	
	u32 spriteCount = 0;
	
	for ( u32 i = baseLine; i < totalLines; ++i ) {
		
		MOAITextLine& line = this->mLayout->mLines [ i ];
		ZLRect& lineRect = line.mRect;
		float lineWidth = line.mRect.Width ();
		
		float lineX = xMin;
		float lineY = penY;
		
		penY += line.mHeight + this->mDesigner->mLineSpacing;
		
		switch ( this->mDesigner->mHAlign ) {
			
			case MOAITextDesigner::CENTER_JUSTIFY:
				lineX = xMin + ( width * 0.5f ) - ( lineWidth * 0.5f );
				break;
				
			case MOAITextDesigner::LEFT_JUSTIFY:
				lineX = xMin;
				break;

			case MOAITextDesigner::RIGHT_JUSTIFY:
				lineX = xMax - lineWidth;
				break;
			
			default:
				break;
		}

		lineX = this->Snap ( lineX + this->mOffset.mX, this->mDesigner->mHLineSnap ) + localXOffset;
		lineY = this->Snap ( lineY + this->mOffset.mY, this->mDesigner->mVLineSnap ) + localYOffset;
		
		float xOff = ( lineX - lineRect.mXMin );
		float yOff = ( lineY + line.mAscent );
		
		lineRect.Init ( lineX, lineY, lineX + lineWidth, lineY + line.mHeight );
		
		if ( hasSprites ) {
		
			MOAIAnimCurve* curve = curves ? curves [( i - baseLine ) % totalCurves ] : 0;
			
			for ( u32 j = 0; j < line.mSize; ++j ) {	
				MOAITextSprite& sprite = this->mLayout->mSprites [ line.mStart + j ];
				sprite.mPen.mX += xOff;
				sprite.mPen.mY += curve ? yOff + curve->GetValue (( sprite.mPen.mX - xMin ) / width ) : yOff;
				
				
				ZLRect glyphRect = sprite.mGlyph->GetRect ( sprite.mPen.mX, sprite.mPen.mY, sprite.mScale.mX, sprite.mScale.mY );
			
				const ZLRect& padding = sprite.mStyle->mPadding;
				glyphRect.mXMin += padding.mXMin;
				glyphRect.mYMin += padding.mYMin;
				glyphRect.mXMax += padding.mXMax;
				glyphRect.mYMax += padding.mYMax;
				
				if ( spriteCount == 0 ) {
					glyphBounds = glyphRect;
				}
				else {
					glyphBounds.Grow ( glyphRect );
				}
				
				spriteCount++;
			}
		}
		
		if ( i == 0 ) {
			layoutBounds = lineRect;
		}
		else {
			layoutBounds.Grow ( lineRect );
		}
	}
	
	// center texbox in model space to make autoflip easier to implement
	
	this->mLayout->mXOffset = -localXOffset;
	
	if ( limitWidth == false ) {
	
		switch ( this->mDesigner->mHAlign ) {
	
			case MOAITextDesigner::CENTER_JUSTIFY:
				this->mLayout->mXOffset = 0.0f;
				break;
				
			case MOAITextDesigner::LEFT_JUSTIFY:
				this->mLayout->mXOffset = -localXOffset;
				break;

			case MOAITextDesigner::RIGHT_JUSTIFY:
				this->mLayout->mXOffset = localXOffset;
				break;
			
			default:
				break;
		}
	}
	
	this->mLayout->mYOffset = -localYOffset;
	
	if ( limitHeight == false ) {
	
		switch ( this->mDesigner->mVAlign ) {
		
			case MOAITextDesigner::CENTER_JUSTIFY:
				this->mLayout->mYOffset = 0.0f;
				break;
				
			case MOAITextDesigner::TOP_JUSTIFY:
				this->mLayout->mYOffset = localYOffset;
				break;

			case MOAITextDesigner::BOTTOM_JUSTIFY:
				this->mLayout->mYOffset = -localYOffset;
				break;
			
			case MOAITextDesigner::BASELINE_JUSTIFY: {
				float sign = this->mDesigner->mYFlip ? -1.0f : 1.0f;
				this->mLayout->mYOffset = ( totalLines ? ( -localYOffset - this->mLayout->mLines [ 0 ].mAscent ) * sign : 0.0f );
				break;
			}
			
			default:
				break;
		}
	}
	
	this->mLayout->mGlyphBounds		= glyphBounds;
	this->mLayout->mLayoutBounds	= layoutBounds;
}

//----------------------------------------------------------------//
void MOAITextDesignParser::BuildLayout () {
	
	bool limitWidth = this->mDesigner->mLimitWidth;
	bool limitHeight = this->mDesigner->mLimitHeight;
	
	float frameWidth = this->mDesigner->mFrame.Width ();
	float frameHeight = this->mDesigner->mFrame.Height ();
	
	bool more = true;
	while ( more ) {
	
		u32 c = this->NextChar ();
		
		float xScale = this->mDesigner->mGlyphScale * ( this->mStyle ? this->mStyle->mScale.mX : 1.0f ) * this->mDeckScale;
		float yScale = this->mDesigner->mGlyphScale * ( this->mStyle ? this->mStyle->mScale.mY : 1.0f ) * this->mDeckScale;
		
		if ( MOAIFont::IsControl ( c )) {
		
			if ( c == '\n' ) {
				
				this->AcceptToken ();
				
				float newLineSize = this->mDeck->mHeight * yScale;
				
				this->mLineHeight = this->mLineHeight == 0.0f ? newLineSize : this->mLineHeight;
				this->mLineAscent = this->mLineAscent == 0.0f ? newLineSize : this->mLineAscent;
				
				this->AcceptLine ();	
			}
			else if ( c == 0 ) {
			
				this->AcceptToken ();
				this->AcceptLine ();
			
				this->mMore = false;
				more = false;
			}
		}
		else {
			MOAIGlyph* glyph = this->mDeck->GetGlyph ( c );
			if ( !glyph ) continue;
			if ( glyph->mCode == MOAIGlyph::NULL_CODE_ID ) continue;
			
			// apply kerning
			if ( this->mPrevGlyph ) {
				MOAIKernVec kernVec = this->mPrevGlyph->GetKerning ( glyph->mCode );
				this->mPenX += kernVec.mX * xScale;
			}
			
			this->mPrevGlyph = glyph;
			
			if ( MOAIFont::IsWhitespace ( c )) {
				this->AcceptToken ();
			}
			else {
				
				ZLRect glyphRect = glyph->GetRect ( this->mPenX, 0.0f, xScale, yScale );
				
				// handle new token
				if ( this->mTokenSize == 0 ) {
					this->mTokenIdx = this->mPrevIdx;
					this->mTokenSpriteID = this->mLayout->mSprites.GetTop ();
					this->mTokenRect = glyphRect;
					this->mTokenHeight = this->mDeck->mHeight * yScale;
					this->mTokenAscent = this->mDeck->mAscent * yScale;
				}
				
				bool overrun = false;
				
				ZLRect tokenRect = this->mTokenRect;
				tokenRect.Grow ( glyphRect );
				
				if ( limitWidth ) {
					overrun = frameWidth < tokenRect.mXMax;
				}
				
				bool discard = ( this->mLineSize == 0 ) && overrun; // this is the first token in the line *and* we have overrun
				
				// if we're the first token in a line *and* have overrun, don't attempt to split the token - just
				// discard the extra glyphs. later on this will be the place to implement fancy/custom token splitting.
				if ( discard ) {
					this->mOverrun = true;
				}
				else {
					// push the sprite
					this->mLayout->PushSprite ( this->mPrevIdx, *glyph, *this->mStyle, this->mPenX, 0.0f, xScale, yScale );
					this->mTokenRect = tokenRect;
					this->mTokenSize++;
				}
				
				if ( overrun ) {
					this->AcceptLine ();
				}
				
				if ( this->mDesigner->mWordBreak == MOAITextDesigner::WORD_BREAK_CHAR ) {
					this->AcceptToken ();
				}
			}
			
			// advance the pen
			this->mPenX += glyph->mAdvanceX * xScale;
		}
		
		// if we overrun this->mHeight, then back up to the start of the current line
		if ( limitHeight && (( this->GetLayoutHeight () + this->mTokenHeight ) > frameHeight )) {
			
			this->mLayout->mSprites.SetTop ( this->mLineSpriteID );
			
			// if we're ending on an empty line (i.e. a newline) then throw it away
			// else back up so the next page will start on the line
			if ( this->mLineSize ) {
				this->mIdx = this->mLineIdx;
			}
			else if ( this->mTokenSize ) {
				this->mIdx = this->mTokenIdx;
			}
			else {
				this->mIdx = this->mIdx;
			}
			
			more = false;
		}
	}

	this->Align ();
}

//----------------------------------------------------------------//
void MOAITextDesignParser::BuildLayout ( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, MOAITextDesigner& designer, cc8* str, u32 idx, ZLVec2D& offset ) {
	
	if ( styleMap.CountSpans () == 0 ) return;
	
	this->mLayout = &layout;
	this->mStyleCache = &styleCache;
	this->mStyleMap = &styleMap;
	this->mDesigner = &designer;
	
	this->mStr = str;
	this->mIdx = idx;
	this->mPrevIdx = idx;
	
	this->mStyleSpan = 0;
	this->mStyle = 0;
	
	this->mLineIdx = this->mIdx;
	this->mLineSpriteID = 0;
	this->mLineSize = 0;
	this->mLineHeight = 0.0f;
	this->mLineAscent = 0.0f;
	
	this->mLineRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mTokenIdx = this->mIdx;
	this->mTokenSpriteID = 0;
	this->mTokenSize = 0;
	this->mTokenHeight = 0.0f;
	this->mTokenAscent = 0.0f;
	
	this->mTokenRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mLayoutWidth = 0.0f;
	this->mLayoutHeightMin = 0.0f;
	this->mLayoutHeightMax = 0.0f;
	
	this->mPenX = 0.0f;
	this->mOffset = offset;
	this->mPrevGlyph = 0;
	
	this->mMore = true;
	this->mOverrun = false;
	
	this->mBaseLine = layout.mLines.GetTop ();
	
	this->BuildLayout ();
}

//----------------------------------------------------------------//
float MOAITextDesignParser::GetLayoutHeight () {

	return this->mLayoutHeightMax - this->mLayoutHeightMin;
}

//----------------------------------------------------------------//
MOAITextDesignParser::MOAITextDesignParser () :
	mDesigner ( 0 ),
	mLayout ( 0 ),
	mStyleCache ( 0 ),
	mStyleMap ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextDesignParser::~MOAITextDesignParser () {
}

//----------------------------------------------------------------//
bool MOAITextDesignParser::More () {

	return this->mMore;
}

//----------------------------------------------------------------//
u32 MOAITextDesignParser::NextChar () {

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
		
			MOAITextStyleState* defaultStyle = this->mStyleCache->GetStyle ();
			MOAIFont* defaultFont = defaultStyle ? defaultStyle->mFont : 0;
		
			if ( this->mIdx < this->mStyleSpan->mBase ) {
				this->mIdx = this->mStyleSpan->mBase;
			}
		
			this->mStyle = this->mStyleSpan->mStyle;
			this->mStyle = this->mStyle ? this->mStyle : defaultStyle;
			if ( !this->mStyle ) return 0; // TODO: report error
			
			MOAIFont* font = this->mStyle->mFont;
			font = font ? font : defaultFont;
			if ( !font ) return 0; // TODO: report error
			
			this->mDeck = font->GetGlyphSet ( this->mStyle->mSize );
			if ( !this->mDeck && defaultFont ) {
				this->mDeck = defaultFont->GetGlyphSet ( this->mStyle->mSize );
			}
			if ( !this->mDeck ) return 0; // TODO: report error
			
			this->mDeckScale = this->mDeck && ( this->mStyle->mSize > 0.0f ) ? this->mStyle->mSize / this->mDeck->GetSize () : 1.0f;
		}
		
		this->mPrevIdx = this->mIdx;
		u32 c = moai_u8_nextchar ( this->mStr, &this->mIdx );
		
		return c;
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAITextDesignParser::Overrun () {

	return this->mMore || this->mOverrun;
}

//----------------------------------------------------------------//
float MOAITextDesignParser::Snap ( float f, float b ) {

	return ( b > 0.0f ) ? floorf (( f / b ) + 0.5f ) * b : f;
}
