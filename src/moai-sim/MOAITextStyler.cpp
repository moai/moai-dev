// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyler.h>
#include <moai-sim/MOAITextStyleParser.h>

#define DEFAULT_STYLE_NAME ""

//================================================================//
// MOAITextStyleRef
//================================================================//

//----------------------------------------------------------------//
MOAITextStyleRef::MOAITextStyleRef () :
	mStyle ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextStyleRef::~MOAITextStyleRef () {
}

//----------------------------------------------------------------//
bool MOAITextStyleRef::NeedsLayout () const {

	if ( this->mStyle ) {
		return this->mState.NeedsLayout ( *this->mStyle );
	}
	return false;
}

//----------------------------------------------------------------//
void MOAITextStyleRef::UpdateState () {

	if ( this->mStyle ) {
		this->mState = *this->mStyle;
	}
}

//================================================================//
// MOAITextStyler
//================================================================//

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyler::AddAnonymousStyle ( MOAITextStyle* source ) {

	MOAITextStyle* style = new MOAITextStyle ();
	style->Init ( *source );
	
	MOAITextStyleRef styleRef;
	styleRef.mStyle = style;
	styleRef.UpdateState ();
	
	this->RetainStyle ( style );
	this->mAnonymousStyles.Push ( styleRef );
	
	return style;
}

//----------------------------------------------------------------//
void MOAITextStyler::BuildStyleMap ( cc8* str ) {

	if ( !this->mStyleMap.GetTop ()) {
		MOAITextStyleParser parser;
		parser.BuildStyleMap ( *this, str );
	}
}

//----------------------------------------------------------------//
bool MOAITextStyler::CheckStylesChanged () {

	bool status = false;

	// TODO: think about keeping list of currently active styles instead of iterating through everything
	
	u32 totalAnonymous = this->mAnonymousStyles.GetTop ();
	for ( u32 i = 0; i < totalAnonymous; i++ ) {
		MOAITextStyleRef& styleRef = this->mAnonymousStyles [ i ];
		if ( styleRef.NeedsLayout ()) {
			styleRef.UpdateState ();
			status = true;
		}
	}

	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		MOAITextStyleRef& styleRef = styleSetIt->second;
		if ( styleRef.NeedsLayout ()) {
			styleRef.UpdateState ();
			status = true;
		}
	}

	return status;
}

//----------------------------------------------------------------//
u32 MOAITextStyler::CountSpans () {

	return this->mStyleMap.GetTop ();
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyler::GetStyle () {

	return this->GetStyle ( DEFAULT_STYLE_NAME );
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyler::GetStyle ( cc8* styleName ) {

	if ( styleName && this->mStyleSet.contains ( styleName )) {
		return this->mStyleSet [ styleName ].mStyle;
	}
	return this->mStyleSet [ DEFAULT_STYLE_NAME ].mStyle;
}

//----------------------------------------------------------------//
MOAITextStyler::MOAITextStyler () :
	mOwner ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextStyler::~MOAITextStyler () {

	this->ResetStyleMap ();
	this->ResetStyleSet ();
}

//----------------------------------------------------------------//
void MOAITextStyler::PushStyleSpan ( int base, int top, MOAITextStyle& style ) {

	MOAITextStyleSpan span;
	
	span.mBase		= base;
	span.mTop		= top;
	span.mStyle		= &style;

	this->mStyleMap.Push ( span );
}

//----------------------------------------------------------------//
void MOAITextStyler::RefreshStyleGlyphs ( cc8* str ) {

	u32 totalSpans = this->mStyleMap.GetTop ();
	if ( !totalSpans ) return;
	
	for ( u32 i = 0; i < totalSpans; ++i ) {
		MOAITextStyleSpan& span = this->mStyleMap [ i ];
		
		int idx = span.mBase;
		while ( idx < span.mTop ) {
			u32 c = u8_nextchar ( str, &idx );
			span.mStyle->AffirmGlyph ( c );
		}
	}
	
	// TODO: think about keeping list of currently active styles instead of iterating through everything
	
	u32 totalAnonymous = this->mAnonymousStyles.GetTop ();
	for ( u32 i = 0; i < totalAnonymous; i++ ) {
		MOAITextStyleRef& styleRef = this->mAnonymousStyles [ i ];
		styleRef.mStyle->mFont->ProcessGlyphs ();
	}

	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		MOAITextStyleRef& styleRef = styleSetIt->second;
		styleRef.mStyle->mFont->ProcessGlyphs ();
	}
}

//----------------------------------------------------------------//
void MOAITextStyler::ReleaseStyle ( MOAITextStyle* style ) {

	if ( style ) {
		if ( this->mOwner ) {
			this->mOwner->ClearNodeLink ( *style );
			this->mOwner->LuaRelease ( style );
		}
		style->Release ();
	}
}

//----------------------------------------------------------------//
void MOAITextStyler::ResetStyleMap () {

	u32 totalAnonymous = this->mAnonymousStyles.GetTop ();
	for ( u32 i = 0; i < totalAnonymous; i++ ) {
		this->ReleaseStyle ( this->mAnonymousStyles [ i ].mStyle );
	}
	this->mAnonymousStyles.Reset ();
	this->mStyleMap.Reset ();
}

//----------------------------------------------------------------//
void MOAITextStyler::ResetStyleSet () {
	
	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		this->ReleaseStyle ( styleSetIt->second.mStyle );
	}
	this->mStyleSet.clear ();
}

//----------------------------------------------------------------//
void MOAITextStyler::RetainStyle ( MOAITextStyle* style ) {

	if ( style ) {
	
		style->Retain ();

		if ( this->mOwner ) {
			this->mOwner->SetNodeLink ( *style );
			this->mOwner->LuaRetain ( style );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextStyler::SetStyle ( MOAITextStyle* style ) {

	this->SetStyle ( DEFAULT_STYLE_NAME, style );
}

//----------------------------------------------------------------//
void MOAITextStyler::SetStyle ( cc8* styleName, MOAITextStyle* style ) {

	styleName = styleName ? styleName : DEFAULT_STYLE_NAME;
		
	MOAITextStyle* prevStyle = 0;
	
	if ( this->mStyleSet.contains ( styleName )) {
		prevStyle = this->mStyleSet [ styleName ].mStyle;
		if ( prevStyle == style ) return;
	}
	
	this->RetainStyle ( style );
	this->ReleaseStyle ( prevStyle );
	
	if ( style ) {
		MOAITextStyleRef& styleRef = this->mStyleSet [ styleName ];
		styleRef.mStyle = style;
		styleRef.UpdateState ();
	}
	else {
		if ( this->mStyleSet.contains ( styleName )) {
			this->mStyleSet.erase ( styleName );
		}
	}
	
	if ( prevStyle ) {
		this->ResetStyleMap ();
	}
}
