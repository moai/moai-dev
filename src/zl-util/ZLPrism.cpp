// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBox.h>
#include <zl-util/ZLPrism.h>

//================================================================//
// ZLPrism
//================================================================//

//----------------------------------------------------------------//
void ZLPrism::GetAABB ( ZLBox& box ) const {

	ZLVec3D walker = mLoc;
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
void ZLPrism::GetCenter ( ZLVec3D& center ) const {

	center = this->mXAxis;
	center.Add ( this->mYAxis );
	center.Add ( this->mZAxis );
	center.Scale ( 0.5f );
	center.Add ( this->mLoc );
}

//----------------------------------------------------------------//
void ZLPrism::Init ( const ZLBox& box ) {

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
void ZLPrism::Transform ( const ZLAffine3D& mtx ) {

	mtx.Transform ( mLoc );
	mtx.TransformVec ( mXAxis );
	mtx.TransformVec ( mYAxis );
	mtx.TransformVec ( mZAxis );
}

//----------------------------------------------------------------//
void ZLPrism::Transform ( const ZLMatrix4x4& mtx ) {

	mtx.Transform ( mLoc );
	mtx.TransformVec ( mXAxis );
	mtx.TransformVec ( mYAxis );
	mtx.TransformVec ( mZAxis );
}
