// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <uslscore/USCurve.h>

//================================================================//
// USCurve
//================================================================//

float	CatmullRom	( float a, float b, float c, float d, float t );
USVec2D	CatmullRom	( const USVec2D& p0, const USVec2D& p1, const USVec2D& p2, const USVec2D& p3, float t );

//----------------------------------------------------------------//
float CatmullRom ( float a, float b, float c, float d, float t ) {

	float t2 = t * t;
	float t3 = t2 * t;
	
	return 0.5f * (
		( 2.0f * b ) +
		( -a + c ) * t +
		( 2.0f * a - 5.0f * b + 4.0f * c - d ) * t2 +
		( -a + 3.0f * b - 3.0f * c + d ) * t3
	);
}

//----------------------------------------------------------------//
USVec2D CatmullRom ( const USVec2D& p0, const USVec2D& p1, const USVec2D& p2, const USVec2D& p3, float t ) {

	USVec2D p;

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
