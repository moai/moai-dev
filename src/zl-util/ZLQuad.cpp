// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBarycentric.h>
#include <zl-util/ZLQuad.h>

//----------------------------------------------------------------//
bool _intersect ( ZLVec2D& p0, ZLVec2D& p1, ZLVec2D& q0, ZLVec2D& q1, ZLVec2D& r );
bool _intersect ( ZLVec2D& p0, ZLVec2D& p1, ZLVec2D& q0, ZLVec2D& q1, ZLVec2D& r ) {

	ZLVec2D u;
	u.mX = p1.mX - p0.mX;
	u.mY = p1.mY - p0.mY;
	
	ZLVec2D v;
	v.mX = q1.mX - q0.mX;
	v.mY = q1.mY - q0.mY;

	ZLVec2D w;
	w.mX = p0.mX - q0.mX;
	w.mY = p0.mY - q0.mY;
	
	float sN = ( v.mY * w.mX ) - ( v.mX * w.mY );
	float sD = ( v.mX * u.mY ) - ( v.mY * u.mX );
	
	if ( sD == 0.0f ) return false;
	
	float s = sN / sD;
	
	if (( s < 0.0f ) || ( s > 1.0f )) return false;
	
	float tN = ( u.mX * w.mY ) - ( u.mY * w.mX );
	float t = tN / -sD;
	
	if (( t < 0.0f ) || ( t > 1.0f )) return false;
	
	r.mX = p0.mX + ( u.mX * s );
	r.mY = p0.mY + ( u.mY * s );
	
	return true;
}

//================================================================//
// ZLQuad
//================================================================//

//----------------------------------------------------------------//
bool ZLQuad::IsSeparatingAxis ( const ZLVec2D& e0, const ZLVec2D& e1, const ZLVec2D& e2, const ZLVec2D& e3 ) const {

	ZLVec2D axis;
	axis.mX = e1.mY - e0.mY;
	axis.mY = -( e1.mX - e0.mX );
	
	float min = 0.0f;
	float max = ( axis.mY * axis.mX ) + ( axis.mY * axis.mY );
	
	float dot;
	
	dot = ( axis.mX * ( e2.mX - e0.mX )) + ( axis.mY * ( e2.mY - e0.mY ));
	if ( dot < min ) min = dot;
	if ( dot > max ) max = dot;
	
	dot = ( axis.mX * ( e3.mX - e0.mX )) + ( axis.mY * ( e3.mY - e0.mY ));
	if ( dot < min ) min = dot;
	if ( dot > max ) max = dot;
	
	dot = ( axis.mX * ( this->mV [ 0 ].mX - e0.mX )) + ( axis.mY * ( this->mV [ 0 ].mY - e0.mY ));
	if (( dot >= min ) && ( dot <= max )) return false;
	
	dot = ( axis.mX * ( this->mV [ 1 ].mX - e0.mX )) + ( axis.mY * ( this->mV [ 1 ].mY - e0.mY ));
	if (( dot >= min ) && ( dot <= max )) return false;
	
	dot = ( axis.mX * ( this->mV [ 2 ].mX - e0.mX )) + ( axis.mY * ( this->mV [ 2 ].mY - e0.mY ));
	if (( dot >= min ) && ( dot <= max )) return false;
	
	dot = ( axis.mX * ( this->mV [ 3 ].mX - e0.mX )) + ( axis.mY * ( this->mV [ 3 ].mY - e0.mY ));
	if (( dot >= min ) && ( dot <= max )) return false;
	
	return true;
}

//----------------------------------------------------------------//
bool ZLQuad::Contains ( float x, float y ) const {
	
	ZLMetaVec2D < float > vec;
	vec.Init ( x, y );
	
	ZLVec2D v0 = this->mV [ 0 ];
	ZLVec2D v1 = this->mV [ 1 ];
	ZLVec2D v2 = this->mV [ 2 ];
	ZLVec2D v3 = this->mV [ 3 ];
	
	v0.Sub ( vec );
	v1.Sub ( vec );
	v2.Sub ( vec );
	v3.Sub ( vec );
	
	u32 mask = 0;
	mask |= ( v0.Cross ( v1 ) > 0.0 ) ? ( 1 << 0 ) : 0;
	mask |= ( v1.Cross ( v2 ) > 0.0 ) ? ( 1 << 1 ) : 0;
	mask |= ( v2.Cross ( v3 ) > 0.0 ) ? ( 1 << 2 ) : 0;
	mask |= ( v3.Cross ( v0 ) > 0.0 ) ? ( 1 << 3 ) : 0;
	
	return (( mask == 0x0f ) || ( mask == 0 ));
}

//----------------------------------------------------------------//
ZLRect ZLQuad::GetBounds () const {

	ZLRect rect;

	rect.Init ( this->mV [ 0 ]);
	rect.Grow ( this->mV [ 1 ]);
	rect.Grow ( this->mV [ 2 ]);
	rect.Grow ( this->mV [ 3 ]);
	
	return rect;
}

//----------------------------------------------------------------//
ZLPlane2D ZLQuad::GetPlane ( u32 id ) {

	id = id % 4;
	
	ZLVec2D v0;
	ZLVec2D v1;
	
	v0 = this->mV [ id++ ];
	v1 = this->mV [ id % 4 ];
	
	ZLPlane2D plane;
	plane.Init ( v1, v0 );
	return plane;
}

//----------------------------------------------------------------//
ZLVec2D ZLQuad::GetVert ( u32 id ) {

	return this->mV [ id % 4 ];
}

//----------------------------------------------------------------//
void ZLQuad::Init ( const ZLRect& rect ) {

	this->Init ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void ZLQuad::Init ( float x0, float y0, float x1, float y1 ) {

	// left top
	this->mV [ 0 ].mX = x0;
	this->mV [ 0 ].mY = y1;
	
	// right top
	this->mV [ 1 ].mX = x1;
	this->mV [ 1 ].mY = y1;
	
	// right bottom
	this->mV [ 2 ].mX = x1;
	this->mV [ 2 ].mY = y0;
	
	// left bottom
	this->mV [ 3 ].mX = x0;
	this->mV [ 3 ].mY = y0;
}

//----------------------------------------------------------------//
u32 ZLQuad::Intersect ( const ZLQuad& quad, ZLVec2D* buffer, u32 max ) const {

	u32 total = 0;
	
	// check containment
	for ( u32 i = 0; i < 4; ++i ) {
		ZLVec2D v = this->mV [ i ];
		if ( quad.Contains ( v.mX, v.mY )) {
			buffer [ total++ ] = v;
			if ( total == max ) return total;
		}
	}
	
	// check containment
	for ( u32 i = 0; i < 4; ++i ) {
		ZLVec2D v = quad.mV [ i ];
		if ( this->Contains ( v.mX, v.mY )) {
			buffer [ total++ ] = v;
			if ( total == max ) return total;
		}
	}
	
	// check intersection
	for ( u32 i = 0; i < 4; ++i ) {
		
		ZLVec2D p0 = this->mV [ i ];
		ZLVec2D p1 = this->mV [( i + 1 ) & 0x03 ];
		
		for ( u32 j = 0; j < 4; ++j ) {
			
			ZLVec2D q0 = quad.mV [ j ];
			ZLVec2D q1 = quad.mV [( j + 1 ) & 0x03 ];
			
			ZLVec2D r;
			if ( _intersect ( p0, p1, q0, q1, r )) {
				buffer [ total++ ] = r;
				if ( total == max ) return total;
			}
		}
	}
	
	return total;
}

//----------------------------------------------------------------//
bool ZLQuad::Intersect ( const ZLQuad& quad, ZLRect& result ) const {

	ZLVec2D buffer [ 8 ];
	
	u32 total = this->Intersect ( quad, buffer, 8 );
	
	if ( total ) {
		
		result.Init ( buffer [ 0 ]);
	
		for ( u32 i = 1; i < total; ++i ) {
			result.Grow ( buffer [ i ]);
		}
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLQuad::Intersect ( const ZLRect& rect, ZLRect& result ) const {

	ZLQuad quad;
	quad.Init ( rect );
	return this->Intersect ( quad, result );
}

//----------------------------------------------------------------//
bool ZLQuad::Overlap ( const ZLQuad& quad ) const {
	
	if ( quad.IsSeparatingAxis ( this->mV [ 0 ], this->mV [ 1 ], this->mV [ 2 ], this->mV [ 3 ] )) return false;
	if ( quad.IsSeparatingAxis ( this->mV [ 1 ], this->mV [ 2 ], this->mV [ 3 ], this->mV [ 0 ] )) return false;
	if ( quad.IsSeparatingAxis ( this->mV [ 2 ], this->mV [ 3 ], this->mV [ 0 ], this->mV [ 1 ] )) return false;
	if ( quad.IsSeparatingAxis ( this->mV [ 3 ], this->mV [ 0 ], this->mV [ 1 ], this->mV [ 2 ] )) return false;
	
	return true;
}

//----------------------------------------------------------------//
bool ZLQuad::Overlap ( const ZLRect& rect ) const {

	if (( this->mV [ 0 ].mX < rect.mXMin ) && ( this->mV [ 1 ].mX < rect.mXMin ) && ( this->mV [ 2 ].mX < rect.mXMin ) && ( this->mV [ 3 ].mX < rect.mXMin )) return false;
	if (( this->mV [ 0 ].mY < rect.mYMin ) && ( this->mV [ 1 ].mY < rect.mYMin ) && ( this->mV [ 2 ].mY < rect.mYMin ) && ( this->mV [ 3 ].mY < rect.mYMin )) return false;
	if (( this->mV [ 0 ].mX > rect.mXMax ) && ( this->mV [ 1 ].mX > rect.mXMax ) && ( this->mV [ 2 ].mX > rect.mXMax ) && ( this->mV [ 3 ].mX > rect.mXMax )) return false;
	if (( this->mV [ 0 ].mY > rect.mYMax ) && ( this->mV [ 1 ].mY > rect.mYMax ) && ( this->mV [ 2 ].mY > rect.mYMax ) && ( this->mV [ 3 ].mY > rect.mYMax )) return false;

	return true;
}

//----------------------------------------------------------------//
void ZLQuad::Scale ( float xScale, float yScale ) {

	this->mV [ 0 ].Scale ( xScale, yScale );
	this->mV [ 1 ].Scale ( xScale, yScale );
	this->mV [ 2 ].Scale ( xScale, yScale );
	this->mV [ 3 ].Scale ( xScale, yScale );
}

//----------------------------------------------------------------//
bool ZLQuad::RemapCoord ( const ZLQuad& src, const ZLQuad& dest, u32 triangleID, float x, float y, ZLVec2D& result ) {

	ZLVec3D bary;
	ZLVec2D cart ( x, y );
	
	u32 i0 = 0;
	u32 i1 = 1;
	u32 i2 = 2;
	
	if ( triangleID & 0x01 ) {
		i0 = 0;
		i1 = 2;
		i2 = 3;
	}
	
	bary = ZLBarycentric::CartesianToBarycentric ( src.mV [ i0 ], src.mV [ i1 ], src.mV [ i2 ], cart );
	result = ZLBarycentric::BarycentricToCartesian ( dest.mV [ i0 ], dest.mV [ i1 ], dest.mV [ i2 ], bary );
	return ZLBarycentric::BarycentricIsInside ( bary );
}

//----------------------------------------------------------------//
void ZLQuad::ReverseWinding () {

	ZLVec2D v [ 4 ];
	
	v [ 0 ] = this->mV [ 3 ];
	v [ 1 ] = this->mV [ 2 ];
	v [ 2 ] = this->mV [ 1 ];
	v [ 3 ] = this->mV [ 0 ];
	
	memcpy ( this->mV, v, sizeof ( v ));
}

//----------------------------------------------------------------//
void ZLQuad::Transform ( const ZLAffine2D& transform ) {
	
	float x;
	float y;
	
	const float* m = transform.m;
	
	x =	( m[ ZLAffine2D::C0_R0 ] * this->mV [ 0 ].mX ) + ( m[ ZLAffine2D::C1_R0 ] * this->mV [ 0 ].mY ) + ( m[ ZLAffine2D::C2_R0 ]);
	y =	( m[ ZLAffine2D::C0_R1 ] * this->mV [ 0 ].mX ) + ( m[ ZLAffine2D::C1_R1 ] * this->mV [ 0 ].mY ) + ( m[ ZLAffine2D::C2_R1 ]);
	
	this->mV [ 0 ].mX = x;
	this->mV [ 0 ].mY = y;
	
	x =	( m[ ZLAffine2D::C0_R0 ] * this->mV [ 1 ].mX ) + ( m[ ZLAffine2D::C1_R0 ] * this->mV [ 1 ].mY ) + ( m[ ZLAffine2D::C2_R0 ]);
	y =	( m[ ZLAffine2D::C0_R1 ] * this->mV [ 1 ].mX ) + ( m[ ZLAffine2D::C1_R1 ] * this->mV [ 1 ].mY ) + ( m[ ZLAffine2D::C2_R1 ]);
	
	this->mV [ 1 ].mX = x;
	this->mV [ 1 ].mY = y;
	
	x =	( m[ ZLAffine2D::C0_R0 ] * this->mV [ 2 ].mX ) + ( m[ ZLAffine2D::C1_R0 ] * this->mV [ 2 ].mY ) + ( m[ ZLAffine2D::C2_R0 ]);
	y =	( m[ ZLAffine2D::C0_R1 ] * this->mV [ 2 ].mX ) + ( m[ ZLAffine2D::C1_R1 ] * this->mV [ 2 ].mY ) + ( m[ ZLAffine2D::C2_R1 ]);
	
	this->mV [ 2 ].mX = x;
	this->mV [ 2 ].mY = y;
	
	x =	( m[ ZLAffine2D::C0_R0 ] * this->mV [ 3 ].mX ) + ( m[ ZLAffine2D::C1_R0 ] * this->mV [ 3 ].mY ) + ( m[ ZLAffine2D::C2_R0 ]);
	y =	( m[ ZLAffine2D::C0_R1 ] * this->mV [ 3 ].mX ) + ( m[ ZLAffine2D::C1_R1 ] * this->mV [ 3 ].mY ) + ( m[ ZLAffine2D::C2_R1 ]);
	
	this->mV [ 3 ].mX = x;
	this->mV [ 3 ].mY = y;
}

//----------------------------------------------------------------//
void ZLQuad::Transform ( const ZLAffine3D& transform ) {
	
	float x;
	float y;
	
	const float* m = transform.m;
	
	x =	( m[ ZLAffine3D::C0_R0 ] * this->mV [ 0 ].mX ) + ( m[ ZLAffine3D::C1_R0 ] * this->mV [ 0 ].mY ) + ( m[ ZLAffine3D::C3_R0 ]);
	y =	( m[ ZLAffine3D::C0_R1 ] * this->mV [ 0 ].mX ) + ( m[ ZLAffine3D::C1_R1 ] * this->mV [ 0 ].mY ) + ( m[ ZLAffine3D::C3_R1 ]);
	
	this->mV [ 0 ].mX = x;
	this->mV [ 0 ].mY = y;
	
	x =	( m[ ZLAffine3D::C0_R0 ] * this->mV [ 1 ].mX ) + ( m[ ZLAffine3D::C1_R0 ] * this->mV [ 1 ].mY ) + ( m[ ZLAffine3D::C3_R0 ]);
	y =	( m[ ZLAffine3D::C0_R1 ] * this->mV [ 1 ].mX ) + ( m[ ZLAffine3D::C1_R1 ] * this->mV [ 1 ].mY ) + ( m[ ZLAffine3D::C3_R1 ]);
	
	this->mV [ 1 ].mX = x;
	this->mV [ 1 ].mY = y;
	
	x =	( m[ ZLAffine3D::C0_R0 ] * this->mV [ 2 ].mX ) + ( m[ ZLAffine3D::C1_R0 ] * this->mV [ 2 ].mY ) + ( m[ ZLAffine3D::C3_R0 ]);
	y =	( m[ ZLAffine3D::C0_R1 ] * this->mV [ 2 ].mX ) + ( m[ ZLAffine3D::C1_R1 ] * this->mV [ 2 ].mY ) + ( m[ ZLAffine3D::C3_R1 ]);
	
	this->mV [ 2 ].mX = x;
	this->mV [ 2 ].mY = y;
	
	x =	( m[ ZLAffine3D::C0_R0 ] * this->mV [ 3 ].mX ) + ( m[ ZLAffine3D::C1_R0 ] * this->mV [ 3 ].mY ) + ( m[ ZLAffine3D::C3_R0 ]);
	y =	( m[ ZLAffine3D::C0_R1 ] * this->mV [ 3 ].mX ) + ( m[ ZLAffine3D::C1_R1 ] * this->mV [ 3 ].mY ) + ( m[ ZLAffine3D::C3_R1 ]);
	
	this->mV [ 3 ].mX = x;
	this->mV [ 3 ].mY = y;
}

//----------------------------------------------------------------//
void ZLQuad::Transform ( const ZLMatrix3x3& transform ) {
	
	float x;
	float y;
	
	const float* m = transform.m;
	
	x =	( m[ ZLMatrix3x3::C0_R0 ] * this->mV [ 0 ].mX ) + ( m[ ZLMatrix3x3::C1_R0 ] * this->mV [ 0 ].mY ) + ( m[ ZLMatrix3x3::C2_R0 ]);
	y =	( m[ ZLMatrix3x3::C0_R1 ] * this->mV [ 0 ].mX ) + ( m[ ZLMatrix3x3::C1_R1 ] * this->mV [ 0 ].mY ) + ( m[ ZLMatrix3x3::C2_R1 ]);
	
	this->mV [ 0 ].mX = x;
	this->mV [ 0 ].mY = y;
	
	x =	( m[ ZLMatrix3x3::C0_R0 ] * this->mV [ 1 ].mX ) + ( m[ ZLMatrix3x3::C1_R0 ] * this->mV [ 1 ].mY ) + ( m[ ZLMatrix3x3::C2_R0 ]);
	y =	( m[ ZLMatrix3x3::C0_R1 ] * this->mV [ 1 ].mX ) + ( m[ ZLMatrix3x3::C1_R1 ] * this->mV [ 1 ].mY ) + ( m[ ZLMatrix3x3::C2_R1 ]);
	
	this->mV [ 1 ].mX = x;
	this->mV [ 1 ].mY = y;
	
	x =	( m[ ZLMatrix3x3::C0_R0 ] * this->mV [ 2 ].mX ) + ( m[ ZLMatrix3x3::C1_R0 ] * this->mV [ 2 ].mY ) + ( m[ ZLMatrix3x3::C2_R0 ]);
	y =	( m[ ZLMatrix3x3::C0_R1 ] * this->mV [ 2 ].mX ) + ( m[ ZLMatrix3x3::C1_R1 ] * this->mV [ 2 ].mY ) + ( m[ ZLMatrix3x3::C2_R1 ]);
	
	this->mV [ 2 ].mX = x;
	this->mV [ 2 ].mY = y;
	
	x =	( m[ ZLMatrix3x3::C0_R0 ] * this->mV [ 3 ].mX ) + ( m[ ZLMatrix3x3::C1_R0 ] * this->mV [ 3 ].mY ) + ( m[ ZLMatrix3x3::C2_R0 ]);
	y =	( m[ ZLMatrix3x3::C0_R1 ] * this->mV [ 3 ].mX ) + ( m[ ZLMatrix3x3::C1_R1 ] * this->mV [ 3 ].mY ) + ( m[ ZLMatrix3x3::C2_R1 ]);
	
	this->mV [ 3 ].mX = x;
	this->mV [ 3 ].mY = y;
}

//----------------------------------------------------------------//
void ZLQuad::Translate ( float xOff, float yOff ) {

	ZLVec2D offset ( xOff, yOff );

	this->mV [ 0 ].Add ( offset );
	this->mV [ 1 ].Add ( offset );
	this->mV [ 2 ].Add ( offset );
	this->mV [ 3 ].Add ( offset );
}