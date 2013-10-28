// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextDesigner.h>
#include <moai-sim/MOAITextDesignParser.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyler.h>

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
void MOAITextDesigner::Layout ( MOAITextLayout& layout, MOAITextStyler& styler, cc8* str, u32 idx, bool* more, u32* nextIdx ) {

	layout.Reset ();

	MOAITextDesignParser parser;
	
	parser.BuildLayout ( layout, styler, *this, str, idx );
	layout.ApplyHighlights ();
	
	if ( more ) {
		*more = parser.More ();
	}
	
	if ( nextIdx ) {
		*nextIdx = parser.GetIndex ();
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
	mWordBreak ( MOAITextDesigner::WORD_BREAK_NONE ),
	mGlyphScale ( 1.0f ),
	mLineSpacing ( 0.0f ) {
	
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

