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
	
	MOAIAnimCurve** curves = this->mDesigner->mCurves;
	u32 totalCurves = this->mDesigner->mCurves.Size ();
	
	for ( u32 i = baseLine; i < totalLines; ++i ) {
		
		MOAITextLine& line = this->mLayout->mLines [ i ];
		const ZLRect& lineRect = line.mLayoutBounds;
		
		float lineWidth = lineRect.Width ();
		float lineHeight = lineRect.Height ();
		
		float adjustedLineXMin = xMin;
		float adjustedLineYMin = lineRect.mYMin + lineYOffset;
		
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
		
		MOAIAnimCurve* curve = curves ? curves [( i - baseLine ) % totalCurves ] : 0;
		
		for ( u32 j = 0; j < line.mSize; ++j ) {
		
			MOAITextSprite& sprite = this->mLayout->mSprites [ line.mStart + j ];
		
			// calculate this *before* shifting the sprite - uses sprite.mPen.mX!
			float yOff = curve ? curve->GetValue (( sprite.mPen.mX - xMin ) / width ) : 0.0f;
		
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
void MOAITextLayoutEngine::BeginChar () {

	this->CaptureRestorePoint ( RESTORE_POINT_CHAR );
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::BeginToken () {

	this->mRestorePoints [ RESTORE_POINT_TOKEN ] = this->mRestorePoints [ RESTORE_POINT_CHAR ];
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::BuildLayout () {
	
	bool limitWidth = this->mDesigner->mLimitWidth;
	bool limitHeight = this->mDesigner->mLimitHeight;
	
	float frameWidth = this->mDesigner->mFrame.Width ();
	float frameHeight = this->mDesigner->mFrame.Height ();
	
	bool more = true;
	while ( more ) {
		
		u32 startingCharIdx = this->GetCharIdx ();
		
		MOAITextSimpleShaper shaper;
		u32 discard = shaper.ShapeLine ( *this, *this->mDesigner );
		
		if ( discard == MOAITextShaper::DISCARD_CHAR ) {
			this->Restore ( RESTORE_POINT_CHAR );
		}
		else if ( discard == MOAITextShaper::DISCARD_TOKEN ) {
			this->Restore ( RESTORE_POINT_TOKEN );
		}
		
		if ( this->GetCharIdx () > startingCharIdx ) {
		
			u32 result = this->PushLine ();
			
			more = (( result == PUSH_OK ) && ( this->mCurrentChar.mChar > 0 ));
		}
		else {
			more = false;
		}
	}

	// discard any sprites not already in a line
	this->mLayout->mSprites.SetTop ( this->mLineSpriteIdx );

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
	
	//this->mLineCharIdx = 0;
	this->mLineSpriteIdx = 0;
	//this->mLineSizeInSprites = 0;
	
	this->mLayoutBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mLineLayoutBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mLineSpacingBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mLineSpacingCursor = 0.0f;
	
	this->mBaseLine = layout.mLines.GetTop ();
	
	memset ( &this->mCurrentChar, 0, sizeof ( MOAITextStyledChar ));
	this->mCurrentChar.mScale.Init ( 1.0f, 1.0f );
	
	this->mCurrentGlyphDeck = 0;
	
	this->BuildLayout ();
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::CaptureRestorePoint ( u32 restorePointID ) {

	RestorePoint& restorePoint = this->mRestorePoints [ restorePointID ];
	
	restorePoint.mCharIdx				= this->mCharIdx;
	restorePoint.mSpriteIdx				= this->mLayout->mSprites.GetTop ();
	restorePoint.mLineLayoutBounds		= this->mLineLayoutBounds;
	restorePoint.mLineSpacingBounds		= this->mLineSpacingBounds;
}

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::GetCharIdx () {

	return this->mCharIdx;
}

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::GetLineSizeInSprites () {

	return this->mLayout->mSprites.GetTop () - this->mLineSpriteIdx;
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

	this->mCurrentChar.mChar = 0; // set this here so shaper will abort if we return due to an error.

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
			
			this->mCurrentGlyphDeck = font->GetGlyphSet ( this->mStyle->mSize );
			if ( !this->mCurrentGlyphDeck && defaultFont ) {
				this->mCurrentGlyphDeck = defaultFont->GetGlyphSet ( this->mStyle->mSize );
			}
			
			if ( !this->mCurrentGlyphDeck ) return this->mCurrentChar; // TODO: report error
			
			float deckScale = this->mCurrentGlyphDeck && ( this->mStyle->mSize > 0.0f ) ? this->mStyle->mSize / this->mCurrentGlyphDeck->GetSize () : 1.0f;
			
			this->mCurrentChar.mStyle = this->mStyle;
			this->mCurrentChar.mScale.mX = this->mDesigner->mGlyphScale * ( this->mStyle ? this->mStyle->mScale.mX : 1.0f ) * deckScale;
			this->mCurrentChar.mScale.mY = this->mDesigner->mGlyphScale * ( this->mStyle ? this->mStyle->mScale.mY : 1.0f ) * deckScale;
		}
		
		this->mCurrentChar.mIdx = this->mCharIdx;
		this->mCurrentChar.mChar = moai_u8_nextchar ( this->mStr, ( int* )&this->mCharIdx ); // advances the character index
		
		if ( !MOAIFont::IsControl ( this->mCurrentChar.mChar )) {
			this->mCurrentChar.mGlyph = this->mCurrentGlyphDeck->GetGlyph ( this->mCurrentChar.mChar );
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
u32 MOAITextLayoutEngine::PushLine () {

	u32 lineSizeInSprites = this->GetLineSizeInSprites ();
	
	if ( lineSizeInSprites == 0 ) {
	
		float ascent = this->mCurrentGlyphDeck->GetAscent ();
		float descent = this->mCurrentGlyphDeck->GetDescent ();
		
		this->mLineLayoutBounds.Init ( 0.0f, -ascent, 0.0f, descent );
		this->mLineSpacingBounds = this->mLineLayoutBounds;
	}

	u32 totalLines = this->mLayout->mLines.GetTop ();

	float yPen = 0.0f;

	// this works because all the rectangles are first drawn with their baselines at the origin
	if ( totalLines > 0 ) {
		yPen = ( this->mLineSpacingCursor - this->mLineSpacingBounds.mYMin ) + this->mDesigner->mLineSpacing;
		this->mLineLayoutBounds.Offset ( 0.0f, yPen );
		this->mLineSpacingBounds.Offset ( 0.0f, yPen );
	}
	this->mLineSpacingCursor = this->mLineSpacingBounds.mYMax;
	
	ZLRect newLayoutBounds = this->mLayoutBounds;
	newLayoutBounds.Grow ( this->mLineLayoutBounds, ( totalLines > 0 ));
	
	if ( this->mDesigner->mLimitHeight ) {
	
		float frameHeight = this->mDesigner->GetFrame ().Height ();
		if ( newLayoutBounds.Height () > frameHeight ) return PUSH_OVERRUN;
	}
	
	this->mLayout->PushLine ( this->mLineSpriteIdx, this->GetLineSizeInSprites (), ZLVec2D ( 0.0f, yPen ), this->mLineLayoutBounds );
	this->mLayoutBounds = newLayoutBounds;
	this->mLineSpriteIdx = this->GetSpriteIdx ();
	
	return PUSH_OK;
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
	
	u32 lineSizeInSprites = this->GetLineSizeInSprites ();
	
	this->mLineLayoutBounds.Grow ( glyphLayoutRect, ( lineSizeInSprites > 0 ));
	this->mLineSpacingBounds.Grow ( glyphSpacingRect, ( lineSizeInSprites > 0 ));
	
	this->mLayout->PushSprite ( styledChar, x, y );
	
	return PUSH_OK;
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::Restore ( u32 restorePointID ) {

	RestorePoint& restorePoint = this->mRestorePoints [ restorePointID ];
	
	this->mCharIdx				= restorePoint.mCharIdx;
	this->mLineLayoutBounds		= restorePoint.mLineLayoutBounds;
	this->mLineSpacingBounds	= restorePoint.mLineSpacingBounds;
	
	this->mLayout->mSprites.SetTop ( restorePoint.mSpriteIdx );
}

//----------------------------------------------------------------//
float MOAITextLayoutEngine::Snap ( float f, float b ) {

	return ( b > 0.0f ) ? floorf (( f / b ) + 0.5f ) * b : f;
}
