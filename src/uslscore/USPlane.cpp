// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USPlane.h>

//================================================================//
// USPlane2D
//================================================================//

//----------------------------------------------------------------//
void USPlane2D::Flip () {

	this->mNorm.Reverse ();
	this->mDist = -this->mDist;
}

//----------------------------------------------------------------//
void USPlane2D::Init ( const USVec2D& p0, const USVec2D& p1 ) {

	this->mNorm = p1;
	this->mNorm.Sub ( p0 );
	
	this->mNorm.Rotate90Anticlockwise ();
	this->mNorm.Norm ();

	this->mDist = -this->mNorm.Dot ( p0 );
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
void USPlane3D::Init ( const USVec3D& p, const USVec3D& n ) {

	mNorm = n;
	mDist = -p.Dot ( n );
}

//----------------------------------------------------------------//
void USPlane3D::Init ( const USVec3D& p0, const USVec3D& p1, const USVec3D& p2 ) {

	USVec3D r;

	mNorm = p1;
	mNorm.Sub ( p0 );

	r = p2;
	r.Sub ( p0 );

	mNorm.Cross ( r );
	mNorm.Norm ();

	mDist = -mNorm.Dot ( p0 );
}
