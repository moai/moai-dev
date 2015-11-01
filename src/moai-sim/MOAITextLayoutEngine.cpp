// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextLayoutRules.h>
#include <moai-sim/MOAITextLayoutEngine.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextShaper.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleCache.h>
#include <moai-sim/MOAITextStyleMap.h>

//================================================================//
// MOAITextLayoutEngine
//================================================================//

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::PushLine () {

	u32 totalLines = this->mLayout->mLines.GetTop ();

	float yDescend = 0.0f;

	// this works because all the rectangles are first drawn with their baselines at the origin
	if ( totalLines > 0 ) {
		yDescend = ( this->mPrevLineSpacingBounds.mYMax - this->mLineSpacingBounds.mYMin ) + this->mDesigner->mLineSpacing;
		this->mLineLayoutBounds.Offset ( 0.0f, yDescend );
		this->mLineSpacingBounds.Offset ( 0.0f, yDescend );
	}
	this->mPrevLineSpacingBounds = this->mLineSpacingBounds;
	
	// TODO: check for overrun here
	
	this->mLayout->PushLine ( this->mLineSpriteIdx, this->mLineSizeInSprites, ZLVec2D ( 0.0f, yDescend ), this->mLineLayoutBounds );
	
	this->mLayoutBounds.Grow ( this->mLineLayoutBounds, ( totalLines > 0 ));
	
	this->mLineCharIdx = this->GetCharIdx ();
	this->mLineSpriteIdx = this->GetSpriteIdx ();
	
	this->mLineSizeInSprites = 0;
	
	return PUSH_OK;
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::Align () {

	u32 totalLines = this->mLayout->mLines.GetTop ();

	if ( totalLines == 0 ) return;

	ZLRect logicalBounds;
	ZLRect glyphBounds;
	
	logicalBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	glyphBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	bool hasSprites = ( this->mLayout->mSprites.GetTop () > 0 );
	
	u32 baseLine = this->mBaseLine;
	
	bool limitWidth = this->mDesigner->mLimitWidth;
	bool limitHeight = this->mDesigner->mLimitHeight;

	// this is for alignment; text should already be within bounds, if constrained
	float width = limitWidth ? this->mDesigner->mFrame.Width () : this->mLayoutBounds.Width ();
	float height = limitHeight ? this->mDesigner->mFrame.Height () : this->mLayoutBounds.Height ();

	float layoutHeight = this->mLayoutBounds.Height ();

	float xMin = limitWidth ? this->mDesigner->mFrame.mXMin : 0.0f;
	float xMax = xMin + width;
	
	float yMin = limitHeight ? this->mDesigner->mFrame.mYMin : 0.0f;
	float yMax = yMin + height;
	
	float xOffsetToCenter = -this->Snap ( xMin + ( width * 0.5f ), this->mDesigner->mHLineSnap );
	float yOffsetToCenter = -this->Snap ( yMin + ( height * 0.5f ), this->mDesigner->mVLineSnap );
	
	float adjustedLayoutYMin = yMin;
	
	switch ( this->mDesigner->mVAlign ) {
		
		case MOAITextLayoutRules::CENTER_JUSTIFY:
			adjustedLayoutYMin = yMin + ( height * 0.5f ) - ( layoutHeight * 0.5f );
			break;
			
		case MOAITextLayoutRules::TOP_JUSTIFY:
			adjustedLayoutYMin = yMin;
			break;

		case MOAITextLayoutRules::BOTTOM_JUSTIFY:
			adjustedLayoutYMin = yMax - layoutHeight;
			break;
		
		default:
			break;
	}
	
	float lineYOffset = adjustedLayoutYMin - this->mLayoutBounds.mYMin;
	
	for ( u32 i = baseLine; i < totalLines; ++i ) {
		
		MOAITextLine& line = this->mLayout->mLines [ i ];
		const ZLRect& lineRect = line.mLayoutBounds;
		
		float lineWidth = lineRect.Width ();
		float lineHeight = lineRect.Height ();
		
		float adjustedLineXMin = xMin;
		float adjustedLineYMin = lineRect.mYMin + lineYOffset;
		//float lineY = penY;
		
		//penY += lineHeight + this->mDesigner->mLineSpacing;
		
		switch ( this->mDesigner->mHAlign ) {
			
			case MOAITextLayoutRules::CENTER_JUSTIFY:
				adjustedLineXMin = xMin + ( width * 0.5f ) - ( lineWidth * 0.5f );
				break;
				
			case MOAITextLayoutRules::LEFT_JUSTIFY:
				adjustedLineXMin = xMin;
				break;

			case MOAITextLayoutRules::RIGHT_JUSTIFY:
				adjustedLineXMin = xMax - lineWidth;
				break;
			
			default:
				break;
		}

		float adjustedLineX = line.mOrigin.mX + ( adjustedLineXMin - lineRect.mXMin );
		float adjustedLineY = line.mOrigin.mY + ( adjustedLineYMin - lineRect.mYMin );

		adjustedLineX = this->Snap ( adjustedLineX, this->mDesigner->mHLineSnap ) + xOffsetToCenter;
		adjustedLineY = this->Snap ( adjustedLineY, this->mDesigner->mVLineSnap ) + yOffsetToCenter;
		
		line.Offset (
			adjustedLineX - line.mOrigin.mX,
			adjustedLineY - line.mOrigin.mY
		);
		
		//MOAIAnimCurve* curve = curves ? curves [( i - baseLine ) % totalCurves ] : 0;
		
		for ( u32 j = 0; j < line.mSize; ++j ) {
		
			// TODO: restore anim curve!
			//sprite.mPen.mY += curve ? yOff + curve->GetValue (( sprite.mPen.mX - xMin ) / width ) : yOff;
		
			MOAITextSprite& sprite = this->mLayout->mSprites [ line.mStart + j ];
			
			sprite.mPen.mX += line.mOrigin.mX;
			sprite.mPen.mY += line.mOrigin.mY;
		}
	}
	
	// center texbox in model space to make autoflip easier to implement
	
	this->mLayout->mXOffset = -xOffsetToCenter;
	
	if ( limitWidth == false ) {
	
		switch ( this->mDesigner->mHAlign ) {
	
			case MOAITextLayoutRules::CENTER_JUSTIFY:
				this->mLayout->mXOffset = 0.0f;
				break;
				
			case MOAITextLayoutRules::LEFT_JUSTIFY:
				this->mLayout->mXOffset = -xOffsetToCenter;
				break;

			case MOAITextLayoutRules::RIGHT_JUSTIFY:
				this->mLayout->mXOffset = xOffsetToCenter;
				break;
			
			default:
				break;
		}
	}

	this->mLayout->mYOffset = -yOffsetToCenter;
	
	if ( limitHeight == false ) {
	
		switch ( this->mDesigner->mVAlign ) {
		
			case MOAITextLayoutRules::CENTER_JUSTIFY:
				this->mLayout->mYOffset = 0.0f;
				break;
				
			case MOAITextLayoutRules::TOP_JUSTIFY:
				this->mLayout->mYOffset = yOffsetToCenter;
				break;

			case MOAITextLayoutRules::BOTTOM_JUSTIFY:
				this->mLayout->mYOffset = -yOffsetToCenter;
				break;
			
			case MOAITextLayoutRules::BASELINE_JUSTIFY: {
				float sign = this->mDesigner->mYFlip ? 1.0f : -1.0f;
				float firstLineAscent = this->mLayout->mLines [ 0 ].GetAscent ();
				this->mLayout->mYOffset = ( yOffsetToCenter + firstLineAscent ) * sign;
				break;
			}
			
			default:
				break;
		}
	}

	this->mLayout->mLayoutBounds = this->mLayoutBounds;
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::BuildLayout () {
	
	bool limitWidth = this->mDesigner->mLimitWidth;
	bool limitHeight = this->mDesigner->mLimitHeight;
	
	float frameWidth = this->mDesigner->mFrame.Width ();
	float frameHeight = this->mDesigner->mFrame.Height ();
	
	bool more = true;
	while ( more ) {
		
		MOAITextSimpleShaper shaper;
		
		shaper.ShapeLine ( *this, *this, *this->mDesigner );
		this->PushLine ();
		
		more = this->mCurrentChar.mChar != 0;
		
//		if ( MOAIFont::IsControl ( c )) {
//		
//			if ( c == '\n' ) {
//				
//				float newLineSize = this->mDeck->mHeight * this->mCurrentChar.mScale.mY;
//				
//				this->mLineHeight = this->mLineHeight == 0.0f ? newLineSize : this->mLineHeight;
//				this->mLineAscent = this->mLineAscent == 0.0f ? newLineSize : this->mLineAscent;
//			}
//			else if ( c == 0 ) {
//				
//				more = false;
//			}
//		}
		
//		// if we overrun this->mHeight, then back up to the start of the current line
//		if ( limitHeight && (( this->GetLayoutHeight () + this->mTokenHeight ) > frameHeight )) {
//			
//			this->mLayout->mSprites.SetTop ( this->mLineSpriteIdx );
//			
//			// if we're ending on an empty line (i.e. a newline) then throw it away
//			// else back up so the next page will start on the line
//			if ( this->mLineSize ) {
//				this->mIdx = this->mLineCharIdx;
//			}
//			else if ( this->mTokenSize ) {
//				this->mIdx = this->mTokenIdx;
//			}
//			else {
//				this->mIdx = this->mIdx;
//			}
//			
//			more = false;
//		}
	}

	this->Align ();
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::BuildLayout ( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, MOAITextLayoutRules& designer, cc8* str, u32 idx ) {
	
	if ( styleMap.CountSpans () == 0 ) return;
	
	this->mLayout = &layout;
	this->mStyleCache = &styleCache;
	this->mStyleMap = &styleMap;
	this->mDesigner = &designer;
	
	this->mStr = str;
	this->mCharIdx = idx;
	
	this->mStyleSpan = 0;
	this->mStyle = 0;
	
	this->mLineCharIdx = 0;
	this->mLineSpriteIdx = 0;
	this->mLineSizeInSprites = 0;
	
	this->mLayoutBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mLineLayoutBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mLineSpacingBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mPrevLineSpacingBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mBaseLine = layout.mLines.GetTop ();
	
	this->BuildLayout ();
}

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::GetCharIdx () {

	return this->mCharIdx;
}

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::GetSpriteIdx () {

	return this->mLayout->mSprites.GetTop ();
}

//----------------------------------------------------------------//
MOAITextLayoutEngine::MOAITextLayoutEngine () :
	mDesigner ( 0 ),
	mLayout ( 0 ),
	mStyleCache ( 0 ),
	mStyleMap ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextLayoutEngine::~MOAITextLayoutEngine () {
}

//----------------------------------------------------------------//
bool MOAITextLayoutEngine::More () {

	//return this->mMore;
	return false;
}

//----------------------------------------------------------------//
MOAITextStyledChar MOAITextLayoutEngine::NextChar () {

	memset ( &this->mCurrentChar, 0, sizeof ( MOAITextStyledChar ));

	bool newSpan = false;

	if ( !this->mStyleSpan ) {
		this->mStyleSpan = &this->mStyleMap->Elem ( 0 );
		this->mSpanIdx = 0;
		newSpan = true;
	}

	if ( this->mCharIdx >= this->mStyleSpan->mTop ) {
		
		this->mStyleSpan = 0;
		
		u32 totalStyles = this->mStyleMap->GetTop ();
		for ( this->mSpanIdx++; this->mSpanIdx < totalStyles; this->mSpanIdx++ ) {
			MOAITextStyleSpan& styleSpan = this->mStyleMap->Elem ( this->mSpanIdx );
			
			if ( this->mCharIdx < styleSpan.mTop ) {
				this->mStyleSpan = &styleSpan;
				newSpan = true;
				break;
			}
		}
	}
	
	if ( this->mStyleSpan ) {
	
		MOAIGlyphSet* glyphDeck = 0;
		float deckScale = 1.0f;
	
		if ( newSpan ) {
		
			MOAITextStyleState* defaultStyle = this->mStyleCache->GetStyle ();
			MOAIFont* defaultFont = defaultStyle ? defaultStyle->mFont : 0;
		
			if ( this->mCharIdx < this->mStyleSpan->mBase ) {
				this->mCharIdx = this->mStyleSpan->mBase;
			}
			
			this->mStyle = this->mStyleSpan->mStyle;
			this->mStyle = this->mStyle ? this->mStyle : defaultStyle;
			if ( !this->mStyle ) return this->mCurrentChar; // TODO: report error
			
			MOAIFont* font = this->mStyle->mFont;
			font = font ? font : defaultFont;
			if ( !font ) return this->mCurrentChar; // TODO: report error
			
			glyphDeck = font->GetGlyphSet ( this->mStyle->mSize );
			if ( !glyphDeck && defaultFont ) {
				glyphDeck = defaultFont->GetGlyphSet ( this->mStyle->mSize );
			}
			
			if ( !glyphDeck ) return this->mCurrentChar; // TODO: report error
			
			deckScale = glyphDeck && ( this->mStyle->mSize > 0.0f ) ? this->mStyle->mSize / glyphDeck->GetSize () : 1.0f;
		}
		
		u32 charIdx = this->mCharIdx;
		u32 c = moai_u8_nextchar ( this->mStr, &this->mCharIdx ); // advances the character index
		
		this->mCurrentChar.mIdx = charIdx;
		this->mCurrentChar.mChar = c;
		
		this->mCurrentChar.mStyle = this->mStyle;
		this->mCurrentChar.mScale.mX = this->mDesigner->mGlyphScale * ( this->mStyle ? this->mStyle->mScale.mX : 1.0f ) * deckScale;
		this->mCurrentChar.mScale.mY = this->mDesigner->mGlyphScale * ( this->mStyle ? this->mStyle->mScale.mY : 1.0f ) * deckScale;
		
		if ( !MOAIFont::IsControl ( c )) {
			this->mCurrentChar.mGlyph = glyphDeck->GetGlyph ( c );
		}
	}
	return this->mCurrentChar;
}

//----------------------------------------------------------------//
bool MOAITextLayoutEngine::Overrun () {

	//return this->mMore || this->mOverrun;
	return false;
}

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::PushSprite ( const MOAITextStyledChar& styledChar, float x, float y ) {
	
	float xScale = styledChar.mScale.mX;
	float yScale = styledChar.mScale.mY;
	
	MOAIGlyph* glyph = styledChar.mGlyph;
	
	ZLRect glyphLayoutRect = this->mDesigner->GetGlyphLayoutRect ( *glyph, x, y, xScale, yScale );
	ZLRect glyphSpacingRect = this->mDesigner->GetGlyphSpacingRect ( *glyph, x, y, xScale, yScale );
	
	if ( this->mDesigner->mLimitWidth ) {
		float frameWidth = this->mDesigner->GetFrame ().Width ();
		if ( glyphLayoutRect.mXMax > frameWidth ) return PUSH_OVERRUN;
	}
	
	this->mLineLayoutBounds.Grow ( glyphLayoutRect, ( this->mLineSizeInSprites > 0 ));
	this->mLineSpacingBounds.Grow ( glyphSpacingRect, ( this->mLineSizeInSprites > 0 ));
	
	this->mLineSizeInSprites++;
	this->mLayout->PushSprite ( styledChar, x, y );
	
	return PUSH_OK;
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::SeekChar ( u32 charIdx ) {

	this->mCharIdx = charIdx;
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::SeekSprite ( u32 spriteIdx ) {

	this->mLayout->mSprites.SetTop ( spriteIdx );
}

//----------------------------------------------------------------//
float MOAITextLayoutEngine::Snap ( float f, float b ) {

	return ( b > 0.0f ) ? floorf (( f / b ) + 0.5f ) * b : f;
}
