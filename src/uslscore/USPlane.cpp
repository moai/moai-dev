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
void USPlane2D::Init ( const USVec2D& p1, const USVec2D& p2 ) {

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
void USPlane3D::Init ( const USVec3D& p, const USVec3D& n ) {

	mNorm = n;
	mDist = -p.Dot ( n );
}

//----------------------------------------------------------------//
void USPlane3D::Init ( const USVec3D& p1, const USVec3D& p2, const USVec3D& p3 ) {

	USVec3D r;

	mNorm = p2;
	mNorm.Sub ( p1 );

	r = p3;
	r.Sub ( p1 );

	mNorm.Cross ( r );
	mNorm.Norm ();

	mDist = -mNorm.Dot ( p1 );
}
