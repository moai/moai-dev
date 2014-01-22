// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorRect.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorRect
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorRect::AddFillContours ( TESStesselator* tess ) {

	ZLVec2D* verts = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * 4 );

	verts [ 0 ].mX = this->mRect.mXMin;
	verts [ 0 ].mY = this->mRect.mYMin;
	
	verts [ 1 ].mX = this->mRect.mXMax;
	verts [ 1 ].mY = this->mRect.mYMin;
	
	verts [ 2 ].mX = this->mRect.mXMax;
	verts [ 2 ].mY = this->mRect.mYMax;
	
	verts [ 3 ].mX = this->mRect.mXMin;
	verts [ 3 ].mY = this->mRect.mYMax;

	this->mStyle.GetDrawingToWorld ().Transform ( verts [ 0 ]);
	this->mStyle.GetDrawingToWorld ().Transform ( verts [ 1 ]);
	this->mStyle.GetDrawingToWorld ().Transform ( verts [ 2 ]);
	this->mStyle.GetDrawingToWorld ().Transform ( verts [ 3 ]);

	tessAddContour ( tess, 2, verts, sizeof ( ZLVec2D ), 4 );
}

//----------------------------------------------------------------//
void MOAIVectorRect::AddStrokeContours ( TESStesselator* tess ) {

	MOAIVectorShape::AddStrokeContours ( tess );
}

//----------------------------------------------------------------//
void MOAIVectorRect::Init ( float xMin, float yMin, float xMax, float yMax ) {

	this->mRect.Init ( xMin, yMin, xMax, yMax );
}

//----------------------------------------------------------------//
bool MOAIVectorRect::IsClosed () {
	return true;
}

//----------------------------------------------------------------//
MOAIVectorRect::MOAIVectorRect () {
}

//----------------------------------------------------------------//
MOAIVectorRect::~MOAIVectorRect () {
}
