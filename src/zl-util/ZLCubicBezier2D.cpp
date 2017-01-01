// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <zl-util/ZLCubicBezier2D.h>

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

	// TODO: need to use split instead of segment since we don't care about left?
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

	ZLCubicBezierFlattener2D flattener;
	flattener.Init ( *this, flatness, angle );
	
	while ( flattener.More ()) {
		writer.WriteVertex ( flattener.Next ());
	}
}

//----------------------------------------------------------------//
float ZLCubicBezier2D::GetFlattenedLength ( float flatness, float angle ) {

	float length = 0.0f;
	
	ZLCubicBezierFlattener2D flattener;
	flattener.Init ( *this, flatness, angle );
	
	if ( flattener.More ()) {
	
		ZLVec2D v0 = flattener.Next ();
	
		while ( flattener.More ()) {
		
			ZLVec2D v1 = flattener.Next ();
			length += v0.Dist ( v1 );
			v0 = v1;
		}
	}
	
	return length;
}

//----------------------------------------------------------------//
size_t ZLCubicBezier2D::GetFlattenedSize ( float flatness, float angle ) {

	size_t size = 0;
	
	ZLCubicBezierFlattener2D flattener;
	flattener.Init ( *this, flatness, angle );
	
	for ( ; flattener.More (); flattener.Next (), size++ );
	
	return size;
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
// ZLCubicBezierFlattener2D
//================================================================//

//----------------------------------------------------------------//
void ZLCubicBezierFlattener2D::Init ( const ZLCubicBezier2D& curve, float flatness, float angle ) {

	// cribbed from https://github.com/rougier/gl-bezier

	this->mFlatness = flatness;
	this->mAngleInRadians = ( float )( angle * D2R );

	this->mCurveIdx = 0;
	this->mVertexIdx = 0;
	this->mCommandIdx = 0;

	float cusp = 0.0f;

	float t0_minus = -1.0f;
	float t0_plus = -1.0f;

	float t1_minus = 2.0f;
	float t1_plus = 2.0f;

	float inflection0;
	float inflection1;
	
	u32 inflections = curve.FindInflections ( inflection0, inflection1 );
	
	switch ( inflections ) {
	
		case ZLCubicBezier2D::ONE_CUSP: {
			cusp = inflection0;
			break;
		}
		
		case ZLCubicBezier2D::TWO_INFLECTIONS: {
			curve.FindInflectionDomain ( inflection0, t0_minus, t0_plus, flatness );
			curve.FindInflectionDomain ( inflection1, t1_minus, t1_plus, flatness );
			break;
			
		}
		
		case ZLCubicBezier2D::ONE_INFLECTION: {
			curve.FindInflectionDomain ( inflection0, t0_minus, t0_plus, flatness );
			break;
		}
		
		case ZLCubicBezier2D::DEGENERATE: {
			this->PushCommand ( curve.mP0 ); // add first point
			this->PushCommand ( curve.mP3 ); // add last point
			break;
		}
	}

	if ( inflections != ZLCubicBezier2D::DEGENERATE ) {

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
		bool t1_cross_end		= ( 0.0f < t1_minus ) && ( t1_minus < 1.0f ) && ( 1.0f < t1_plus );

		this->PushCommand ( curve.mP0 ); // add first point
		
		ZLCubicBezier2D left;
		ZLCubicBezier2D right;
		
		if ( inflections == ZLCubicBezier2D::ONE_CUSP ) {
		
			// 3 commands
			// 2 curves
			// 1 vertex
		
			curve.Split ( inflection0, left, right );
			this->PushCommand ( left );
			this->PushCommand ( curve.Evaluate ( inflection0 )); // the inflection point
			this->PushCommand ( right );
		}
		else {
			
			if ( t0_out && t1_out ) {
			
				// 1 commands
				// 1 curves
			
				// No inflection points
				this->PushCommand ( curve );
			}
			else if (( t0_in || t0_cross ) && t1_out ) {
			
				// One inflection point
				if ( t0_cross_start ) {
				
					// 2 commands
					// 1 curve
					// 1 vertex
				
					this->PushCommand ( curve.Evaluate ( t0_plus ));
					curve.Split ( t0_plus, left, right );
					this->PushCommand ( right );
				}
				else if ( t0_cross_end ) {
				
					// 2 commands
					// 1 curve
					// 1 vertex
				
					curve.Split ( t0_minus, left, right );
					this->PushCommand ( left );
					this->PushCommand ( curve.Evaluate ( t0_minus ) );
				}
				else {
				
					// 4 commands
					// 2 curves
					// 2 vertices
				
					curve.Split ( t0_minus, left, right );
					this->PushCommand ( left );
					
					this->PushCommand ( curve.Evaluate ( t0_minus ));
					this->PushCommand ( curve.Evaluate ( t0_plus ));
					
					curve.Split ( t0_plus, left, right );
					this->PushCommand ( right );
				}
			}
			else if (( t0_in || t0_cross_start ) && ( t1_in || t1_cross_end )) {
			
				// Two inflection points
				
				// max 7 commands
				// max 3 curves
				// max 4 vertices
				
				if ( !t0_cross_start ) {
				
					// 2 commands
					// 1 curve
					// 1 vertex
				
					curve.Split ( t0_minus, left, right );
					this->PushCommand ( left );
					this->PushCommand ( curve.Evaluate ( t0_minus ));
				}
				
				if ( t0_t1_cross ) {
				
					// 2 commands
					// 2 vertices
				
					this->PushCommand ( curve.Evaluate ( t1_minus ));
					this->PushCommand ( curve.Evaluate ( t0_plus ));
				}
				else {
				
					// 3 commands
					// 1 curve
					// 2 vertices
				
					this->PushCommand ( curve.Evaluate ( t0_plus ));
					ZLCubicBezier2D middle = curve.Split ( t0_plus, t1_minus );
					this->PushCommand ( middle );
					this->PushCommand ( curve.Evaluate ( t1_minus ));
				}
				
				if ( !t1_cross_end ) {
				
					// 2 commands
					// 1 curve
					// 1 vertex
					
					this->PushCommand ( curve.Evaluate ( t1_plus ));
					curve.Split ( t1_plus, left, right );
					this->PushCommand ( right );
				}
			}
		}

		this->PushCommand ( curve.mP3 );
	}
	
	assert ( this->mCommandIdx );
	
	this->mTotalCommands = this->mCommandIdx;
	
	this->mCurveIdx = 0;
	this->mVertexIdx = 0;
	this->mCommandIdx = 0;
	
	this->mIsProcessingCurve = false;
}

//----------------------------------------------------------------//
bool ZLCubicBezierFlattener2D::More () {

	return this->mCommandIdx < this->mTotalCommands;
}

//----------------------------------------------------------------//
ZLVec2D ZLCubicBezierFlattener2D::Next () {

	if ( this->mCommandIdx >= this->mTotalCommands ) {
		return this->mLastVertex;
	}

	if ( !this->mIsProcessingCurve ) {
	
		u32 command = this->mCommands [ this->mCommandIdx++ ];
		
		if ( command == COMMAND_VERTEX ) {
			return this->mVertices [ this->mVertexIdx++ ];
		}
		
		// must be a curve
		assert ( command == COMMAND_CURVE );
		
		this->mIsProcessingCurve = true;
		this->mCurve = this->mCurves [ this->mCurveIdx++ ];
		this->mCurve.Bless ();
	}
	
	const ZLCubicBezier2D& curve = this->mCurve;
	
	float dx = curve.mP1.mX - curve.mP0.mX;
	float dy = curve.mP1.mY - curve.mP0.mY;
	
	float norm = Sqrt (( dx * dx ) + ( dy * dy ));
	float s3 = ( float )fabs ((( curve.mP2.mX - curve.mP0.mX ) * dy ) - ((  curve.mP2.mY -  curve.mP0.mY ) * dx )) / norm;
	float t = 2.0f * Sqrt ( this->mFlatness /( 3.0f * s3 ));
	
	if (( norm == 0.0f ) || ( s3 == 0.0f ) || ( t > 1.0f )) {
		this->mIsProcessingCurve = false;
		this->mLastVertex = this->mCurve.mP3;
		return this->mLastVertex;
	}
	
	ZLCubicBezier2D left;
	ZLCubicBezier2D right;
	
	// check if angle is below tolerance
	for ( u32 i = 0; i < 50; ++i ) { // TODO: make param; min of t better?
		curve.Split ( t, left, right );
		if ( left.Angle () > this->mAngleInRadians ) {
			t /= 1.75f; // magic numbers, yay
		}
		else {
			break;
		}
	}
	
	this->mCurve = right;
	this->mLastVertex = right.mP0;
	return this->mLastVertex;
}

//----------------------------------------------------------------//
void ZLCubicBezierFlattener2D::PushCommand ( const ZLCubicBezier2D& curve ) {

	this->mCurves [ this->mCurveIdx++ ] = curve;
	this->mCommands [ this->mCommandIdx++ ] = COMMAND_CURVE;
}

//----------------------------------------------------------------//
void ZLCubicBezierFlattener2D::PushCommand ( const ZLVec2D& vertex ) {

	this->mVertices [ this->mVertexIdx++ ] = vertex;
	this->mCommands [ this->mCommandIdx++ ] = COMMAND_VERTEX;
}

//----------------------------------------------------------------//
ZLCubicBezierFlattener2D::ZLCubicBezierFlattener2D () :
	mTotalCommands ( 0 ) {
}

//----------------------------------------------------------------//
ZLCubicBezierFlattener2D::~ZLCubicBezierFlattener2D () {
}
