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
float USCurve::Bezier1D ( float a, float b, float c, float d, float t ) {
	float t2 = t * t;
	float t3 = t2 * t;
	
	float u = 1 - t;
	float u2 = u * u;
	float u3 = u2 * u;
	
	
	
	return ((u3 * a) +
			(3.0 * t * u2 * b) +
			(3.0 * t2 * u * c) +
			(t3 * d));
}


//----------------------------------------------------------------//
USVec2D USCurve::Bezier2D ( const USVec2D &p0, const USVec2D &p1, const USVec2D &p2, const USVec2D &p3, float t ) {
	
	USVec2D p;
	
	float t2 = t * t;
	float t3 = t2 * t;
	
	float u = 1 - t;
	float u2 = u * u;
	float u3 = u2 * u;
	
	p.mX = ((u3 * p0.mX) +
			(3.0 * t * u2 * p1.mX) +
			(3.0 * t2 * u * p2.mX) +
			(t3 * p3.mX));
	p.mY = ((u3 * p0.mY) +
			(3.0 * t * u2 * p1.mY) +
			(3.0 * t2 * u * p2.mY) +
			(t3 * p3.mY));
	
	return p;
	
}

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

//----------------------------------------------------------------//
float USCurve::CatmullRom1D(float a, float b, float c, float d, float t){
	return CatmullRom(a, b, c, d, t);
}

//----------------------------------------------------------------//
USVec2D USCurve::CatmullRom2D(const USVec2D &p0, const USVec2D &p1, const USVec2D &p2, const USVec2D &p3, float t){
	return CatmullRom(p0, p1, p2, p3, t);
}


//----------------------------------------------------------------//
float USCurve::CardinalSpline1D(float a, float b, float c, float d, float tension, float t){
	float t2 = t * t;
    float t3 = t2 * t;
    
	/*
	 * Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
	 */
    float s = (1 - tension) / 2;
	
    float b1 = s * ((-t3 + (2 * t2)) - t);                      // s(-t3 + 2 t2 - t)P1
    float b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);          // s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    float b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2);      // s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    float b4 = s * (t3 - t2);                                   // s(t3 - t2)P4
	
	return a*b1 + b*b2 + c*b3 + d*b4;
}

//----------------------------------------------------------------//
USVec2D USCurve::CardinalSpline2D(const USVec2D &p0, const USVec2D &p1, const USVec2D &p2, const USVec2D &p3, float tension, float t){
	
	USVec2D p;
	
	float t2 = t * t;
    float t3 = t2 * t;
    
	/*
	 * Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
	 */
    float s = (1 - tension) / 2;
	
    float b1 = s * ((-t3 + (2 * t2)) - t);                      // s(-t3 + 2 t2 - t)P1
    float b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);          // s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    float b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2);      // s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    float b4 = s * (t3 - t2);                                   // s(t3 - t2)P4
	
	p.mX = (p0.mX*b1 + p1.mX*b2 + p2.mX*b3 + p3.mX*b4);
	p.mY = (p0.mY*b1 + p1.mY*b2 + p2.mY*b3 + p3.mY*b4);
	return p;
}


