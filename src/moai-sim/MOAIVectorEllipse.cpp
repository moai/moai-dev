// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorEllipse.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorEllipse
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorEllipse::AddFillContours ( SafeTesselator* tess ) {
	
	u32 steps = this->mStyle.GetCircleResolution ();

	ZLVec2D* verts = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * steps );

	float step = ( float )TWOPI / ( float )steps;
	float angle = ( float )PI;
	
	for ( u32 i = 0; i < steps; ++i, angle += step ) {
		verts [ i ].mX = this->mLoc.mX + ( Cos ( angle ) * this->mXRad );
		verts [ i ].mY = this->mLoc.mY + ( Sin ( angle ) * this->mYRad );
		this->mStyle.GetDrawingToWorld ().Transform ( verts [ i ]);
	}
	tess->AddContour ( 2, verts, sizeof ( ZLVec2D ), steps );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVectorEllipse::AddStrokeContours ( SafeTesselator* tess ) {

	return MOAIVectorShape::AddStrokeContours ( tess );
}

//----------------------------------------------------------------//
void MOAIVectorEllipse::Init ( float x, float y, float xRad, float yRad ) {

	this->mLoc.Init ( x, y );
	this->mXRad = xRad;
	this->mYRad = yRad;
}

//----------------------------------------------------------------//
bool MOAIVectorEllipse::IsClosed () {
	return true;
}

//----------------------------------------------------------------//
MOAIVectorEllipse::MOAIVectorEllipse () {
}

//----------------------------------------------------------------//
MOAIVectorEllipse::~MOAIVectorEllipse () {
}
