// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleCache.h>
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
// MOAITextStyleCache
//================================================================//

//----------------------------------------------------------------//
MOAITextStyleState* MOAITextStyleCache::AddAnonymousStyle ( MOAITextStyleState* source ) {

	// TODO: replace with a pool
	MOAITextStyleState* style = new MOAITextStyleState ();
	style->Init ( *source );
	
	this->mAnonymousStyles.Push ( style );
	
	return style;
}

//----------------------------------------------------------------//
bool MOAITextStyleCache::CheckStylesChanged () {

	// no need to check anonymous styles; they cannot change
	
	// always trust people who like big butts; they cannot lie

	// check named/public styles
	
	bool status = false;
	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		MOAITextStyleRef& styleRef = styleSetIt->second;
		if ( styleRef.NeedsLayout ()) {
			styleRef.UpdateState ();
			status = true;
		}
	}
	
	// TODO: styles can change if styles are cleared; handle that case
	
	return status;
}

//----------------------------------------------------------------//
void MOAITextStyleCache::Clear () {

	this->ClearAnonymousStyles ();
	this->ClearNamedStyles ();
}

//----------------------------------------------------------------//
void MOAITextStyleCache::ClearAnonymousStyles () {

	size_t totalAnonymous = this->mAnonymousStyles.GetTop ();
	for ( size_t i = 0; i < totalAnonymous; i++ ) {
	
		// TODO: replace with a pool
		delete this->mAnonymousStyles [ i ];
	}
	this->mAnonymousStyles.Reset ();
}

//----------------------------------------------------------------//
void MOAITextStyleCache::ClearNamedStyles () {
	
	this->ClearAnonymousStyles ();
	
	StyleSetIt styleSetIt = this->mStyleSet.begin ();
	for ( ; styleSetIt != this->mStyleSet.end (); ++styleSetIt ) {
		this->ReleaseStyle ( styleSetIt->second.mStyle );
	}
	this->mStyleSet.clear ();
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyleCache::GetStyle () {

	return this->GetStyle ( DEFAULT_STYLE_NAME );
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyleCache::GetStyle ( cc8* styleName ) {

	if ( styleName && this->mStyleSet.contains ( styleName )) {
		return this->mStyleSet [ styleName ].mStyle;
	}
	return this->mStyleSet [ DEFAULT_STYLE_NAME ].mStyle;
}

//----------------------------------------------------------------//
MOAITextStyleCache::MOAITextStyleCache () :
	mOwner ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextStyleCache::~MOAITextStyleCache () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAITextStyleCache::ReleaseStyle ( MOAITextStyle* style ) {

	if ( style ) {
		if ( this->mOwner ) {
			this->mOwner->ClearNodeLink ( *style );
			this->mOwner->LuaRelease ( style );
		}
		style->Release ();
	}
}

//----------------------------------------------------------------//
void MOAITextStyleCache::RetainStyle ( MOAITextStyle* style ) {

	if ( style ) {
	
		style->Retain ();

		if ( this->mOwner ) {
			this->mOwner->SetNodeLink ( *style );
			this->mOwner->LuaRetain ( style );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextStyleCache::SetStyle ( MOAITextStyle* style ) {

	this->SetStyle ( DEFAULT_STYLE_NAME, style );
}

//----------------------------------------------------------------//
void MOAITextStyleCache::SetStyle ( cc8* styleName, MOAITextStyle* style ) {

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
}
