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
// ZLCubicBezier2D
//================================================================//

//----------------------------------------------------------------//
float ZLCubicBezier2D::Angle () {
	// Compute angle between (p0,p1) and (p2,p3)
        
	float dx0 = this->mP1.mX - this->mP0.mX;
	float dy0 = this->mP1.mY - this->mP0.mY;
	float dx1 = this->mP3.mX - this->mP2.mX;
	float dy1 = this->mP3.mY - this->mP2.mY;
	
	float angle = ( float )atan2 ( ABS (( dx0 * dy1 ) - ( dy0 * dx1 )), ( dx0 * dx1 ) + ( dy0 * dy1 ));
	return angle;
}

//----------------------------------------------------------------//
void ZLCubicBezier2D::Bless () {

	//if ( this->mP0.Equals ( this->mP1 )) this->mP1 = this->mP2;
	//if ( this->mP3.Equals ( this->mP2 )) this->mP2 = this->mP1;

	if ( this->mP0.Equals ( this->mP1 )) this->mP1 = this->mP2;
	if ( this->mP1.Equals ( this->mP2 )) this->mP2 = this->mP3;
	if ( this->mP0.Equals ( this->mP1 )) this->mP1 = this->mP2;
}

//----------------------------------------------------------------//
ZLVec2D ZLCubicBezier2D::Evaluate ( float t ) const {
	
	float tM = 1.0f - t;
	
	float b = tM * tM;
	float c = t * t;
	float d = c * t;
	float a = b * tM;
	
	b *= 3.0f * t;
	c *= 3.0f * tM;
	
	ZLVec2D v;
	
	v.mX = a * this->mP0.mX + b * this->mP1.mX + c * this->mP2.mX + d * this->mP3.mX;
	v.mY = a * this->mP0.mY + b * this->mP1.mY + c * this->mP2.mY + d * this->mP3.mY;
	
	return v;
}

//----------------------------------------------------------------//
void ZLCubicBezier2D::FindInflectionDomain ( float t, float& t0, float& t1, float flatness ) const {

	// for a given inflection point, find the domain around the point where
	// the curve is 'flat' and can be approximated by a line
    
	const float EPSILON = 1e-5f;
	const float POWER = 1.0f/3.0f;
    
	t0 = t;
	t1 = 1;

	// TODO: need to use split instead of segmetn since we don't care about left?
	ZLCubicBezier2D right;

	// why not just use segment for both?
	if ( fabs ( t - 1.0 ) > EPSILON ) {
		ZLCubicBezier2D left;
		this->Split ( t, left, right );
	}
	else {
		right = this->Split ( 1.0f, 2.0f );
	}

	float dx = right.mP1.mX - right.mP0.mX;
	float dy = right.mP1.mY - right.mP0.mY;
	
	float norm = Sqrt (( dx * dx ) + ( dy * dy ));
    
	if ( norm != 0.0f ) {
		
		float s4 = ( float )fabs ((( this->mP3.mX - this->mP0.mX ) * dy ) - ((  this->mP3.mY -  this->mP0.mY ) * dx )) / norm;
		float tf = ( float )pow ( flatness / s4, POWER );
		
		if ( ABS ( t - 1.0f ) > EPSILON ) {
			t0 = t - tf * ( 1.0f - t );
			t1 = t + tf * ( 1.0f - t );
		}
		else {
			t0 = 1.0f - ( float )( EPSILON * tf );
			t1 = 1.0f + ( float )( EPSILON * tf );
		}
	}
}

//----------------------------------------------------------------//
u32 ZLCubicBezier2D::FindInflections ( float& t0, float& t1 ) const {

	const ZLVec2D& p0 = this->mP0;
	const ZLVec2D& p1 = this->mP1;
	const ZLVec2D& p2 = this->mP2;
	const ZLVec2D& p3 = this->mP3;

	// math from 'Precise Flattening of Cubic Bezier Segments'
	// ~ Hain, Ahmad, Langan

	/// A, B, C - first three coefficients of f(t) via the Bezier matrix

	float ax = -p0.mX + ( 3.0f * p1.mX ) - ( 3.0f * p2.mX ) + p3.mX;
	float ay = -p0.mY + ( 3.0f * p1.mY ) - ( 3.0f * p2.mY ) + p3.mY;
	
	float bx = ( 3.0f * p0.mX ) - ( 6.0f * p1.mX ) + ( 3.0f * p2.mX );
	float by = ( 3.0f * p0.mY ) - ( 6.0f * p1.mY ) + ( 3.0f * p2.mY );
	
	float cx = -( 3.0f * p0.mX ) + ( 3.0f * p1.mX );
	float cy = -( 3.0f * p0.mY ) + ( 3.0f * p1.mY );

	// At inflection points the component of the acceleration (second
	// derivative of position) perpendicular to the velocity (first
	// derivative of position) is zero; the cross product of the
	// two vectors is zero. Thus:
	
	// 6*(ay*bx-ax*by)*t^2 + 6*(ay*cx-ax*cy)*t + 2*(by*cx-bx*cy)
	
	// Solving for this quadratic equation yields the parametric
	// positions t0 and t1 of the inflection points, if they exist.

	float ab = ( ay * bx ) - ( ax * by );
	float ac = ( ay * cx ) - ( ax * cy );
	float bc = ( by * cx ) - ( bx * cy );
	
	// Dont want a straight line if P0.x == P1.x or P0.y == P1.y
	if ( cx == 0.0f || cy == 0.0f ) {
		return NONE;
	}
	
	if (( ac == 0.0f ) || ( bc == 0.0f )) {
		return DEGENERATE;
	}

	if ( ab == 0.0f ) {
		t0 = -bc / ( 3.0f * ac );
		return ONE_INFLECTION;
	}
	
	float tcusp = -0.5f * ( ac / ab );
	float d = ( tcusp * tcusp ) - ( bc / ( 3.0f * ab ));

	if ( d > 0.0f ) {
	
		d = Sqrt ( d );
		
		t0 = tcusp - d;
		t1 = tcusp + d;
		
		return TWO_INFLECTIONS;
	}
	
    if ( d == 0.0f ) {
		t0 = tcusp;
		return ONE_CUSP;
	}

	return NONE;
}

//----------------------------------------------------------------//
void ZLCubicBezier2D::Flatten ( ZLAbstractVertexWriter2D& writer, float flatness, float angle ) const {

	// cribbed from https://github.com/rougier/gl-bezier

	float cusp = 0.0f;

	float t0_minus = -1.0f;
	float t0_plus = -1.0f;

	float t1_minus = 2.0f;
	float t1_plus = 2.0f;

	float inflection0;
	float inflection1;
	u32 inflections = this->FindInflections ( inflection0, inflection1 );
	
	switch ( inflections ) {
		case ONE_CUSP: {
			cusp = inflection0;
			break;
		}
		case TWO_INFLECTIONS: {
			this->FindInflectionDomain ( inflection0, t0_minus, t0_plus, flatness );
			this->FindInflectionDomain ( inflection1, t1_minus, t1_plus, flatness );
			break;
			
		}
		case ONE_INFLECTION: {
			this->FindInflectionDomain ( inflection0, t0_minus, t0_plus, flatness );
			break;
		}
		case DEGENERATE: {
			writer.WriteVertex ( this->mP0 ); // add first point
			writer.WriteVertex ( this->mP3 ); // add last point
			return;
		}
	}

	// Split the two domains if they overlap
	if (( t0_minus < t1_minus ) && ( t1_minus < t0_plus )) {
		t0_plus = t1_minus;
		t1_minus = t0_plus;
	}

    bool t0_out = ( t0_plus < 0.0f ) || ( t0_minus > 1.0f );
	bool t1_out = ( t1_plus < 0.0f ) || ( t1_minus > 1.0f );
	
	bool t0_t1_cross = t1_minus < t0_plus;

    // Make sure the possible out domain is always t1
    // (this will save some specific tests below)
    if ( t0_out ) {
    
        t0_minus = t1_minus;
        t0_plus = t1_plus;
        t0_out = t1_out;
        
        t1_minus = 2.0f;
        t1_plus = 2.0f;
        t1_out = true;
	}

	bool t0_in				= ( 0.0f < t0_minus ) && ( t0_minus < t0_plus ) && ( t0_plus < 1.0f );
	bool t0_cross_start		= ( t0_minus < 0.0f ) && ( 0.0f < t0_plus ) && ( t0_plus < 1.0f );
	bool t0_cross_end		= ( 0.0f < t0_minus ) && ( t0_minus < 1.0f ) && ( 1.0f < t0_plus );
	bool t0_cross			= t0_cross_start || t0_cross_end;

	bool t1_in				= ( 0.0f < t1_minus ) && ( t1_minus < t1_plus ) && ( t1_plus < 1.0f );
	//bool t1_cross_start		= ( t1_minus < 0.0f ) && ( 0.0f < t1_plus ) && ( t1_plus < 1.0f );
	bool t1_cross_end		= ( 0.0f < t1_minus ) && ( t1_minus < 1.0f ) && ( 1.0f < t1_plus );
	//bool t1_cross			= t1_cross_start || t1_cross_end;

	writer.WriteVertex ( this->mP0 ); // add first point
	
	ZLCubicBezier2D left;
	ZLCubicBezier2D right;
	
	if ( inflections == ONE_CUSP ) {
	
		this->Split ( inflection0, left, right );
		left.FlattenProgressive ( writer, flatness, angle );
		writer.WriteVertex ( this->Evaluate ( inflection0 )); // the inflection point
		right.FlattenProgressive ( writer, flatness, angle );
	}
	else {
		
        if ( t0_out && t1_out ) {
			// No inflection points
			this->FlattenProgressive ( writer, flatness, angle );
        }
        else if (( t0_in || t0_cross ) && t1_out ) {
			// One inflection point
			if ( t0_cross_start ) {
				writer.WriteVertex ( this->Evaluate ( t0_plus ));
				this->Split ( t0_plus, left, right );
				right.FlattenProgressive ( writer, flatness, angle );
			}
			else if ( t0_cross_end ) {
				this->Split ( t0_minus, left, right );
				left.FlattenProgressive ( writer, flatness, angle );
				writer.WriteVertex ( this->Evaluate ( t0_minus ) );
			}
			else {
				this->Split ( t0_minus, left, right );
				left.FlattenProgressive ( writer, flatness, angle );
				
				writer.WriteVertex ( this->Evaluate ( t0_minus ) );
				writer.WriteVertex ( this->Evaluate ( t0_plus ) );
				
				this->Split ( t0_plus, left, right );
				right.FlattenProgressive ( writer, flatness, angle );
			}
        }
        else if (( t0_in || t0_cross_start ) && ( t1_in || t1_cross_end )) {
			// Two inflection points
			if ( !t0_cross_start ) {
				this->Split ( t0_minus, left, right );
				left.FlattenProgressive ( writer, flatness, angle );
				writer.WriteVertex ( this->Evaluate ( t0_minus ) );
			}
			
			if ( t0_t1_cross ) {
				writer.WriteVertex ( this->Evaluate ( t1_minus ) );
				writer.WriteVertex ( this->Evaluate ( t0_plus ) );
			}
			else {
				writer.WriteVertex ( this->Evaluate ( t0_plus ) );
				ZLCubicBezier2D middle = this->Split ( t0_plus, t1_minus );
				middle.FlattenProgressive ( writer, flatness, angle );
				writer.WriteVertex ( this->Evaluate ( t1_minus ) );
			}
			
			if ( !t1_cross_end ) {
				writer.WriteVertex ( this->Evaluate ( t1_plus ) );
				this->Split ( t1_plus, left, right );
				right.FlattenProgressive ( writer, flatness, angle );
			}
		}
	}

	writer.WriteVertex ( this->mP3 );

//	switch ( inflections ) {
//		
//		case NONE: {
//			this->FlattenProgressive ( writer, flatness, angle );
//			break;
//		}
//		case ONE_CUSP: {
//		
//			this->Split ( inflection0, left, right );
//			left.FlattenProgressive ( writer, flatness, angle );
//			writer.WriteVertex ( this->Evaluate ( inflection0 )); // the inflection point
//			right.FlattenProgressive ( writer, flatness, angle );
//			break;
//		}
//		case ONE_INFLECTION: {
//			
//			if ( t0_cross_start ) {
//				writer.WriteVertex ( this->Evaluate ( t0_plus ));
//				this->Split ( t0_plus, left, right );
//				right.FlattenProgressive ( writer, flatness, angle );
//			}
//			else if ( t0_cross_end ) {
//				this->Split ( t0_minus, left, right );
//				left.FlattenProgressive ( writer, flatness, angle );
//				writer.WriteVertex ( this->Evaluate ( t0_minus ) );
//			}
//			else {
//				this->Split ( t0_minus, left, right );
//				left.FlattenProgressive ( writer, flatness, angle );
//				
//				writer.WriteVertex ( this->Evaluate ( t0_minus ) );
//				writer.WriteVertex ( this->Evaluate ( t0_plus ) );
//				
//				this->Split ( t0_plus, left, right );
//				right.FlattenProgressive ( writer, flatness, angle );
//			}
//			break;
//		}
//		case TWO_INFLECTIONS: {
//			
//			if ( !t0_cross_start ) {
//				this->Split ( t0_minus, left, right );
//				left.FlattenProgressive ( writer, flatness, angle );
//				writer.WriteVertex ( this->Evaluate ( t0_minus ) );
//			}
//			
//			if ( t0_t1_cross ) {
//				writer.WriteVertex ( this->Evaluate ( t1_minus ) );
//				writer.WriteVertex ( this->Evaluate ( t0_plus ) );
//			}
//			else {
//				writer.WriteVertex ( this->Evaluate ( t0_plus ) );
//				ZLCubicBezier2D middle = this->Split ( t0_plus, t1_minus );
//				middle.FlattenProgressive ( writer, flatness, angle );
//				writer.WriteVertex ( this->Evaluate ( t1_minus ) );
//			}
//			
//			if ( !t1_cross_end ) {
//				writer.WriteVertex ( this->Evaluate ( t1_plus ) );
//				this->Split ( t1_plus, left, right );
//				right.FlattenProgressive ( writer, flatness, angle );
//			}
//			break;
//		}
//	}
}

//----------------------------------------------------------------//
void ZLCubicBezier2D::FlattenProgressive ( ZLAbstractVertexWriter2D& writer, float flatness, float angle ) const {

	angle *= ( float )D2R;
	ZLCubicBezier2D curve = *this;
    curve.Bless ();
    
    bool more = true;
	while ( more ) {
    
		float dx = curve.mP1.mX - curve.mP0.mX;
		float dy = curve.mP1.mY - curve.mP0.mY;
		
		float norm = Sqrt (( dx * dx ) + ( dy * dy ));
		if ( norm == 0.0f ) {
			break;
		}

		float s3 = ( float )fabs ((( curve.mP2.mX - curve.mP0.mX ) * dy ) - ((  curve.mP2.mY -  curve.mP0.mY ) * dx )) / norm;
		if ( s3 == 0.0f ) break;

		float t = 2.0f * Sqrt ( flatness /( 3.0f * s3 ));
		if ( t > 1.0f )break;

		ZLCubicBezier2D left;
		ZLCubicBezier2D right;

		// check if angle is below tolerance
		for ( u32 i = 0; i < 50; ++i ) { // TODO: make param; min of t better?
			curve.Split ( t, left, right );
			if ( left.Angle () > angle ) {
				t /= 1.75; // magic numbers, yay
			}
			else {
				break;
			}
		}
		
		curve = right;
		writer.WriteVertex ( curve.mP0 );
	}
	
	writer.WriteVertex ( curve.mP3 );
}

//----------------------------------------------------------------//
void ZLCubicBezier2D::Split ( float t, ZLCubicBezier2D& left, ZLCubicBezier2D& right ) const {

    // Split curve at t into left and right cubic bezier curves

	left.mP0 = this->mP0;
	
	left.mP1 = ZLVec2D::Lerp ( this->mP0, this->mP1, t );
	left.mP2 = ZLVec2D::Lerp ( this->mP1, this->mP2, t );

	right.mP2 = ZLVec2D::Lerp ( this->mP2, this->mP3, t );
	right.mP1 = ZLVec2D::Lerp ( left.mP2, right.mP2, t );
	
	left.mP2 = ZLVec2D::Lerp ( left.mP1, left.mP2, t );
	left.mP3 = ZLVec2D::Lerp ( left.mP2, right.mP1, t );
	
	right.mP0 = left.mP3;
	right.mP3 = this->mP3;
}

//----------------------------------------------------------------//
ZLCubicBezier2D ZLCubicBezier2D::Split ( float t0, float t1 ) const {
    // Extract a segment of t curve

    float u0 = 1.0f - t0;
    float u1 = 1.0f - t1;

	ZLVec2D a = ZLVec2D::Add ( ZLVec2D::Scale ( this->mP0, ( u0 * u0 )), ZLVec2D::Add ( ZLVec2D::Scale ( this->mP1, ( 2.0f * t0 * u0 )), ZLVec2D::Scale ( this->mP2, ( t0 * t0 ))));
	ZLVec2D b = ZLVec2D::Add ( ZLVec2D::Scale ( this->mP0, ( u1 * u1 )), ZLVec2D::Add ( ZLVec2D::Scale ( this->mP1, ( 2.0f * t1 * u1 )), ZLVec2D::Scale ( this->mP2, ( t1 * t1 ))));
	ZLVec2D c = ZLVec2D::Add ( ZLVec2D::Scale ( this->mP1, ( u0 * u0 )), ZLVec2D::Add ( ZLVec2D::Scale ( this->mP2, ( 2.0f * t0 * u0 )), ZLVec2D::Scale ( this->mP3, ( t0 * t0 ))));
	ZLVec2D d = ZLVec2D::Add ( ZLVec2D::Scale ( this->mP1, ( u1 * u1 )), ZLVec2D::Add ( ZLVec2D::Scale ( this->mP2, ( 2.0f * t1 * u1 )), ZLVec2D::Scale ( this->mP3, ( t1 * t1 ))));

    ZLCubicBezier2D segment;

    segment.mP0 = ZLVec2D::Add ( ZLVec2D::Scale ( a, u0 ), ZLVec2D::Scale ( c, t0 ));
    segment.mP1 = ZLVec2D::Add ( ZLVec2D::Scale ( a, u1 ), ZLVec2D::Scale ( c, t1 ));
    segment.mP2 = ZLVec2D::Add ( ZLVec2D::Scale ( b, u0 ), ZLVec2D::Scale ( d, t0 ));
    segment.mP3 = ZLVec2D::Add ( ZLVec2D::Scale ( b, u1 ), ZLVec2D::Scale ( d, t1 ));

	return segment;
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
