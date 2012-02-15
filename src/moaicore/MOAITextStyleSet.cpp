// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITextStyle.h>
#include <moaicore/MOAITextStyleSet.h>

//================================================================//
// MOAITextStyleSet
//================================================================//

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyleSet::FindStyle ( cc8* styleName ) {

	if ( this->mStyleMap.contains ( styleName )) {
		return this->mStyleMap [ styleName ];
	}
	return this->mDefault;
}

//----------------------------------------------------------------//
MOAITextStyle* MOAITextStyleSet::GetDefault () {

	return this->mDefault;
}

//----------------------------------------------------------------//
MOAITextStyleSet::MOAITextStyleSet () :
	mDefault ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextStyleSet::~MOAITextStyleSet () {
}

//----------------------------------------------------------------//
void MOAITextStyleSet::SetDefault ( MOAITextStyle* style ) {

	this->mDefault = style;
}

//----------------------------------------------------------------//
void MOAITextStyleSet::SetStyle ( cc8* styleName, MOAITextStyle* style ) {

	if ( !styleName ) return;
	
	if ( style ) {
		this->mStyleMap [ styleName ] = style;
	}
	else {
		if ( this->mStyleMap.contains ( styleName )) {
			this->mStyleMap.erase ( styleName );
		}
	}
}
