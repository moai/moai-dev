// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAITextLayout.h>
#include <moaicore/MOAITextStyle.h>

//================================================================//
// MOAITextLayout
//================================================================//

//----------------------------------------------------------------//
u32 MOAITextLayout::AffirmStyle ( MOAITextStyle& style ) {

	u32 top = this->mStyles.GetTop ();
	for ( u32 i = 0; i < top; ++i ) {
		if ( this->mStyles [ i ] == &style) {
			return i;
		}
	}
	this->mStyles.Push ( &style );
	return top;
}

//----------------------------------------------------------------//
void MOAITextLayout::LoadGlyphMetrics () {

	u32 top = this->mStyles.GetTop ();
	for ( u32 i = 0; i < top; ++i ) {
	
		MOAITextStyle* style = this->mStyles [ i ];
		MOAIFont* font = style->GetFont ();
		font->LoadGlyphMetrics ();
	}
}

//----------------------------------------------------------------//
MOAITextLayout::MOAITextLayout () {
}

//----------------------------------------------------------------//
MOAITextLayout::~MOAITextLayout () {
}

//----------------------------------------------------------------//
void MOAITextLayout::PushToken ( cc8* str, int base, int size, u32 flags, MOAITextStyle& style ) {

	MOAITextToken token;
	
	token.mBase			= base;
	token.mStyle		= ( u16 )this->AffirmStyle ( style );
	token.mFlags		= ( u16 )flags;

	float points = style.GetPoints ();
	MOAIFont* font = style.GetFont ();
	assert ( font );

	font->AffirmGlyphs ( points, &str [ base ], size );

	this->Push ( token );
	
	printf ( "found token: \'" );
		
	int idx = base;
	for ( int i = 0; i < size; ++i ) {
		u32 c = u8_nextchar ( str, &idx );
		putc ( c, stdout );
	}
	printf ( "\'\n" );
}

//----------------------------------------------------------------//
void MOAITextLayout::SizeTokens () {
}
