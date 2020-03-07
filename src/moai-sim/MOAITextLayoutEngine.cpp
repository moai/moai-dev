// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIAnimCurveFloat.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextLayoutRules.h>
#include <moai-sim/MOAITextLayoutEngine.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextShaper.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleCache.h>
#include <moai-sim/MOAITextStyleMap.h>

//================================================================//
// MOAILayoutEngineState
//================================================================//

//----------------------------------------------------------------//
MOAILayoutEngineState::MOAILayoutEngineState () :
	mStyleSpan ( 0 ),
	mSpanIdx ( 0 ),
	mCharIdx ( 0 ),
	mSpriteIdx ( 0 ) {
}

//----------------------------------------------------------------//
MOAILayoutEngineState::~MOAILayoutEngineState () {
}
	

//================================================================//
// MOAITextLayoutEngine
//================================================================//

//----------------------------------------------------------------//
void MOAITextLayoutEngine::Align () {

	ZLSize totalLines = this->mLayout->mLines.GetTop ();

	if ( totalLines == 0 ) return;

	ZLRect glyphFrame = ZLRect::EMPTY;
	ZLRect layoutFrame = ZLRect::EMPTY;
	
	ZLIndex baseLine = this->mBaseLine;
	
	bool limitWidth = this->mLayoutRules->mLimitWidth;
	bool limitHeight = this->mLayoutRules->mLimitHeight;
	
	// this is for alignment; text should already be within bounds, if constrained
	ZLReal width = limitWidth ? this->mLayoutFrameWithMargins.Width () : this->mLayoutBounds.Width ();
	ZLReal height = limitHeight ? this->mLayoutFrameWithMargins.Height () : this->mLayoutBounds.Height ();

	ZLReal layoutHeight = this->mLayoutBounds.Height ();

	ZLReal xMin = limitWidth ? this->mLayoutFrameWithMargins.mXMin : 0.0;
	ZLReal xMax = xMin + width;
	
	ZLReal yMin = limitHeight ? this->mLayoutFrameWithMargins.mYMin : 0.0;
	ZLReal yMax = yMin + height;
	
	ZLReal xOffsetToCenter = -this->Snap ( xMin + ( width * 0.5 ), this->mLayoutRules->mHLineSnap );
	ZLReal yOffsetToCenter = -this->Snap ( yMin + ( height * 0.5 ), this->mLayoutRules->mVLineSnap );
	
	ZLReal adjustedLayoutYMin = yMin;
	
	switch ( this->mLayoutRules->mVAlign ) {
		
		case MOAITextLayoutRules::CENTER_JUSTIFY:
			adjustedLayoutYMin = yMin + ( height * 0.5 ) - ( layoutHeight * 0.5 );
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
	
	ZLReal lineYOffset = adjustedLayoutYMin - this->mLayoutBounds.mYMin;
	
	MOAIAnimCurveFloat** curves = this->mLayoutRules->mCurves.GetBuffer ();
	ZLSize totalCurves = this->mLayoutRules->mCurves.Size ();
	
	for ( ZLIndex i = baseLine; i < totalLines; ++i ) {
		
		MOAITextLine& line = this->mLayout->mLines [ i ];
		const ZLRect& lineRect = line.mLayoutBounds;
		
		float lineWidth = lineRect.Width ();

		float adjustedLineXMin = xMin;
		float adjustedLineYMin = lineRect.mYMin + lineYOffset;
		
		switch ( this->mLayoutRules->mHAlign ) {
			
			case MOAITextLayoutRules::CENTER_JUSTIFY:
				adjustedLineXMin = xMin + ( width * 0.5 ) - ( lineWidth * 0.5 );
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

		adjustedLineX = this->Snap ( adjustedLineX, this->mLayoutRules->mHLineSnap ) + xOffsetToCenter;
		adjustedLineY = this->Snap ( adjustedLineY, this->mLayoutRules->mVLineSnap ) + yOffsetToCenter;
		
		float xOff = adjustedLineX - line.mOrigin.mX;
		float yOff = adjustedLineY - line.mOrigin.mY;
		
		line.Offset ( xOff, yOff );
		
		MOAIAnimCurveFloat* curve = curves ? curves [( i - baseLine ) % totalCurves ] : 0;
		
		for ( ZLIndex j = 0; j < line.mSize; ++j ) {
			
			MOAITextSprite& sprite = this->mLayout->mSprites [ line.mStart + j ];
			
			sprite.mPen.mX += xOff;
			sprite.mPen.mY += line.mOrigin.mY + ( curve ? curve->GetValue (( sprite.mPen.mX - xMin ) / width ) : 0.0 );
			
			//printf ( "SPRITE: %f %f\n", sprite.mPen.mX, sprite.mPen.mY );
			
			ZLRect glyphRect = sprite.mGlyph->GetGlyphRect ( sprite.mPen.mX, sprite.mPen.mY, sprite.mScale.mX, sprite.mScale.mY );
			glyphRect.Inflate ( sprite.mStyle->mPadding );
			line.mGlyphBounds.Grow ( glyphRect, j > 0 );
		}
		
		glyphFrame.Grow ( line.mGlyphBounds, i > 0 );
		layoutFrame.Grow ( line.mLayoutBounds, i > 0 );
	}
	
	// center texbox in model space to make autoflip easier to implement
	
	this->mLayout->mXOffset = -xOffsetToCenter;
	
	if ( limitWidth == false ) {
	
		switch ( this->mLayoutRules->mHAlign ) {
	
			case MOAITextLayoutRules::CENTER_JUSTIFY:
				this->mLayout->mXOffset = 0.0;
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
	
		switch ( this->mLayoutRules->mVAlign ) {
		
			case MOAITextLayoutRules::CENTER_JUSTIFY:
				this->mLayout->mYOffset = 0.0;
				break;
				
			case MOAITextLayoutRules::TOP_JUSTIFY:
				this->mLayout->mYOffset = yOffsetToCenter;
				break;

			case MOAITextLayoutRules::BOTTOM_JUSTIFY:
				this->mLayout->mYOffset = -yOffsetToCenter;
				break;
			
			case MOAITextLayoutRules::BASELINE_JUSTIFY: {
				ZLReal sign = this->mLayoutRules->mYFlip ? 1.0 : -1.0;
				ZLReal firstLineAscent = this->mLayout->mLines [ 0 ].GetAscent ();
				this->mLayout->mYOffset = ( yOffsetToCenter + firstLineAscent ) * sign;
				break;
			}
			
			default:
				break;
		}
	}

//	printf ( "OFFSET: %f %f\n", this->mLayout->mXOffset, this->mLayout->mYOffset );

	this->mLayout->mGlyphFrame		= glyphFrame;
	this->mLayout->mLayoutFrame		= layoutFrame;
	
//	printf ( "GLYPH BOUNDS: %f %f %f %f\n",
//		this->mLayout->mGlyphBounds.mXMin,
//		this->mLayout->mGlyphBounds.mYMin,
//		this->mLayout->mGlyphBounds.mXMax,
//		this->mLayout->mGlyphBounds.mYMax
//	);
//	
//	printf ( "LAYOUT BOUNDS: %f %f %f %f\n",
//		this->mLayout->mLayoutBounds.mXMin,
//		this->mLayout->mLayoutBounds.mYMin,
//		this->mLayout->mLayoutBounds.mXMax,
//		this->mLayout->mLayoutBounds.mYMax
//	);
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
	
	bool more = true;
	while ( more ) {
		
		this->CaptureRestorePoint ( RESTORE_POINT_LINE );
		
		u32 startingCharIdx = this->GetCharIndex ();
		
		MOAITextSimpleShaper shaper;
		u32 wrap = shaper.ShapeLine ( *this, *this->mLayoutRules );
		
		switch ( wrap ) {
			
			case MOAITextLayoutRules::OVERRUN_NONE:
				break;
			
			case MOAITextLayoutRules::OVERRUN_MOVE_WORD:
				
				this->Restore ( RESTORE_POINT_TOKEN );
				break;
			
			case MOAITextLayoutRules::OVERRUN_ABORT_LAYOUT:
			
				more = false;
				this->Restore ( RESTORE_POINT_TOKEN );
				break;
			
			case MOAITextLayoutRules::OVERRUN_TRUNCATE_WORD:
			case MOAITextLayoutRules::OVERRUN_SPLIT_WORD:
			
				this->Restore ( RESTORE_POINT_CHAR );
				break;
		}
		
		this->mOverrun = wrap != MOAITextLayoutRules::OVERRUN_NONE;
		
		if ( this->GetCharIndex () > startingCharIdx ) {
			
			u32 result = this->PushLine ();
			
			if ( result != PUSH_OK ) {
				this->Restore ( RESTORE_POINT_LINE );
				more = false;
			}
		}
		else {
			more = false;
		}
	}

	this->Align ();
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::BuildLayout ( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, MOAITextLayoutRules& layoutRules, cc8* str, u32 idx ) {
	
	if ( styleMap.CountSpans () == 0 ) return;
	
	this->mLayout		= &layout;
	this->mStyleCache	= &styleCache;
	this->mStyleMap		= &styleMap;
	this->mLayoutRules	= &layoutRules;
	
	this->mStr			= str;
	this->mCharIdx		= idx;
	this->mSpriteIdx	= this->mLayout->mSprites.GetTop ();
	
	this->mStyleSpan	= 0;
	this->mSpanIdx		= 0;
	
	this->mLayoutFrameWithMargins = layoutRules.GetFrameWithMargins ();
	
	this->mLayoutBounds = ZLRect::EMPTY;
	this->mLineLayoutBounds = ZLRect::EMPTY;
	
	this->mLineSpacingBounds = ZLRect::EMPTY;
	this->mLineSpacingCursor = 0.0;
	
	this->mBaseLine = layout.mLines.GetTop ();
	
	memset ( &this->mCurrentChar, 0, sizeof ( MOAITextStyledChar ));
	this->mCurrentChar.mScale.Init ( 1.0, 1.0 );
	
	this->mCurrentGlyphDeck = 0;
	
	this->mResetStyle = true;
	this->mOverrun = false;
	
	this->BuildLayout ();
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::CaptureRestorePoint ( u32 restorePointID ) {

	this->mRestorePoints [ restorePointID ] = *( MOAILayoutEngineState* )this;
}

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::GetCharIndex () {

	return this->mCharIdx;
}

//----------------------------------------------------------------//
ZLSize MOAITextLayoutEngine::GetLineSizeInSprites () {

	return this->mLayout->mSprites.GetTop () - this->GetLineSpriteIdx ();
}

//----------------------------------------------------------------//
ZLIndex MOAITextLayoutEngine::GetLineSpriteIdx () {

	return this->mRestorePoints [ RESTORE_POINT_LINE ].mSpriteIdx;
}

//----------------------------------------------------------------//
ZLIndex MOAITextLayoutEngine::GetSpriteIndex () {

	return this->mLayout->mSprites.GetTop ();
}

//----------------------------------------------------------------//
MOAITextLayoutEngine::MOAITextLayoutEngine () :
	mStr ( 0 ),
	mLineSpacingCursor ( 0.0f ),
	mEmptyLineAscent ( 0.0f ),
	mEmptyLineDescent ( 0.0f ),
	mBaseLine ( 0 ),
	mCurrentGlyphDeck ( 0 ),
	mResetStyle ( false ),
	mOverrun ( false ),
	mLayoutRules ( 0 ),
	mLayout ( 0 ),
	mStyleCache ( 0 ),
	mStyleMap ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextLayoutEngine::~MOAITextLayoutEngine () {
}

//----------------------------------------------------------------//
bool MOAITextLayoutEngine::More () {

	return ( this->mStr && ( this->mStr [ this->mCharIdx ] != NULL ));
}

//----------------------------------------------------------------//
MOAITextStyledChar MOAITextLayoutEngine::NextChar () {

	this->mCurrentChar.mChar = 0; // set this here so shaper will abort if we return due to an error.

	if ( !this->mStyleSpan ) {
		this->mStyleSpan = &this->mStyleMap->Elem ( 0 );
		this->mSpanIdx = 0;
		this->mResetStyle = true;
	}

	if (( int )this->mCharIdx >= this->mStyleSpan->mTop ) {
		
		this->mStyleSpan = 0;
		
		ZLSize totalStyles = ( u32 )this->mStyleMap->GetTop ();
		for ( this->mSpanIdx++; this->mSpanIdx < totalStyles; this->mSpanIdx++ ) {
			MOAITextStyleSpan& styleSpan = this->mStyleMap->Elem ( this->mSpanIdx );
			
			if (( int )this->mCharIdx < styleSpan.mTop ) {
				this->mStyleSpan = &styleSpan;
				this->mResetStyle = true;
				break;
			}
		}
	}
	
	if ( this->mStyleSpan ) {
	
		if ( this->mResetStyle ) {
		
			MOAITextStyleState* defaultStyle = this->mStyleCache->GetStyle ();
			MOAIFont* defaultFont = defaultStyle ? defaultStyle->mFont : 0;
		
			if (( int )this->mCharIdx < this->mStyleSpan->mBase ) {
				this->mCharIdx = this->mStyleSpan->mBase;
			}
			
			MOAITextStyleState* style = 0;
			
			style = this->mStyleSpan->mStyle;
			style = style ? style : defaultStyle;
			if ( !style ) return this->mCurrentChar; // TODO: report error
			
			MOAIFont* font = style->mFont;
			font = font ? font : defaultFont;
			if ( !font ) return this->mCurrentChar; // TODO: report error
			
			this->mCurrentGlyphDeck = font->GetGlyphSet ( style->mSize );
			if ( !this->mCurrentGlyphDeck && defaultFont ) {
				this->mCurrentGlyphDeck = defaultFont->GetGlyphSet ( style->mSize );
			}
			
			if ( !this->mCurrentGlyphDeck ) return this->mCurrentChar; // TODO: report error
			
			float deckScale = this->mCurrentGlyphDeck && ( style->mSize > 0.0f ) ? style->mSize / this->mCurrentGlyphDeck->GetSize () : 1.0f;
			
			this->mCurrentChar.mStyle = style;
			this->mCurrentChar.mScale.mX = this->mLayoutRules->mGlyphScale * style->mScale.mX * deckScale;
			this->mCurrentChar.mScale.mY = this->mLayoutRules->mGlyphScale * style->mScale.mY * deckScale;
			
			this->mResetStyle = false;
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

	return this->mOverrun;
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

	u32 totalLines = ( u32 )this->mLayout->mLines.GetTop ();

	float yPen = 0.0f;

	// this works because all the rectangles are first drawn with their baselines at the origin
	if ( totalLines > 0 ) {
		yPen = ( this->mLineSpacingCursor - this->mLineSpacingBounds.mYMin ) + this->mLayoutRules->mLineSpacing;
		this->mLineLayoutBounds.Offset ( 0.0f, yPen );
		this->mLineSpacingBounds.Offset ( 0.0f, yPen );
	}
	this->mLineSpacingCursor = this->mLineSpacingBounds.mYMax;
	
	ZLRect newLayoutBounds = this->mLayoutBounds;
	newLayoutBounds.Grow ( this->mLineLayoutBounds, ( totalLines > 0 ));
	
	if ( this->mLayoutRules->mLimitHeight ) {
	
		float frameHeight = this->mLayoutFrameWithMargins.Height ();
		if ( newLayoutBounds.Height () > frameHeight ) return PUSH_OVERRUN;
	}
	
	this->mLayout->PushLine ( this->GetLineSpriteIdx (), this->GetLineSizeInSprites (), ZLVec2D ( 0.0f, yPen ), this->mLineLayoutBounds );
	this->mLayoutBounds = newLayoutBounds;
	
	return PUSH_OK;
}

//----------------------------------------------------------------//
u32 MOAITextLayoutEngine::PushSprite ( const MOAITextStyledChar& styledChar, float x, float y ) {
	
	float xScale = styledChar.mScale.mX;
	float yScale = styledChar.mScale.mY;
	
	MOAIGlyph* glyph = styledChar.mGlyph;
	
	ZLRect glyphLayoutRect = this->mLayoutRules->GetGlyphLayoutRect ( *glyph, x, y, xScale, yScale );
	ZLRect glyphSpacingRect = this->mLayoutRules->GetGlyphSpacingRect ( *glyph, x, y, xScale, yScale );
	
	if ( this->mLayoutRules->mLimitWidth ) {
		float frameWidth = this->mLayoutFrameWithMargins.Width ();
		if ( glyphLayoutRect.mXMax > frameWidth ) return PUSH_OVERRUN;
	}
	
	u32 lineSizeInSprites = this->GetLineSizeInSprites ();
	
	this->mLineLayoutBounds.Grow ( glyphLayoutRect, ( lineSizeInSprites > 0 ));
	this->mLineSpacingBounds.Grow ( glyphSpacingRect, ( lineSizeInSprites > 0 ));
	
	this->mLayout->PushSprite ( styledChar, x, y );
	
	this->mSpriteIdx = this->mLayout->mSprites.GetTop ();
	
	return PUSH_OK;
}

//----------------------------------------------------------------//
void MOAITextLayoutEngine::Restore ( u32 restorePointID ) {

	*( MOAILayoutEngineState* )this = this->mRestorePoints [ restorePointID ];
	
	this->mLayout->mSprites.SetTop ( this->mSpriteIdx );
	
	this->mResetStyle = true;
}

//----------------------------------------------------------------//
float MOAITextLayoutEngine::Snap ( float f, float b ) {

	return ( b > 0.0f ) ? floorf (( f / b ) + 0.5f ) * b : f;
}
