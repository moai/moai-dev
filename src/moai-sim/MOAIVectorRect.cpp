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
void MOAIVectorRect::AddFillContours ( MOAIVectorDrawing& drawing, TESStesselator* tess ) {
	UNUSED ( drawing );

	ZLVec2D* verts = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * 4 );

	verts [ 0 ].mX = this->mRect.mXMin;
	verts [ 0 ].mY = this->mRect.mYMin;
	
	verts [ 1 ].mX = this->mRect.mXMax;
	verts [ 1 ].mY = this->mRect.mYMin;
	
	verts [ 2 ].mX = this->mRect.mXMax;
	verts [ 2 ].mY = this->mRect.mYMax;
	
	verts [ 3 ].mX = this->mRect.mXMin;
	verts [ 3 ].mY = this->mRect.mYMax;

	tessAddContour ( tess, 2, verts, sizeof ( ZLVec2D ), 4 );
}

//----------------------------------------------------------------//
void MOAIVectorRect::AddStrokeContours ( MOAIVectorDrawing& drawing, TESStesselator* tess ) {

	MOAIVectorShape::AddStrokeContours ( drawing, tess );
}

//----------------------------------------------------------------//
void MOAIVectorRect::Init ( const ZLRect& rect ) {

	this->mRect = rect;
}

//----------------------------------------------------------------//
MOAIVectorRect::MOAIVectorRect () {
}

//----------------------------------------------------------------//
MOAIVectorRect::~MOAIVectorRect () {
}
