// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <zl-util/ZLTrig.h>
#include <zl-util/ZLInterpolate.h>

//----------------------------------------------------------------//
// t ^ 4
static float _pow ( float t ) {

	return t * t * t * t;
}

//----------------------------------------------------------------//
// t ^ 16
static float _pow_extra_sharp ( float t ) {

	t = t * t * t * t;
	t = t * t;
	t = t * t;
	return t;
}

//----------------------------------------------------------------//
// t ^ 8
static float _pow_sharp ( float t ) {

	t = t * t * t * t;
	t = t * t;
	return t;
}

//----------------------------------------------------------------//
// t ^ 2
static float _pow_soft ( float t ) {

	return t * t;
}

//================================================================//
// ZLInterpolate
//================================================================//

//----------------------------------------------------------------//
float ZLInterpolate::Curve ( u32 mode, float t ) {

	switch ( mode ) {
		
		case kEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow ( t );
		
		case kEaseOut:
		
			return _pow ( t );
		
		case kExtraSharpEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow_extra_sharp ( t );
			
		case kExtraSharpEaseOut:
		
			return _pow_extra_sharp ( t );
		
		case kExtraSharpSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow_extra_sharp ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow_extra_sharp ( t )) * 0.5f;
		
		case kFlat:
		
			return ( t < 1.0f ) ? 0.0f : 1.0f;
		
		case kLinear:
		
			return t;
		
		case kSharpEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow_sharp ( t );
			
		case kSharpEaseOut:
		
			return _pow_sharp ( t );
		
		case kSharpSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow_sharp ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow_sharp ( t )) * 0.5f;
		
		case kSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow ( t )) * 0.5f;
		
		case kSoftEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow_soft ( t );
			
		case kSoftEaseOut:
		
			return _pow_soft ( t );
		
		case kSoftSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow_soft ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow_soft ( t )) * 0.5f;

	}
	return 0.0f;
}

//----------------------------------------------------------------//
float ZLInterpolate::Curve ( u32 mode, float t, float w ) {

	float v0 = Curve ( kLinear, t );
	float v1 = Curve ( mode, t );
	
	return Interpolate ( kLinear, v0, v1, w );
}

//----------------------------------------------------------------//
float ZLInterpolate::Interpolate ( u32 mode, float x0, float x1, float t ) {

	if ( mode == kFlat ) {
		return ( t < 1.0f ) ? x0 : x1;
	}
	float s = Curve ( mode, t );
	return x0 + (( x1 - x0 ) * s );
}

//----------------------------------------------------------------//
float ZLInterpolate::Interpolate ( u32 mode, float x0, float x1, float t, float w ) {
	
	float s = Curve ( mode, t, w );
	return x0 + (( x1 - x0 ) * s );
}
