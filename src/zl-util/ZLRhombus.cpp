// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBox.h>
#include <zl-util/ZLPlane.h>
#include <zl-util/ZLRhombus.h>

//================================================================//
// ZLRhombus
//================================================================//

//----------------------------------------------------------------//
void ZLRhombus::GetAABB ( ZLBox& box ) const {

	ZLVec3D walker = mLoc;
	box.Init ( walker );

	walker.Add ( mXAxis );
	box.Grow ( walker );

	walker.Add ( mYAxis );
	box.Grow ( walker );

	walker.Sub ( mXAxis );
	box.Grow ( walker );
	
	walker.Sub ( mYAxis );
	box.Grow ( walker );
}

//----------------------------------------------------------------//
void ZLRhombus::GetCenter ( ZLVec3D& center ) const {

	center = this->mXAxis;
	center.Add ( this->mYAxis );
	center.Scale ( 0.5f );
	center.Add ( this->mLoc );
}

//----------------------------------------------------------------//
void ZLRhombus::GetPlane ( ZLPlane3D& plane ) const {

	ZLVec3D norm;
	norm.Cross ( this->mXAxis, this->mYAxis );
	norm.Norm ();
	plane.Init ( this->mLoc, norm );
}

//----------------------------------------------------------------//
void ZLRhombus::InitXY ( const ZLRect& rect, float zOff ) {

	this->mLoc.mX = rect.mXMin;
	this->mLoc.mY = rect.mYMin;
	this->mLoc.mZ = zOff;
	
	this->mXAxis.mX = rect.mXMax - rect.mXMin;
	this->mXAxis.mY = 0.0f;
	this->mXAxis.mZ = 0.0f;
	
	this->mYAxis.mX = 0.0f;
	this->mYAxis.mY = rect.mYMax - rect.mYMin;
	this->mYAxis.mZ = 0.0f;
}

//----------------------------------------------------------------//
void ZLRhombus::InitXZ ( const ZLRect& rect, float yOff ) {

	this->mLoc.mX = rect.mXMin;
	this->mLoc.mY = yOff;
	this->mLoc.mZ = rect.mYMin;
	
	this->mXAxis.mX = rect.mXMax - rect.mXMin;
	this->mXAxis.mY = 0.0f;
	this->mXAxis.mZ = 0.0f;
	
	this->mYAxis.mX = 0.0f;
	this->mYAxis.mY = 0.0f;
	this->mYAxis.mZ = rect.mYMax - rect.mYMin;
}

//----------------------------------------------------------------//
void ZLRhombus::InitZY ( const ZLRect& rect, float xOff ) {

	this->mLoc.mX = xOff;
	this->mLoc.mY = rect.mYMin;
	this->mLoc.mZ = rect.mXMin;
	
	this->mXAxis.mX = 0.0f;
	this->mXAxis.mY = 0.0f;
	this->mXAxis.mZ = rect.mXMax - rect.mXMin;
	
	this->mYAxis.mX = 0.0f;
	this->mYAxis.mY = rect.mYMax - rect.mYMin;
	this->mYAxis.mZ = 0.0f;
}

//----------------------------------------------------------------//
void ZLRhombus::Transform ( const ZLMatrix4x4& mtx ) {

	mtx.Transform ( mLoc );
	mtx.TransformVec ( mXAxis );
	mtx.TransformVec ( mYAxis );
}
