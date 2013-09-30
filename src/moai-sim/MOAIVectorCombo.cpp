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
MOAIVectorCombo::MOAIVectorCombo () {
}

//----------------------------------------------------------------//
MOAIVectorCombo::~MOAIVectorCombo () {
}

//----------------------------------------------------------------//
void MOAIVectorCombo::Reserve ( u32 total ) {

	this->mShapes.Init ( total );
}

//----------------------------------------------------------------//
void MOAIVectorCombo::SetShape ( u32 idx, MOAIVectorShape* shape ) {

	this->mShapes [ idx ] = shape;
}

//----------------------------------------------------------------//
void MOAIVectorCombo::ToOutline ( TESStesselator* tess ) {

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		this->mShapes [ i ]->ToOutline ( tess );
	}
}
