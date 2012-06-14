// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USBox.h>
#include <uslscore/USPlane.h>
#include <uslscore/USRhombus.h>

//================================================================//
// USRhombus
//================================================================//

//----------------------------------------------------------------//
void USRhombus::GetAABB ( USBox& box ) const {

	USVec3D walker = mLoc;
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
void USRhombus::GetCenter ( USVec3D& center ) const {

	center = this->mXAxis;
	center.Add ( this->mYAxis );
	center.Scale ( 0.5f );
	center.Add ( this->mLoc );
}

//----------------------------------------------------------------//
void USRhombus::GetPlane ( USPlane3D& plane ) const {

	USVec3D norm;
	norm.Cross ( this->mXAxis, this->mYAxis );
	norm.Norm ();
	plane.Init ( this->mLoc, norm );
}

//----------------------------------------------------------------//
void USRhombus::InitXY ( const USRect& rect, float zOff ) {

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
void USRhombus::InitXZ ( const USRect& rect, float yOff ) {

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
void USRhombus::InitZY ( const USRect& rect, float xOff ) {

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
void USRhombus::Transform ( const USMatrix4x4& mtx ) {

	mtx.Transform ( mLoc );
	mtx.TransformVec ( mXAxis );
	mtx.TransformVec ( mYAxis );
}
