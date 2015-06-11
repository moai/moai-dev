// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLDistance.h>
#include <zl-util/ZLTrig.h>

// TODO: this threshold should probably be passed in; dangerous to assume
#define	FP_NEAR 0.000001f

//================================================================//
// ZLDist
//================================================================//

//----------------------------------------------------------------//
float ZLDist::BoxToPlane ( const ZLBox& b, const ZLPlane3D& p, float& r ) {

	// Get the box spans
	ZLVec3D spans = b.mMax;
	spans.Sub ( b.mMin );
	spans.Scale ( 0.5f );

	// Get the span dots
	float sdX = spans.mX * p.mNorm.mX;
	if ( sdX < 0.0f ) sdX = -sdX;

	float sdY = spans.mY * p.mNorm.mY;
	if ( sdY < 0.0f ) sdY = -sdY;

	float sdZ = spans.mZ * p.mNorm.mZ;
	if ( sdZ < 0.0f ) sdZ = -sdZ;

	// Get the radius of the box (as projected onto the plane's normal)
	r = sdX + sdY + sdZ;

	// Get the box center
	ZLVec3D c = b.mMin;
	c.Add ( spans );

	// The distance from the center of the box to the plane
	return ZLDist::VecToPlane ( c, p );
}

//----------------------------------------------------------------//
float ZLDist::PointToPlane2D ( const ZLVec2D& p, const ZLPlane2D& plane ) {

	float d = p.Dot ( plane.mNorm ) + plane.mDist;
	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float ZLDist::PointToPoint ( const ZLVec2D& p1, const ZLVec2D& p2 ) {

	float x = p2.mX - p1.mX;
	float y = p2.mY - p1.mY;

	float d = sqrtf (( x * x ) + ( y * y ));

	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float ZLDist::PointToPointSqrd ( const ZLVec2D& p1, const ZLVec2D& p2 ) {

	float x = p2.mX - p1.mX;
	float y = p2.mY - p1.mY;

	return  (( x * x ) + ( y * y ));
}

//----------------------------------------------------------------//
float ZLDist::SnapToPlane ( ZLVec3D& p, const ZLPlane3D& plane ) {

	float dist = ZLDist::VecToPlane ( p, plane );

	p.mX = p.mX + ( plane.mNorm.mX * dist );
	p.mY = p.mY + ( plane.mNorm.mY * dist );
	p.mZ = p.mZ + ( plane.mNorm.mZ * dist );

	return dist;
}

//----------------------------------------------------------------//
float ZLDist::SnapToPlane2D ( ZLVec2D& p, const ZLPlane2D& plane ) {

	float dist = ZLDist::PointToPlane2D ( p, plane );

	p.mX = p.mX - ( plane.mNorm.mX * dist );
	p.mY = p.mY - ( plane.mNorm.mY * dist );

	return dist;
}

//----------------------------------------------------------------//
float ZLDist::VecToPlane ( const ZLVec3D& v, const ZLPlane3D& p ) {

	float d = v.Dot ( p.mNorm ) + p.mDist;
	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float ZLDist::VecToVec ( const ZLVec3D& v1, const ZLVec3D& v2 ) {

	float x = v2.mX - v1.mX;
	float y = v2.mY - v1.mY;
	float z = v2.mZ - v1.mZ;

	float d = sqrtf (( x * x ) + ( y * y ) + ( z * z ));

	return (( d < FP_NEAR ) && ( d > -FP_NEAR )) ? 0.0f : d;
}

//----------------------------------------------------------------//
float ZLDist::VecToVecSqrd ( const ZLVec3D& v1, const ZLVec3D& v2 ) {

	float x = v2.mX - v1.mX;
	float y = v2.mY - v1.mY;
	float z = v2.mZ - v1.mZ;

	return  (( x * x ) + ( y * y ) + ( z * z ));
}
