// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorCombo.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorCombo
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorCombo::AddFillContours ( TESStesselator* tess ) {

	TESStesselator* outline = tessNewTess ( 0 );
	assert ( outline );

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		MOAIVectorShape& shape = *this->mShapes [ i ];
		//if ( shape.IsClosed ()) {
			shape.AddFillContours ( outline );
		//}
	}
	
	tessTesselate ( outline, ( int )this->mStyle.GetWindingRule (), TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( tess, outline );
	
	tessDeleteTess ( outline );
}


//----------------------------------------------------------------//
bool MOAIVectorCombo::GroupShapes ( MOAIVectorShape** shapes, u32 total ) {
	
	this->mShapes.Init ( total );
	memcpy ( this->mShapes.Data (), shapes, sizeof ( MOAIVectorShape** ) * total );
	this->mCanGroup = false;
	return true;
}

//----------------------------------------------------------------//
bool MOAIVectorCombo::IsClosed () {

	return true;
}

//----------------------------------------------------------------//
MOAIVectorCombo::MOAIVectorCombo () {

	this->mCanGroup = true;
}

//----------------------------------------------------------------//
MOAIVectorCombo::~MOAIVectorCombo () {
}


