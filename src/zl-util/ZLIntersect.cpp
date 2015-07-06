// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLDistance.h>
#include <zl-util/ZLIntersect.h>
#include <zl-util/ZLTrig.h>
#include <zl-util/ZLPrism.h>
#include <zl-util/ZLRhombus.h>
#include <float.h>

//================================================================//
// local
//================================================================//

bool _clipRayToBoxAxis ( float min, float max, float pos, float dir, float& t0, float& t1 );

//----------------------------------------------------------------//
bool _clipRayToBoxAxis ( float min, float max, float pos, float dir, float& t0, float& t1 ) {
	
	if ( fabs ( dir ) < EPSILON ) {
		return pos >= min && pos <= max;
	}
	
	float u0, u1;
	
	u0 = ( min - pos ) / ( dir );
	u1 = ( max - pos ) / ( dir );
	
	if ( u0 > u1 ) {
		float temp = u0;
		u0 = u1;
		u1 = temp;
	}
	
	if ( u1 < t0 || u0 > t1 ) {
		return false;
	}
	
	t0 = MAX ( u0, t0 );
	t1 = MIN ( u1, t1 );
	
	if ( t1 < t0 ) {
		
		return false;
	}
	
	return true; 
}

//================================================================//
// ZLSect
//================================================================//

//----------------------------------------------------------------//
// Return:
//	 1:		Box is in front of the plane
//	 0:		Box intersects the plane
//	-1:		Box is behind the plane
s32 ZLSect::BoxToPlane ( const ZLBox& b, const ZLPlane3D& p ) {

	// The distance from the center of the box to the plane
	float r;
	float d = ZLDist::BoxToPlane ( b, p, r );

	// Now test against the span
	if ( d > r ) return 1; // The box is in front of the plane
	if ( d < -r ) return -1; // The box is behind the plane
	return 0; // The box intersects the plane
}

//----------------------------------------------------------------//
u32 ZLSect::LineToLine ( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& q0, const ZLVec2D& q1 ) {

	float t;
	return ZLSect::LineToLine ( p0, p1, q0, q1, t );
}

//----------------------------------------------------------------//
u32 ZLSect::LineToLine ( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& q0, const ZLVec2D& q1, float& t ) {

	// TODO: this one ignores colinear lines. provide an alternative method that cares about colinear lines.

	ZLVec2D r = ZLVec2D::Sub ( p1, p0 ); // a0, b0
	ZLVec2D s = ZLVec2D::Sub ( q1, q0 ); // a1, b1

	float det = ZLVec2D::Cross ( r, s ); // c0

	// parallel
	if ( det == 0 ) return SECT_PARALLEL;

	ZLVec2D m = ZLVec2D::Sub ( q0, p0 );

	t = ZLVec2D::Cross ( m, s ) / det;
	float u = ZLVec2D::Cross ( m, r ) / det;

	return ( t >= 0.0f ) && ( t <= 1.0f ) && ( u >= 0.0f ) && ( u <= 1.0f ) ? SECT_HIT : SECT_HIT_OUT_OF_RANGE;
}

//----------------------------------------------------------------//
u32 ZLSect::PlaneToPlane ( const ZLPlane2D& p0, const ZLPlane2D& p1, ZLVec2D& loc ) {

	loc = p0.mNorm;
	loc.Scale ( -p0.mDist );

	ZLVec2D vec = p0.mNorm;
	vec.Rotate90Clockwise ();
	
	float t;
	u32 result = ZLSect::VecToPlane ( loc, vec, p1, t );
	if ( result == SECT_HIT ) {
		loc.Add ( vec, t );
	}
	return result;
}

//----------------------------------------------------------------//
u32 ZLSect::PlaneToPlane ( const ZLPlane3D& p0, const ZLPlane3D& p1, ZLVec3D& loc, ZLVec3D& vec ) {

	vec = ZLVec3D::Cross ( p0.mNorm, p1.mNorm );
    if ( vec.LengthSqrd () < FLT_EPSILON ) return SECT_PARALLEL;

	u32 component;
	
	float absX = ABS ( vec.mX );
	float absY = ABS ( vec.mY );
	float absZ = ABS ( vec.mZ );

	// find the mightiest component
    switch (( absX > absY ) ? ( absX > absZ ? 0 : 2 ) : ( absY > absZ ? 1 : 2 )) {
	
		// solve using X
		case 0:
			loc.mX = 0;
			loc.mY = (( p1.mDist * p0.mNorm.mZ ) - ( p0.mDist * p1.mNorm.mZ )) / vec.mX;
			loc.mZ = (( p0.mDist * p1.mNorm.mY ) - ( p1.mDist * p0.mNorm.mY )) / vec.mX;
			break;
		
		// solve using Y
		case 1:
			loc.mX = (( p0.mDist * p1.mNorm.mZ ) - ( p1.mDist * p0.mNorm.mZ )) / vec.mY;
			loc.mY = 0;
			loc.mZ = (( p1.mDist * p0.mNorm.mX ) - ( p0.mDist * p1.mNorm.mX )) / vec.mY;
			break;
		
		// solve using Z
		case 2:
			loc.mX = (( p1.mDist * p0.mNorm.mY ) - ( p0.mDist * p1.mNorm.mY )) / vec.mZ;
			loc.mY = (( p0.mDist * p1.mNorm.mX ) - ( p1.mDist * p0.mNorm.mX )) / vec.mZ;
			loc.mZ = 0;
    }
	
    return SECT_HIT;
}

//----------------------------------------------------------------//
// Return:
//	 1:		Prism is in front of the plane
//	 0:		Prism intersects the plane
//	-1:		Prism is behind the plane
s32 ZLSect::PrismToPlane ( const ZLPrism& prism, const ZLPlane3D& p ) {

	// Get the span dots
	float sdX = prism.mXAxis.Dot ( p.mNorm );
	if ( sdX < 0.0f ) sdX = -sdX;

	float sdY = prism.mXAxis.Dot ( p.mNorm );
	if ( sdY < 0.0f ) sdY = -sdY;

	float sdZ = prism.mXAxis.Dot ( p.mNorm );
	if ( sdZ < 0.0f ) sdZ = -sdZ;

	// Get the radius of the prism (as projected onto the plane's normal)
	float r = ( sdX + sdY + sdZ ) * 0.5f;

	// Get the prism center
	ZLVec3D c;
	prism.GetCenter ( c );

	// The distance from the center of the prism to the plane
	float d = ZLDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The prism is in front of the plane
	if ( d < -r ) return -1; // The prism is behind the plane
	return 0; // The prism intersects the plane
}

//----------------------------------------------------------------//
// Return:
//	 1:		Rhombus is in front of the plane
//	 0:		Rhombus intersects the plane
s32 ZLSect::RayToBox ( const ZLBox& b, const ZLVec3D& loc, const ZLVec3D& dir, float &t ) {

	float t0 = 0.0f;
	float t1 = FLT_MAX;
	
	if ( !_clipRayToBoxAxis ( b.mMin.mX, b.mMax.mX, loc.mX, dir.mX, t0, t1 )) {
		return 1;
	}
	
	if ( !_clipRayToBoxAxis ( b.mMin.mY, b.mMax.mY, loc.mY, dir.mY, t0, t1 )) {
		return 1;
	}
	
	if ( !_clipRayToBoxAxis ( b.mMin.mZ, b.mMax.mZ, loc.mZ, dir.mZ, t0, t1 )) {
		return 1;
	}
	
	if ( t0 < t1 ) {
		t = t0;
	}
	else {
		t = t1;
	}

	return 0;
}

//----------------------------------------------------------------//
// Return:
//	 1:		Rhombus is in front of the plane
//	 0:		Rhombus intersects the plane
//	-1:		Rhombus is behind the plane
s32 ZLSect::RhombusToPlane ( const ZLRhombus& rhombus, const ZLPlane3D& p ) {

	// Get the span dots
	float sdX = rhombus.mXAxis.Dot ( p.mNorm );
	if ( sdX < 0.0f ) sdX = -sdX;

	float sdY = rhombus.mYAxis.Dot ( p.mNorm );
	if ( sdY < 0.0f ) sdY = -sdY;

	// Get the radius of the rhombus (as projected onto the plane's normal)
	float r = ( sdX + sdY ) * 0.5f;

	// Get the rhombus center
	ZLVec3D c;
	rhombus.GetCenter ( c );

	// The distance from the center of the rhombus to the plane
	float d = ZLDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The rhombus is in front of the plane
	if ( d < -r ) return -1; // The rhombus is behind the plane
	return 0; // The rhombus intersects the plane
}

//----------------------------------------------------------------//
u32 ZLSect::VecToCircle ( float& t0, float& t1, ZLVec2D& loc, ZLVec2D& vec, ZLVec2D& circleLoc, float radius ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY );

	b = (( vec.mX * ( loc.mX - circleLoc.mX )) +
		( vec.mY * ( loc.mY - circleLoc.mY ))) * 2.0f;

	c = ((( loc.mX - circleLoc.mX ) * ( loc.mX - circleLoc.mX )) +
		(( loc.mY - circleLoc.mY ) * ( loc.mY - circleLoc.mY )) -
		radius * radius );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		assert ( t0 < t1 );

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 ZLSect::VecToPlane ( const ZLVec2D& loc, const ZLVec2D& vec, const ZLPlane2D& p, float& t ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 ZLSect::VecToPlane ( const ZLVec3D& loc, const ZLVec3D& vec, const ZLPlane3D& p, float& t ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 ZLSect::VecToPlane ( const ZLVec3D& loc, const ZLVec3D& vec, const ZLPlane3D& p, ZLVec3D& result ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	float t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	
	result = vec;
	result.Scale ( t );
	result.Add ( loc );
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 ZLSect::VecToPlane ( const ZLVec3D& loc, const ZLVec3D& vec, const ZLPlane3D& p, float& t, ZLVec3D& result ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	
	result = vec;
	result.Scale ( t );
	result.Add ( loc );
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 ZLSect::VecToSphere ( float& t0, float& t1, const ZLVec3D& loc, const ZLVec3D& vec, const ZLVec3D& sphereLoc, float radius ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY ) + ( vec.mZ * vec.mZ );

	b = (( vec.mX * ( loc.mX - sphereLoc.mX )) +
		( vec.mY * ( loc.mY - sphereLoc.mY )) +
		( vec.mZ * ( loc.mZ - sphereLoc.mZ ))) * 2.0f;

	c = ((( loc.mX - sphereLoc.mX ) * ( loc.mX - sphereLoc.mX )) +
		(( loc.mY - sphereLoc.mY ) * ( loc.mY - sphereLoc.mY )) +
		(( loc.mZ - sphereLoc.mZ ) * ( loc.mZ - sphereLoc.mZ )) -
		radius * radius );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 ZLSect::VecToUnitCircle ( float& t0, float& t1, const ZLVec2D& loc, const ZLVec2D& vec ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY );

	b = (( vec.mX * loc.mX ) +
		( vec.mY * loc.mY )) * 2.0f;

	c = (( loc.mX * loc.mX ) +
		( loc.mY * loc.mY ) -
		1.0f );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		assert ( t0 < t1 );

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 ZLSect::VecToUnitSphere ( float& t0, float& t1, const ZLVec3D& loc, const ZLVec3D& vec ) {

	float a, b, c, d;

	a = ( vec.mX * vec.mX ) + ( vec.mY * vec.mY ) + ( vec.mZ * vec.mZ );

	b = (( vec.mX * loc.mX ) +
		( vec.mY * loc.mY ) +
		( vec.mZ * loc.mZ )) * 2.0f;

	c = (( loc.mX * loc.mX ) +
		( loc.mY * loc.mY ) +
		( loc.mZ * loc.mZ ) -
		1.0f );

	d = (( b * b ) - ( 4.0f * a * c ));
	a = 2.0f * a;

	if ( d > 0.0f ) {

		d = sqrtf ( d );

		t0 = (( -b - d )/( a ));
		t1 = (( -b + d )/( a ));

		assert ( t0 < t1 );

		return SECT_HIT;
	}
	else if ( d == 0.0f ) {

		t0 = (( -b )/( a ));
		t1 = t0;
		return SECT_TANGENT;
	}

	return SECT_PARALLEL;
}

//----------------------------------------------------------------//
u32 ZLSect::XAxisToPlane ( float y, const ZLPlane2D& p, float& t ) {

	float d;
	d = p.mNorm.mX;
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = (( p.mNorm.mY * y ) + p.mDist ) / -d;
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 ZLSect::YAxisToPlane ( float x, const ZLPlane2D& p, float& t ) {

	float d;
	d = p.mNorm.mY;
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = (( p.mNorm.mX * x ) + p.mDist ) / -d;
	
	return SECT_HIT;
}
