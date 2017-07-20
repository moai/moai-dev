// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <zl-util/ZLCurve.h>

//================================================================//
// ZLCubicCurve1D
//================================================================//
	
//----------------------------------------------------------------//
void ZLCubicCurve1D::Init ( float a, float b, float c, float d ) {
	this->mA = a;
	this->mB = b;
	this->mC = c;
	this->mD = d;
}

//================================================================//
// ZLCubicCurve2D
//================================================================//
	
//----------------------------------------------------------------//
void ZLCubicCurve2D::Init ( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& p2, const ZLVec2D& p3 ) {
	this->mP0 = p0;
	this->mP1 = p1;
	this->mP2 = p2;
	this->mP3 = p3;
}

//================================================================//
// ZLQuadraticCurve2D
//================================================================//
	
//----------------------------------------------------------------//
void ZLQuadraticCurve2D::Init ( const ZLVec2D& p0, const ZLVec2D& p1, const ZLVec2D& p2 ) {
	this->mP0 = p0;
	this->mP1 = p1;
	this->mP2 = p2;
}

//================================================================//
// ZLCatmullRom1D
//================================================================//

//----------------------------------------------------------------//
float ZLCatmullRom1D::Evaluate ( float t ) const {

	const float &a = this->mA;
	const float &b = this->mB;
	const float &c = this->mC;
	const float &d = this->mD;

	float t2 = t * t;
	float t3 = t2 * t;
	
	return 0.5f * (
		( 2.0f * b ) +
		( -a + c ) * t +
		( 2.0f * a - 5.0f * b + 4.0f * c - d ) * t2 +
		( -a + 3.0f * b - 3.0f * c + d ) * t3
	);
}

//================================================================//
// ZLCatmullRom2D
//================================================================//

//----------------------------------------------------------------//
ZLVec2D ZLCatmullRom2D::Evaluate ( float t ) const {

	const ZLVec2D& p0 = this->mP0;
	const ZLVec2D& p1 = this->mP1;
	const ZLVec2D& p2 = this->mP2;
	const ZLVec2D& p3 = this->mP3;

	ZLVec2D p;

	float t2 = t * t;
	float t3 = t2 * t;
	
	p.mX = 0.5f * (
		( 2.0f * p1.mX ) +
		( -p0.mX + p2.mX ) * t +
		( 2.0f * p0.mX - 5.0f * p1.mX + 4.0f * p2.mX - p3.mX ) * t2 +
		( -p0.mX + 3.0f * p1.mX - 3.0f * p2.mX + p3.mX ) * t3
	);
	
	p.mY = 0.5f * (
		( 2.0f * p1.mY ) +
		( -p0.mY + p2.mY ) * t +
		( 2.0f * p0.mY - 5.0f * p1.mY + 4.0f * p2.mY - p3.mY ) * t2 +
		( -p0.mY + 3.0f * p1.mY - 3.0f * p2.mY + p3.mY ) * t3
	);
	
	return p;
}

//================================================================//
// ZLQuadraticBezier2D
//================================================================//

//----------------------------------------------------------------//
ZLVec2D ZLQuadraticBezier2D::Evaluate ( float t ) const {
	UNUSED ( t );
	ZLVec2D v ( 0.0f, 0.0f );
	assert ( false );
	return v;
}
