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
void MOAIVectorPath::AddFillContours ( TESStesselator* tess ) {
	UNUSED ( tess );
}

//----------------------------------------------------------------//
void MOAIVectorPath::AddStrokeContours ( TESStesselator* tess ) {

	ZLVec2D* verts = this->mVertices;
	int nJoins = ( int )this->mVertices.Size ();

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nJoins * 2 );
	
	MOAIVectorLineJoin* joins0 = joins;
	MOAIVectorLineJoin* joins1 = &joins [ nJoins ];
	
	this->ComputeLineJoins ( joins, verts, nJoins, true, true, false );
	this->ComputeLineJoins ( &joins [ nJoins ], verts, nJoins, true, false, false );
	
	float width = this->mStyle.GetLineWidth () * 0.5f;
	
	int nVerts0 = this->Stroke ( 0, joins0, nJoins, width, false );
	int nVerts1 = this->Stroke ( 0, joins1, nJoins, width, false );
	int nVerts = nVerts0 + nVerts1;
	
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * nVerts );
	
	ZLVec2D* contour0 = contour;
	ZLVec2D* contour1 = &contour [ nVerts0 ];
	
	this->Stroke ( contour0, joins0, nJoins, width, false );
	this->Stroke ( contour1, joins1, nJoins, width, false );
	
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

