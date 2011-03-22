// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USDistance.h>
#include <uslsext/USTrig.h>

// TODO: this threshold should probably be passed in; dangerous to assume
#define	FP_NEAR 0.000001f

//================================================================//
// USDist
//================================================================//

//----------------------------------------------------------------//
float USDist::PointToPlane2D ( USVec2D& p, USPlane2D& plane ) {

	float d = p.Dot ( plane.mNorm ) + plane.mDist;
	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float USDist::PointToPoint ( USVec2D& p1, USVec2D& p2 ) {

	float x = p2.mX - p1.mX;
	float y = p2.mY - p1.mY;

	float d = sqrtf (( x * x ) + ( y * y ));

	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float USDist::PointToPointSqrd ( USVec2D& p1, USVec2D& p2 ) {

	float x = p2.mX - p1.mX;
	float y = p2.mY - p1.mY;

	return  (( x * x ) + ( y * y ));
}

//----------------------------------------------------------------//
float USDist::SnapToPlane ( USVec3D& p, USPlane3D& plane ) {

	float dist = USDist::VecToPlane ( p, plane );

	p.mX = p.mX + ( plane.mNorm.mX * dist );
	p.mY = p.mY + ( plane.mNorm.mY * dist );
	p.mZ = p.mZ + ( plane.mNorm.mZ * dist );

	return dist;
}

//----------------------------------------------------------------//
float USDist::SnapToPlane2D ( USVec2D& p, USPlane2D& plane ) {

	float dist = USDist::PointToPlane2D ( p, plane );

	p.mX = p.mX - ( plane.mNorm.mX * dist );
	p.mY = p.mY - ( plane.mNorm.mY * dist );

	return dist;
}

//----------------------------------------------------------------//
float USDist::VecToPlane ( USVec3D& v, USPlane3D& p ) {

	float d = v.Dot ( p.mNorm ) + p.mDist;
	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float USDist::VecToVec ( USVec3D& v1, USVec3D& v2 ) {

	float x = v2.mX - v1.mX;
	float y = v2.mY - v1.mY;
	float z = v2.mZ - v1.mZ;

	float d = sqrtf (( x * x ) + ( y * y ) + ( z * z ));

	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float USDist::VecToVecSqrd ( USVec3D& v1, USVec3D& v2 ) {

	float x = v2.mX - v1.mX;
	float y = v2.mY - v1.mY;
	float z = v2.mZ - v1.mZ;

	return  (( x * x ) + ( y * y ) + ( z * z ));
}
