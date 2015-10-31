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
#include <moai-sim/MOAITextStyleMap.h>

//================================================================//
// MOAITextDesigner
//================================================================//

//----------------------------------------------------------------//
void MOAITextDesigner::ClearCurves () {

	for ( u32 i = 0; i < this->mCurves.Size (); ++i ) {
		this->ReleaseCurve ( this->mCurves [ i ]);
	}
	this->mCurves.Clear ();
}

//----------------------------------------------------------------//
ZLRect MOAITextDesigner::GetGlyphRect ( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale, u32 hRule, u32 vRule ) {
	UNUSED ( hRule );
	
	const MOAIGlyphSet& glyphSet = glyph.GetDeck ();

	x += ( glyph.mBearingX * xScale );
	y -= ( glyph.mBearingY * yScale );

	ZLRect rect;

	rect.mXMin = x;
	rect.mXMax = x + ( glyph.mWidth * xScale );

	switch ( vRule ) {
	
		case GLYPH_SIZE:
			
			rect.mYMin = y;
			rect.mYMax = y + ( glyph.mHeight * yScale );
			break;
		
		case LOGICAL_SIZE:
		
			rect.mYMin = y - ( glyphSet.GetDescent () * yScale ),
			rect.mYMax = y + ( glyphSet.GetAscent () * yScale );
			break;
		
		case MAXIMUM_SIZE:
		case VISIBLE_SIZE:
		case VISIBLE_OR_LOGICAL_SIZE:
			assert ( 0 ); // TODO: unsupported
		break;
	}

	return rect;
}

//----------------------------------------------------------------//
ZLRect MOAITextDesigner::GetGlyphLayoutRect ( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale ) {

	return MOAITextDesigner::GetGlyphRect ( glyph, x, y, xScale, yScale, this->mHLayoutSizingRule, this->mVLayoutSizingRule );
}

//----------------------------------------------------------------//
ZLRect MOAITextDesigner::GetGlyphSpacingRect ( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale ) {

	return MOAITextDesigner::GetGlyphRect ( glyph, x, y, xScale, yScale, this->mLineSizingRule, this->mLineSizingRule );
}

//----------------------------------------------------------------//
void MOAITextDesigner::Layout ( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, cc8* str, u32 idx, bool* more, u32* nextIdx, bool* overrun ) {

	MOAITextDesignParser parser;
	
	parser.BuildLayout ( layout, styleCache, styleMap, *this, str, idx );
	layout.ApplyHighlights ();
	
	if ( more ) {
		*more = parser.More ();
	}
	
	if ( nextIdx ) {
		*nextIdx = parser.GetIndex ();
	}

	if ( overrun ) {
		*overrun = parser.Overrun ();
	}
}

//----------------------------------------------------------------//
void MOAITextDesigner::Init ( const MOAITextDesigner& designer ) {

	this->ClearCurves ();

	this->mOwner			= designer.mOwner;
	this->mFrame			= designer.mFrame;
	this->mLimitWidth		= designer.mLimitWidth;
	this->mLimitHeight		= designer.mLimitHeight;
	this->mHAlign			= designer.mHAlign;
	this->mVAlign			= designer.mVAlign;
	this->mYFlip			= designer.mYFlip;
	this->mWordBreakRule	= designer.mWordBreakRule;
	this->mGlyphScale		= designer.mGlyphScale;
	this->mLineSpacing		= designer.mLineSpacing;
	
	u32 totalCurves = designer.mCurves.Size ();
	this->ReserveCurves ( totalCurves );
	for ( u32 i = 0; i < totalCurves; ++i ) {
		this->SetCurve ( i, designer.mCurves [ i ]);
	}
}

//----------------------------------------------------------------//
MOAITextDesigner::MOAITextDesigner () :
	mOwner ( 0 ),
	mLimitWidth ( false ),
	mLimitHeight ( false ),
	mHAlign ( MOAITextDesigner::LEFT_JUSTIFY ),
	mVAlign ( MOAITextDesigner::TOP_JUSTIFY ),
	mYFlip ( false ),
	mWordBreakRule ( MOAITextDesigner::WORD_BREAK_NONE ),
	mHLayoutSizingRule ( LOGICAL_SIZE ),
	mVLayoutSizingRule ( LOGICAL_SIZE ),
	mLineSizingRule ( LOGICAL_SIZE ),
	mGlyphScale ( 1.0f ),
	mLineSpacing ( 0.0f ),
	mHLineSnap ( 0.0f ),
	mVLineSnap ( 0.0f ) {
	
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAITextDesigner::~MOAITextDesigner () {

	this->ClearCurves ();
}

//----------------------------------------------------------------//
void MOAITextDesigner::ReleaseCurve ( MOAIAnimCurve* curve  ) {

	if ( curve ) {
		if ( this->mOwner ) {
			this->mOwner->LuaRelease ( curve );
		}
		curve->Release ();
	}
}

//----------------------------------------------------------------//
void MOAITextDesigner::ReserveCurves ( u32 total ) {

	this->ClearCurves ();
	
	this->mCurves.Init ( total );
	this->mCurves.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAITextDesigner::RetainCurve ( MOAIAnimCurve* curve  ) {

	if ( curve ) {
		curve->Retain ();
		if ( this->mOwner ) {
			this->mOwner->LuaRetain ( curve );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextDesigner::SetCurve ( u32 idx, MOAIAnimCurve* curve ) {

	if ( idx > this->mCurves.Size ()) return;
	if ( this->mCurves [ idx ] == curve ) return;

	this->RetainCurve ( curve );
	this->ReleaseCurve ( this->mCurves [ idx ]);

	this->mCurves [ idx ] = curve;
}

