// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleMap.h>
#include <moai-sim/MOAITextStyleParser.h>

#define DEFAULT_STYLE_NAME ""

//================================================================//
// MOAITextStyleMap
//================================================================//

//----------------------------------------------------------------//
void MOAITextStyleMap::BuildStyleMap ( MOAITextStyleCache& styleCache, cc8* str ) {

	this->Reset ();

	MOAITextStyleParser parser;
	parser.BuildStyleMap ( *this, styleCache, str );
}

//----------------------------------------------------------------//
u32 MOAITextStyleMap::CountSpans () {

	return this->GetTop ();
}

//----------------------------------------------------------------//
MOAITextStyleMap::MOAITextStyleMap () {
}

//----------------------------------------------------------------//
MOAITextStyleMap::~MOAITextStyleMap () {
}

//----------------------------------------------------------------//
void MOAITextStyleMap::PushStyleSpan ( int base, int top, MOAITextStyle& style ) {

	MOAITextStyleSpan span;
	
	span.mBase		= base;
	span.mTop		= top;
	span.mStyle		= &style;

	this->Push ( span );
}

//----------------------------------------------------------------//
void MOAITextStyleMap::RefreshStyleGlyphs ( cc8* str ) {

	u32 totalSpans = this->GetTop ();
	if ( !totalSpans ) return;
	
	for ( u32 i = 0; i < totalSpans; ++i ) {
		MOAITextStyleSpan& span = this->Elem ( i );
		
		int idx = span.mBase;
		while ( idx < span.mTop ) {
			u32 c = moai_u8_nextchar ( str, &idx );
			span.mStyle->AffirmGlyph ( c );
		}
	}
	
	// TODO: think about keeping list of currently active styles instead of iterating through everything
	
	for ( u32 i = 0; i < totalSpans; ++i ) {
		MOAITextStyleSpan& span = this->Elem ( i );
		span.mStyle->mFont->ProcessGlyphs ();
	}
}
