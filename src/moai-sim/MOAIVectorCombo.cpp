// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorCombo.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorCombo
//================================================================//

//----------------------------------------------------------------//
bool MOAIVectorCombo::GroupShapes ( MOAIVectorShape** shapes, u32 total ) {
	
	this->mShapes.Init ( total );
	memcpy ( this->mShapes.Data (), shapes, sizeof ( MOAIVectorShape** ) * total );
	return true;
}

//----------------------------------------------------------------//
MOAIVectorCombo::MOAIVectorCombo () {
}

//----------------------------------------------------------------//
MOAIVectorCombo::~MOAIVectorCombo () {
}

//----------------------------------------------------------------//
void MOAIVectorCombo::ToOutline ( TESStesselator* tess ) {

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		this->mShapes [ i ]->ToOutline ( tess );
	}
}
