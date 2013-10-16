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
void MOAIVectorCombo::AddFillContours ( MOAIVectorDrawing& drawing, TESStesselator* tess ) {

	TESStesselator* outline = tessNewTess ( 0 );
	assert ( outline );

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		this->mShapes [ i ]->AddFillContours ( drawing, outline );
	}
	
	tessTesselate ( outline, ( int )this->mStyle.GetWindingRule (), TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( tess, outline );
	
	tessDeleteTess ( outline );
}

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


