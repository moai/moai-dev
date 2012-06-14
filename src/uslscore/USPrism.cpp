// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USBox.h>
#include <uslscore/USPrism.h>

//================================================================//
// USPrism
//================================================================//

//----------------------------------------------------------------//
void USPrism::GetAABB ( USBox& box ) const {

	USVec3D walker = mLoc;
	box.Init ( walker );

	walker.Add ( mYAxis );
	box.Grow ( walker );

	walker.Add ( mZAxis );
	box.Grow ( walker );

	walker.Sub ( mYAxis );
	box.Grow ( walker );
	
	walker.Add ( mXAxis );
	box.Grow ( walker );
	
	walker.Sub ( mZAxis );
	box.Grow ( walker );

	walker.Add ( mYAxis );
	box.Grow ( walker );

	walker.Add ( mZAxis );
	box.Grow ( walker );
}

//----------------------------------------------------------------//
void USPrism::GetCenter ( USVec3D& center ) const {

	center = this->mXAxis;
	center.Add ( this->mYAxis );
	center.Add ( this->mZAxis );
	center.Scale ( 0.5f );
	center.Add ( this->mLoc );
}

//----------------------------------------------------------------//
void USPrism::Init ( const USBox& box ) {

	this->mLoc = box.mMin;
	
	this->mXAxis.mX = box.mMax.mX - box.mMin.mX;
	this->mXAxis.mY = 0.0f;
	this->mXAxis.mZ = 0.0f;
	
	this->mYAxis.mX = 0.0f;
	this->mYAxis.mY = box.mMax.mY - box.mMin.mY;
	this->mYAxis.mZ = 0.0f;
	
	this->mZAxis.mX = 0.0f;
	this->mZAxis.mY = 0.0f;
	this->mZAxis.mZ = box.mMax.mZ - box.mMin.mZ;
}

//----------------------------------------------------------------//
void USPrism::Transform ( const USAffine3D& mtx ) {

	mtx.Transform ( mLoc );
	mtx.TransformVec ( mXAxis );
	mtx.TransformVec ( mYAxis );
	mtx.TransformVec ( mZAxis );
}

//----------------------------------------------------------------//
void USPrism::Transform ( const USMatrix4x4& mtx ) {

	mtx.Transform ( mLoc );
	mtx.TransformVec ( mXAxis );
	mtx.TransformVec ( mYAxis );
	mtx.TransformVec ( mZAxis );
}
