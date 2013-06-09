// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLPlane.h>

//================================================================//
// ZLPlane2D
//================================================================//

//----------------------------------------------------------------//
void ZLPlane2D::Flip () {

	this->mNorm.Reverse ();
	this->mDist = -this->mDist;
}

//----------------------------------------------------------------//
void ZLPlane2D::Init ( const USVec2D& p1, const USVec2D& p2 ) {

	this->mNorm = p2;
	this->mNorm.Sub ( p1 );
	
	this->mNorm.Rotate90Anticlockwise ();
	this->mNorm.Norm ();

	this->mDist = -this->mNorm.Dot ( p1 );
}

//================================================================//
// USPlane3D
//================================================================//

//----------------------------------------------------------------//
void USPlane3D::Flip () {

	this->mNorm.Reverse ();
	this->mDist = -this->mDist;
}

//----------------------------------------------------------------//
void USPlane3D::Init ( const ZLVec3D& p, const ZLVec3D& n ) {

	mNorm = n;
	mDist = -p.Dot ( n );
}

//----------------------------------------------------------------//
void USPlane3D::Init ( const ZLVec3D& p1, const ZLVec3D& p2, const ZLVec3D& p3 ) {

	ZLVec3D r;

	mNorm = p2;
	mNorm.Sub ( p1 );

	r = p3;
	r.Sub ( p1 );

	mNorm.Cross ( r );
	mNorm.Norm ();

	mDist = -mNorm.Dot ( p1 );
}
