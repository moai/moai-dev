// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIShape.h>

//================================================================//
// MOAIShape
//================================================================//

//----------------------------------------------------------------//
MOAIShape::MOAIShape () :
	mFillStyle ( FILL_SOLID ),
	mLineStyle ( LINE_NONE ) {
	
	this->mFillColor.SetWhite ();
	this->mLineColor.SetWhite ();
}

//----------------------------------------------------------------//
MOAIShape::~MOAIShape () {
}
