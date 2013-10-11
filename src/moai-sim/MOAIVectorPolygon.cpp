// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorPolygon.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorPolygon
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorPolygon::AddFillContours ( TESStesselator* tess ) {

	TESStesselator* outline = tessNewTess ( 0 );
	assert ( outline );

	tessAddContour ( outline, 2, this->mVertices.Data (), sizeof ( ZLVec2D ), this->mVertices.Size ());
	
	tessTesselate ( outline, ( int )this->mStyle.GetWindingRule (), TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( tess, outline );
	
	tessDeleteTess ( outline );
}

//----------------------------------------------------------------//
void MOAIVectorPolygon::AddStrokeContours ( TESStesselator* tess ) {

	MOAIVectorShape::AddStrokeContours ( tess );
}

//----------------------------------------------------------------//
MOAIVectorPolygon::MOAIVectorPolygon () {
}

//----------------------------------------------------------------//
MOAIVectorPolygon::~MOAIVectorPolygon () {
}
