// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorPath.h>
#include <moai-sim/MOAIVectorUtil.h>
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
MOAIVectorPath::MOAIVectorPath () {
}

//----------------------------------------------------------------//
MOAIVectorPath::~MOAIVectorPath () {
}

//----------------------------------------------------------------//
bool MOAIVectorPath::SetVertices ( const ZLVec2D* vertices, u32 total ) {

	if ( total ) {
		this->mVertices.Init ( total );
		MOAIVectorShape::CopyAndTransformVertices ( this->mVertices, this->mStyle.GetTransform (), vertices, total );
		this->SetOpen ( false );
	}
	return true;
}
