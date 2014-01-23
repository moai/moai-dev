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
	
	if ( this->mVertices.Size () < 3 ) return;
	
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

	int nVerts = ( int )this->mVertices.Size ();
	if ( nVerts < 2 ) return;

	// filter out duplicate vertices
	ZLVec2D* verts = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * nVerts );
	verts [ 0 ] = this->mVertices [ 0 ];

	int nJoins = 1;
	for ( int i = 1; i < nVerts; ++i ) {
		
		const ZLVec2D& v0 = verts [ nJoins - 1 ];
		const ZLVec2D& v1 = this->mVertices [ i ];
	
		if (( v1.mX != v0.mX ) || ( v1.mY != v0.mY )) {
			verts [ nJoins++ ] = v1;
		}
	}
	if ( nJoins < 2 ) return;

	MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nJoins * 2 );
	
	MOAIVectorLineJoin* joins0 = joins;
	MOAIVectorLineJoin* joins1 = &joins [ nJoins ];
	
	MOAIVectorUtil::ComputeLineJoins ( joins, verts, nJoins, true, true, false );
	MOAIVectorUtil::ComputeLineJoins ( &joins [ nJoins ], verts, nJoins, true, false, false );
	
	float width = this->mStyle.GetStrokeWidth () * 0.5f;
	
	int nContourVerts0 = MOAIVectorUtil::StrokeLine ( this->mStyle, 0, joins0, nJoins, width, false );
	int nContourVerts1 = MOAIVectorUtil::StrokeLine ( this->mStyle, 0, joins1, nJoins, width, false );
	int nContourVerts = nContourVerts0 + nContourVerts1;
	
	ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * nContourVerts );
	
	ZLVec2D* contour0 = contour;
	ZLVec2D* contour1 = &contour [ nContourVerts0 ];
	
	MOAIVectorUtil::StrokeLine ( this->mStyle, contour0, joins0, nJoins, width, false );
	MOAIVectorUtil::StrokeLine ( this->mStyle, contour1, joins1, nJoins, width, false );
	
	tessAddContour ( tess, 2, contour, sizeof ( ZLVec2D ), nContourVerts );
}

//----------------------------------------------------------------//
bool MOAIVectorPoly::IsClosed () {
	return this->mIsClosed;
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
		MOAIVectorShape::CopyAndTransformVertices ( this->mVertices, this->mStyle.GetDrawingToWorld (), vertices, total );
		this->mIsClosed = closed;
		//this->SetOpen ( false );
	}
	return true;
}
