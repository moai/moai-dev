// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorPoly.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorPoly
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorPoly::AddFillContours ( TESStesselator* tess ) {
	
	TESStesselator* outline = tessNewTess ( 0 );
	assert ( outline );

	tessAddContour ( outline, 2, this->mVertices.Data (), sizeof ( ZLVec2D ), this->mVertices.Size ());
	
	tessTesselate ( outline, ( int )this->mStyle.GetWindingRule (), TESS_BOUNDARY_CONTOURS, 0, 0, ( const TESSreal* )&sNormal );
	this->CopyBoundaries ( tess, outline );
	
	tessDeleteTess ( outline );
}

//----------------------------------------------------------------//
void MOAIVectorPoly::AddStrokeContours ( TESStesselator* tess ) {

	if ( this->mIsClosed ) {
		MOAIVectorShape::AddStrokeContours ( tess );
		return;
	}

	ZLVec2D* verts = this->mVertices;
	int nJoins = ( int )this->mVertices.Size ();

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nJoins * 2 );
	
	MOAIVectorLineJoin* joins0 = joins;
	MOAIVectorLineJoin* joins1 = &joins [ nJoins ];
	
	MOAIVectorUtil::ComputeLineJoins ( joins, verts, nJoins, true, true, false );
	MOAIVectorUtil::ComputeLineJoins ( &joins [ nJoins ], verts, nJoins, true, false, false );
	
	float width = this->mStyle.GetStrokeWidth () * 0.5f;
	
	int nVerts0 = MOAIVectorUtil::StrokeLine ( this->mStyle, 0, joins0, nJoins, width, false );
	int nVerts1 = MOAIVectorUtil::StrokeLine ( this->mStyle, 0, joins1, nJoins, width, false );
	int nVerts = nVerts0 + nVerts1;
	
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * nVerts );
	
	ZLVec2D* contour0 = contour;
	ZLVec2D* contour1 = &contour [ nVerts0 ];
	
	MOAIVectorUtil::StrokeLine ( this->mStyle, contour0, joins0, nJoins, width, false );
	MOAIVectorUtil::StrokeLine ( this->mStyle, contour1, joins1, nJoins, width, false );
	
	tessAddContour ( tess, 2, contour, sizeof ( ZLVec2D ), nVerts );
}

//----------------------------------------------------------------//
MOAIVectorPoly::MOAIVectorPoly () :
	mIsClosed ( true ) {
}

//----------------------------------------------------------------//
MOAIVectorPoly::~MOAIVectorPoly () {
}

//----------------------------------------------------------------//
bool MOAIVectorPoly::SetVertices ( const ZLVec2D* vertices, u32 total, bool closed ) {

	if ( total ) {
		this->mVertices.Init ( total );
		MOAIVectorShape::CopyAndTransformVertices ( this->mVertices, this->mStyle.GetTransform (), vertices, total );
		this->mIsClosed = closed;
		//this->SetOpen ( false );
	}
	return true;
}
