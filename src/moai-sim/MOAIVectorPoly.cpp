// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorPoly.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorPoly
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorPoly::AddFillContours ( SafeTesselator* tess ) {
	
	if ( this->mVertices.Size () < 3 ) return 0;
	
	SafeTesselator outline;

	outline.AddContour ( 2, this->mVertices.Data (), sizeof ( ZLVec2D ), this->mVertices.Size ());
	
	int error = outline.Tesselate (( int )this->mStyle.GetWindingRule (), TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( error ) return error;
	
	this->CopyBoundaries ( tess, &outline );
	
	return error;
}

//----------------------------------------------------------------//
int MOAIVectorPoly::AddStrokeContours ( SafeTesselator* tess ) {

	if ( this->mIsClosed ) {
		return MOAIVectorShape::AddStrokeContours ( tess );
	}

	int nVerts = ( int )this->mVertices.Size ();
	if ( nVerts < 2 ) return 0;

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
	if ( nJoins < 2 ) return 0;

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
	
	tess->AddContour ( 2, contour, sizeof ( ZLVec2D ), nContourVerts );
	
	return 0;
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
	}
	return true;
}
