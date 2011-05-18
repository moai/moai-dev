// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USIntersect.h>
#include <uslsext/USTrig.h>
#include <uslsext/USDistance.h>

//----------------------------------------------------------------//
// Return:
//	 1:		Box is in front of the plane
//	 0:		Box intersects the plane
//	-1:		Box is behind the plane
s32 USSect::BoxToPlane ( USBox& b, USPlane3D& p ) {

	// Get the box spans
	USVec3D spans = b.mMax;
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
	float r = sdX + sdY + sdZ;

	// Get the box center
	USVec3D c = b.mMin;
	c.Add ( spans );

	// The distance from the center of the box to the plane
	float d = USDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The box is in front of the plane
	if ( d < -r ) return -1; // The box is behind the plane
	return 0; // The box intersects the plane
}

//----------------------------------------------------------------//
// Return:
//	 1:		Prism is in front of the plane
//	 0:		Prism intersects the plane
//	-1:		Prism is behind the plane
s32 USSect::PrismToPlane ( USPrism& prism, USPlane3D& p ) {

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
	USVec3D c;
	prism.GetCenter ( c );

	// The distance from the center of the prism to the plane
	float d = USDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The prism is in front of the plane
	if ( d < -r ) return -1; // The prism is behind the plane
	return 0; // The prism intersects the plane
}

//----------------------------------------------------------------//
// Return:
//	 1:		Rhombus is in front of the plane
//	 0:		Rhombus intersects the plane
//	-1:		Rhombus is behind the plane
s32 USSect::RhombusToPlane ( USRhombus& rhombus, USPlane3D& p ) {

	// Get the span dots
	float sdX = rhombus.mXAxis.Dot ( p.mNorm );
	if ( sdX < 0.0f ) sdX = -sdX;

	float sdY = rhombus.mYAxis.Dot ( p.mNorm );
	if ( sdY < 0.0f ) sdY = -sdY;

	// Get the radius of the rhombus (as projected onto the plane's normal)
	float r = ( sdX + sdY ) * 0.5f;

	// Get the rhombus center
	USVec3D c;
	rhombus.GetCenter ( c );

	// The distance from the center of the rhombus to the plane
	float d = USDist::VecToPlane ( c, p );

	// Now test against the span
	if ( d > r ) return 1; // The rhombus is in front of the plane
	if ( d < -r ) return -1; // The rhombus is behind the plane
	return 0; // The rhombus intersects the plane
}

//----------------------------------------------------------------//
u32 USSect::VecToCircle ( float& t0, float& t1, USVec2D& loc, USVec2D& vec, USVec2D& circleLoc, float radius ) {

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
u32 USSect::VecToPlane ( USVec2D& loc, USVec2D& vec, USPlane2D& p, float& t ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::VecToPlane ( USVec3D& loc, USVec3D& vec, USPlane3D& p, float& t ) {

	float d;
	d = vec.Dot ( p.mNorm );
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = ( loc.Dot ( p.mNorm ) + p.mDist ) / -d;
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::VecToSphere ( float& t0, float& t1, USVec3D& loc, USVec3D& vec, USVec3D& sphereLoc, float radius ) {

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
u32 USSect::VecToUnitCircle ( float& t0, float& t1, USVec2D& loc, USVec2D& vec ) {

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
u32 USSect::VecToUnitSphere ( float& t0, float& t1, USVec3D& loc, USVec3D& vec ) {

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
u32 USSect::XAxisToPlane ( float y, USPlane2D& p, float& t ) {

	float d;
	d = p.mNorm.mX;
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = (( p.mNorm.mY * y ) + p.mDist ) / -d;
	
	return SECT_HIT;
}

//----------------------------------------------------------------//
u32 USSect::YAxisToPlane ( float x, USPlane2D& p, float& t ) {

	float d;
	d = p.mNorm.mY;
	if ( d == 0.0f ) return SECT_TANGENT; // ray is parallel
	
	t = (( p.mNorm.mX * x ) + p.mDist ) / -d;
	
	return SECT_HIT;
}
