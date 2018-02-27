// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <zl-util/ZLCurve.h>

//================================================================//
// ZLCubicCurve1D
//================================================================//
	
//----------------------------------------------------------------//
void ZLCubicCurve1D::Init ( ZLReal a, ZLReal b, ZLReal c, ZLReal d ) {
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
ZLReal ZLCatmullRom1D::Evaluate ( ZLReal t ) const {

	const ZLReal &a = this->mA;
	const ZLReal &b = this->mB;
	const ZLReal &c = this->mC;
	const ZLReal &d = this->mD;

	ZLReal t2 = t * t;
	ZLReal t3 = t2 * t;
	
	return 0.5 * (
		( 2.0 * b ) +
		( -a + c ) * t +
		( 2.0 * a - 5.0 * b + 4.0 * c - d ) * t2 +
		( -a + 3.0 * b - 3.0 * c + d ) * t3
	);
}

//================================================================//
// ZLCatmullRom2D
//================================================================//

//----------------------------------------------------------------//
ZLVec2D ZLCatmullRom2D::Evaluate ( ZLReal t ) const {

	const ZLVec2D& p0 = this->mP0;
	const ZLVec2D& p1 = this->mP1;
	const ZLVec2D& p2 = this->mP2;
	const ZLVec2D& p3 = this->mP3;

	ZLVec2D p;

	ZLReal t2 = t * t;
	ZLReal t3 = t2 * t;
	
	p.mX = 0.5 * (
		( 2.0 * p1.mX ) +
		( -p0.mX + p2.mX ) * t +
		( 2.0 * p0.mX - 5.0 * p1.mX + 4.0 * p2.mX - p3.mX ) * t2 +
		( -p0.mX + 3.0 * p1.mX - 3.0 * p2.mX + p3.mX ) * t3
	);
	
	p.mY = 0.5 * (
		( 2.0 * p1.mY ) +
		( -p0.mY + p2.mY ) * t +
		( 2.0 * p0.mY - 5.0 * p1.mY + 4.0 * p2.mY - p3.mY ) * t2 +
		( -p0.mY + 3.0 * p1.mY - 3.0 * p2.mY + p3.mY ) * t3
	);
	
	return p;
}

//================================================================//
// ZLQuadraticBezier2D
//================================================================//

//----------------------------------------------------------------//
ZLVec2D ZLQuadraticBezier2D::Evaluate ( ZLReal t ) const {
	UNUSED ( t );
	ZLVec2D v ( 0.0, 0.0 );
	assert ( false );
	return v;
}
