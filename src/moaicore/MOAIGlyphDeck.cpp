// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGlyphDeck.h>

//================================================================//
// MOAIGlyphDeck
//================================================================//

//----------------------------------------------------------------//
void MOAIGlyphDeck::AffirmGlyph ( u32 c ) {

	if ( !this->mGlyphMap.contains ( c )) {
	
		MOAIGlyph& glyph = this->mGlyphMap [ c ];
		glyph.mNext = this->mPending;
		this->mPending = &glyph;
		glyph.SetCode ( c );
	}
}

//----------------------------------------------------------------//
MOAIGlyph* MOAIGlyphDeck::GetGlyph ( u32 c ) {

	return &this->mGlyphMap [ c ];
}

//----------------------------------------------------------------//
MOAIGlyphDeck::MOAIGlyphDeck () :
	mPoints ( 0.0f ),
	mHeight ( 0.0f ),
	mAscent ( 0.0f ),
	mPending ( 0 ) {
}

//----------------------------------------------------------------//
MOAIGlyphDeck::~MOAIGlyphDeck (){
}
