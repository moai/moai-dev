// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorPath.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorPath
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorPath::AddFillContours ( MOAIVectorDrawing& drawing, TESStesselator* tess ) {
	UNUSED ( drawing );
	UNUSED ( tess );
}

//----------------------------------------------------------------//
void MOAIVectorPath::AddStrokeContours ( MOAIVectorDrawing& drawing, TESStesselator* tess ) {

	ZLVec2D* verts = this->mVertices;
	int nJoins = ( int )this->mVertices.Size ();

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nJoins * 2 );
	
	MOAIVectorLineJoin* joins0 = joins;
	MOAIVectorLineJoin* joins1 = &joins [ nJoins ];
	
	drawing.ComputeLineJoins ( joins, verts, nJoins, true, true, false );
	drawing.ComputeLineJoins ( &joins [ nJoins ], verts, nJoins, true, false, false );
	
	float width = this->mStyle.GetStrokeWidth () * 0.5f;
	
	int nVerts0 = drawing.StrokeLine ( 0, joins0, nJoins, width, false );
	int nVerts1 = drawing.StrokeLine ( 0, joins1, nJoins, width, false );
	int nVerts = nVerts0 + nVerts1;
	
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * nVerts );
	
	ZLVec2D* contour0 = contour;
	ZLVec2D* contour1 = &contour [ nVerts0 ];
	
	drawing.StrokeLine ( contour0, joins0, nJoins, width, false );
	drawing.StrokeLine ( contour1, joins1, nJoins, width, false );
	
	tessAddContour ( tess, 2, contour, sizeof ( ZLVec2D ), nVerts );
}

//----------------------------------------------------------------//
bool MOAIVectorPath::GroupVertices ( MOAIVectorDrawing& drawing, u32 total ) {
	
	if ( total ) {
		this->mVertices.Init ( total );
		drawing.CopyVertexStack ( this->mVertices, total );
	}
	return true;
}

//----------------------------------------------------------------//
MOAIVectorPath::MOAIVectorPath () {
}

//----------------------------------------------------------------//
MOAIVectorPath::~MOAIVectorPath () {
}

//----------------------------------------------------------------//
void MOAIVectorPath::SetVertices ( ZLVec2D* vertices, u32 total ) {

	if ( total ) {
		this->mVertices.Init ( total );
		memcpy ( this->mVertices.Data (), vertices, total * sizeof ( ZLVec2D ));
		this->SetOpen ( false );
	}
}

