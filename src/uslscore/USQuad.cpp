// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USQuad.h>

//----------------------------------------------------------------//
bool _intersect ( USVec2D& p0, USVec2D& p1, USVec2D& q0, USVec2D& q1, USVec2D& r );
bool _intersect ( USVec2D& p0, USVec2D& p1, USVec2D& q0, USVec2D& q1, USVec2D& r ) {

	USVec2D u;
	u.mX = p1.mX - p0.mX;
	u.mY = p1.mY - p0.mY;
	
	USVec2D v;
	v.mX = q1.mX - q0.mX;
	v.mY = q1.mY - q0.mY;

	USVec2D w;
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
// USQuad
//================================================================//

//----------------------------------------------------------------//
bool USQuad::IsSeparatingAxis ( const USVec2D& e0, const USVec2D& e1, const USVec2D& e2, const USVec2D& e3 ) const {

	USVec2D axis;
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
bool USQuad::Contains ( float x, float y ) const {
	
	USMetaVec2D < float > vec;
	vec.Init ( x, y );
	
	USVec2D v0 = this->mV [ 0 ];
	USVec2D v1 = this->mV [ 1 ];
	USVec2D v2 = this->mV [ 2 ];
	USVec2D v3 = this->mV [ 3 ];
	
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
USRect USQuad::GetBounds () const {

	USRect rect;

	rect.Init ( this->mV [ 0 ]);
	rect.Grow ( this->mV [ 1 ]);
	rect.Grow ( this->mV [ 2 ]);
	rect.Grow ( this->mV [ 3 ]);
	
	return rect;
}

//----------------------------------------------------------------//
USPlane2D USQuad::GetPlane ( u32 id ) {

	id = id % 4;
	
	USVec2D v0;
	USVec2D v1;
	
	v0 = this->mV [ id++ ];
	v1 = this->mV [ id % 4 ];
	
	USPlane2D plane;
	plane.Init ( v1, v0 );
	return plane;
}

//----------------------------------------------------------------//
USVec2D USQuad::GetVert ( u32 id ) {

	return this->mV [ id % 4 ];
}

//----------------------------------------------------------------//
void USQuad::Init ( const USRect& rect ) {

	this->Init ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void USQuad::Init ( float x0, float y0, float x1, float y1 ) {

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
u32 USQuad::Intersect ( const USQuad& quad, USVec2D* buffer, u32 max ) const {

	u32 total = 0;
	
	// check containment
	for ( u32 i = 0; i < 4; ++i ) {
		USVec2D v = this->mV [ i ];
		if ( quad.Contains ( v.mX, v.mY )) {
			buffer [ total++ ] = v;
			if ( total == max ) return total;
		}
	}
	
	// check containment
	for ( u32 i = 0; i < 4; ++i ) {
		USVec2D v = quad.mV [ i ];
		if ( this->Contains ( v.mX, v.mY )) {
			buffer [ total++ ] = v;
			if ( total == max ) return total;
		}
	}
	
	// check intersection
	for ( u32 i = 0; i < 4; ++i ) {
		
		USVec2D p0 = this->mV [ i ];
		USVec2D p1 = this->mV [( i + 1 ) & 0x03 ];
		
		for ( u32 j = 0; j < 4; ++j ) {
			
			USVec2D q0 = quad.mV [ j ];
			USVec2D q1 = quad.mV [( j + 1 ) & 0x03 ];
			
			USVec2D r;
			if ( _intersect ( p0, p1, q0, q1, r )) {
				buffer [ total++ ] = r;
				if ( total == max ) return total;
			}
		}
	}
	
	return total;
}

//----------------------------------------------------------------//
bool USQuad::Intersect ( const USQuad& quad, USRect& result ) const {

	USVec2D buffer [ 8 ];
	
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
bool USQuad::Intersect ( const USRect& rect, USRect& result ) const {

	USQuad quad;
	quad.Init ( rect );
	return this->Intersect ( quad, result );
}

//----------------------------------------------------------------//
bool USQuad::Overlap ( const USQuad& quad ) const {
	
	if ( quad.IsSeparatingAxis ( this->mV [ 0 ], this->mV [ 1 ], this->mV [ 2 ], this->mV [ 3 ] )) return false;
	if ( quad.IsSeparatingAxis ( this->mV [ 1 ], this->mV [ 2 ], this->mV [ 3 ], this->mV [ 0 ] )) return false;
	if ( quad.IsSeparatingAxis ( this->mV [ 2 ], this->mV [ 3 ], this->mV [ 0 ], this->mV [ 1 ] )) return false;
	if ( quad.IsSeparatingAxis ( this->mV [ 3 ], this->mV [ 0 ], this->mV [ 1 ], this->mV [ 2 ] )) return false;
	
	return true;
}

//----------------------------------------------------------------//
bool USQuad::Overlap ( const USRect& rect ) const {

	if (( this->mV [ 0 ].mX < rect.mXMin ) && ( this->mV [ 1 ].mX < rect.mXMin ) && ( this->mV [ 2 ].mX < rect.mXMin ) && ( this->mV [ 3 ].mX < rect.mXMin )) return false;
	if (( this->mV [ 0 ].mY < rect.mYMin ) && ( this->mV [ 1 ].mY < rect.mYMin ) && ( this->mV [ 2 ].mY < rect.mYMin ) && ( this->mV [ 3 ].mY < rect.mYMin )) return false;
	if (( this->mV [ 0 ].mX > rect.mXMax ) && ( this->mV [ 1 ].mX > rect.mXMax ) && ( this->mV [ 2 ].mX > rect.mXMax ) && ( this->mV [ 3 ].mX > rect.mXMax )) return false;
	if (( this->mV [ 0 ].mY > rect.mYMax ) && ( this->mV [ 1 ].mY > rect.mYMax ) && ( this->mV [ 2 ].mY > rect.mYMax ) && ( this->mV [ 3 ].mY > rect.mYMax )) return false;

	return true;
}

//----------------------------------------------------------------//
void USQuad::Scale ( float xScale, float yScale ) {

	this->mV [ 0 ].Scale ( xScale, yScale );
	this->mV [ 1 ].Scale ( xScale, yScale );
	this->mV [ 2 ].Scale ( xScale, yScale );
	this->mV [ 3 ].Scale ( xScale, yScale );
}

//----------------------------------------------------------------//
void USQuad::ReverseWinding () {

	USVec2D v [ 4 ];
	
	v [ 0 ] = this->mV [ 3 ];
	v [ 1 ] = this->mV [ 2 ];
	v [ 2 ] = this->mV [ 1 ];
	v [ 3 ] = this->mV [ 0 ];
	
	memcpy ( this->mV, v, sizeof ( v ));
}

//----------------------------------------------------------------//
void USQuad::Transform ( const USAffine2D& transform ) {
	
	float x;
	float y;
	
	const float* m = transform.m;
	
	x =	( m[ USAffine2D::C0_R0 ] * this->mV [ 0 ].mX ) + ( m[ USAffine2D::C1_R0 ] * this->mV [ 0 ].mY ) + ( m[ USAffine2D::C2_R0 ]);
	y =	( m[ USAffine2D::C0_R1 ] * this->mV [ 0 ].mX ) + ( m[ USAffine2D::C1_R1 ] * this->mV [ 0 ].mY ) + ( m[ USAffine2D::C2_R1 ]);
	
	this->mV [ 0 ].mX = x;
	this->mV [ 0 ].mY = y;
	
	x =	( m[ USAffine2D::C0_R0 ] * this->mV [ 1 ].mX ) + ( m[ USAffine2D::C1_R0 ] * this->mV [ 1 ].mY ) + ( m[ USAffine2D::C2_R0 ]);
	y =	( m[ USAffine2D::C0_R1 ] * this->mV [ 1 ].mX ) + ( m[ USAffine2D::C1_R1 ] * this->mV [ 1 ].mY ) + ( m[ USAffine2D::C2_R1 ]);
	
	this->mV [ 1 ].mX = x;
	this->mV [ 1 ].mY = y;
	
	x =	( m[ USAffine2D::C0_R0 ] * this->mV [ 2 ].mX ) + ( m[ USAffine2D::C1_R0 ] * this->mV [ 2 ].mY ) + ( m[ USAffine2D::C2_R0 ]);
	y =	( m[ USAffine2D::C0_R1 ] * this->mV [ 2 ].mX ) + ( m[ USAffine2D::C1_R1 ] * this->mV [ 2 ].mY ) + ( m[ USAffine2D::C2_R1 ]);
	
	this->mV [ 2 ].mX = x;
	this->mV [ 2 ].mY = y;
	
	x =	( m[ USAffine2D::C0_R0 ] * this->mV [ 3 ].mX ) + ( m[ USAffine2D::C1_R0 ] * this->mV [ 3 ].mY ) + ( m[ USAffine2D::C2_R0 ]);
	y =	( m[ USAffine2D::C0_R1 ] * this->mV [ 3 ].mX ) + ( m[ USAffine2D::C1_R1 ] * this->mV [ 3 ].mY ) + ( m[ USAffine2D::C2_R1 ]);
	
	this->mV [ 3 ].mX = x;
	this->mV [ 3 ].mY = y;
}

//----------------------------------------------------------------//
void USQuad::Transform ( const USAffine3D& transform ) {
	
	float x;
	float y;
	
	const float* m = transform.m;
	
	x =	( m[ USAffine3D::C0_R0 ] * this->mV [ 0 ].mX ) + ( m[ USAffine3D::C1_R0 ] * this->mV [ 0 ].mY ) + ( m[ USAffine3D::C3_R0 ]);
	y =	( m[ USAffine3D::C0_R1 ] * this->mV [ 0 ].mX ) + ( m[ USAffine3D::C1_R1 ] * this->mV [ 0 ].mY ) + ( m[ USAffine3D::C3_R1 ]);
	
	this->mV [ 0 ].mX = x;
	this->mV [ 0 ].mY = y;
	
	x =	( m[ USAffine3D::C0_R0 ] * this->mV [ 1 ].mX ) + ( m[ USAffine3D::C1_R0 ] * this->mV [ 1 ].mY ) + ( m[ USAffine3D::C3_R0 ]);
	y =	( m[ USAffine3D::C0_R1 ] * this->mV [ 1 ].mX ) + ( m[ USAffine3D::C1_R1 ] * this->mV [ 1 ].mY ) + ( m[ USAffine3D::C3_R1 ]);
	
	this->mV [ 1 ].mX = x;
	this->mV [ 1 ].mY = y;
	
	x =	( m[ USAffine3D::C0_R0 ] * this->mV [ 2 ].mX ) + ( m[ USAffine3D::C1_R0 ] * this->mV [ 2 ].mY ) + ( m[ USAffine3D::C3_R0 ]);
	y =	( m[ USAffine3D::C0_R1 ] * this->mV [ 2 ].mX ) + ( m[ USAffine3D::C1_R1 ] * this->mV [ 2 ].mY ) + ( m[ USAffine3D::C3_R1 ]);
	
	this->mV [ 2 ].mX = x;
	this->mV [ 2 ].mY = y;
	
	x =	( m[ USAffine3D::C0_R0 ] * this->mV [ 3 ].mX ) + ( m[ USAffine3D::C1_R0 ] * this->mV [ 3 ].mY ) + ( m[ USAffine3D::C3_R0 ]);
	y =	( m[ USAffine3D::C0_R1 ] * this->mV [ 3 ].mX ) + ( m[ USAffine3D::C1_R1 ] * this->mV [ 3 ].mY ) + ( m[ USAffine3D::C3_R1 ]);
	
	this->mV [ 3 ].mX = x;
	this->mV [ 3 ].mY = y;
}

//----------------------------------------------------------------//
void USQuad::Transform ( const USMatrix3x3& transform ) {
	
	float x;
	float y;
	
	const float* m = transform.m;
	
	x =	( m[ USMatrix3x3::C0_R0 ] * this->mV [ 0 ].mX ) + ( m[ USMatrix3x3::C1_R0 ] * this->mV [ 0 ].mY ) + ( m[ USMatrix3x3::C2_R0 ]);
	y =	( m[ USMatrix3x3::C0_R1 ] * this->mV [ 0 ].mX ) + ( m[ USMatrix3x3::C1_R1 ] * this->mV [ 0 ].mY ) + ( m[ USMatrix3x3::C2_R1 ]);
	
	this->mV [ 0 ].mX = x;
	this->mV [ 0 ].mY = y;
	
	x =	( m[ USMatrix3x3::C0_R0 ] * this->mV [ 1 ].mX ) + ( m[ USMatrix3x3::C1_R0 ] * this->mV [ 1 ].mY ) + ( m[ USMatrix3x3::C2_R0 ]);
	y =	( m[ USMatrix3x3::C0_R1 ] * this->mV [ 1 ].mX ) + ( m[ USMatrix3x3::C1_R1 ] * this->mV [ 1 ].mY ) + ( m[ USMatrix3x3::C2_R1 ]);
	
	this->mV [ 1 ].mX = x;
	this->mV [ 1 ].mY = y;
	
	x =	( m[ USMatrix3x3::C0_R0 ] * this->mV [ 2 ].mX ) + ( m[ USMatrix3x3::C1_R0 ] * this->mV [ 2 ].mY ) + ( m[ USMatrix3x3::C2_R0 ]);
	y =	( m[ USMatrix3x3::C0_R1 ] * this->mV [ 2 ].mX ) + ( m[ USMatrix3x3::C1_R1 ] * this->mV [ 2 ].mY ) + ( m[ USMatrix3x3::C2_R1 ]);
	
	this->mV [ 2 ].mX = x;
	this->mV [ 2 ].mY = y;
	
	x =	( m[ USMatrix3x3::C0_R0 ] * this->mV [ 3 ].mX ) + ( m[ USMatrix3x3::C1_R0 ] * this->mV [ 3 ].mY ) + ( m[ USMatrix3x3::C2_R0 ]);
	y =	( m[ USMatrix3x3::C0_R1 ] * this->mV [ 3 ].mX ) + ( m[ USMatrix3x3::C1_R1 ] * this->mV [ 3 ].mY ) + ( m[ USMatrix3x3::C2_R1 ]);
	
	this->mV [ 3 ].mX = x;
	this->mV [ 3 ].mY = y;
}

//----------------------------------------------------------------//
void USQuad::Translate ( float xOff, float yOff ) {

	USVec2D offset ( xOff, yOff );

	this->mV [ 0 ].Add ( offset );
	this->mV [ 1 ].Add ( offset );
	this->mV [ 2 ].Add ( offset );
	this->mV [ 3 ].Add ( offset );
}