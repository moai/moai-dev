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
	
	float p, s;
	
	switch ( mode ) {

		//................................................................
		case kEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow ( t );
		
		//................................................................
		case kEaseOut:
		
			return _pow ( t );
		
		//................................................................
		case kExtraSharpEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow_extra_sharp ( t );
		
		//................................................................
		case kExtraSharpEaseOut:
		
			return _pow_extra_sharp ( t );
		
		//................................................................
		case kExtraSharpSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow_extra_sharp ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow_extra_sharp ( t )) * 0.5f;
		
		//................................................................
		case kExtraSharpSmoothEaseOut:
		
			return Curve ( kExtraSharpSmooth, _pow ( t ));
		
		//................................................................
		case kFlat:
		
			return ( t < 1.0f ) ? 0.0f : 1.0f;
		
		//................................................................
		case kLinear:
		
			return t;
		
		//................................................................
		case kSharpEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow_sharp ( t );
		
		//................................................................
		case kSharpEaseOut:
		
			return _pow_sharp ( t );
		
		//................................................................
		case kSharpSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow_sharp ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow_sharp ( t )) * 0.5f;
		
		//................................................................
		case kSharpSmoothEaseOut:
		
			return Curve ( kSharpSmooth, _pow ( t ));
		
		//................................................................
		case kSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow ( t )) * 0.5f;
		
		//................................................................
		case kSmoothEaseOut:
		
			return Curve ( kSmooth, _pow ( t ));
		
		//................................................................
		case kSoftEaseIn:
		
			t = t - 1.0f;
			return 1.0f - _pow_soft ( t );
		
		//................................................................
		case kSoftEaseOut:
		
			return _pow_soft ( t );
		
		//................................................................
		case kSoftSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return _pow_soft ( t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - _pow_soft ( t )) * 0.5f;
		
		//................................................................
		case kSoftSmoothEaseOut:
		
			return Curve ( kSoftSmooth, _pow ( t ));

		//................................................................
		case kSineEaseIn:
			
			return sinf ( t * M_PI_2 );
		
		//................................................................
		case kSineEaseOut:
			
			return 1.0f - cosf ( t * M_PI_2 );

		//................................................................
		case kSineSmooth:
			
			return 0.5f - 0.5f * cosf ( t * M_PI );

		//................................................................
		case kCircEaseIn:
			
			t = t - 1.0f;
			return sqrtf ( 1.0f - t * t );

		//................................................................
		case kCircEaseOut:
			
			return 1.0f - sqrtf ( 1.0f - t * t );

		//................................................................
		case kCircSmooth:
			
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return 0.5f - 0.5f * sqrtf ( 1.0f - t * t );
			}
			t = t * 2.0f - 2.0f;
			return 0.5f + 0.5f * sqrtf ( 1.0f - t * t );
			
		//................................................................
		case kBounceIn:
			
			if ( t < (1.0f / 2.75f) ) {
				return 7.5625f * t * t;
			}
			else if ( t < (2.0f / 2.75f) ) {
				t = t - 1.5f / 2.75f;
				return 7.5625f * t * t + 0.75f;
			}
			else if ( t < (2.5f / 2.75f) ) {
				t = t - 2.25f / 2.75f;
				return 7.5625f * t * t + 0.9375f;
			}
			else {
				t = t - 2.625 / 2.75f;
				return 7.5625f * t * t + 0.984375f;
			}
			return 0.0f;

		//................................................................
		case kBounceOut:
			
			return 1.0f - Curve ( kBounceIn, 1.0f - t );

		//................................................................
		case kBounceSmooth:
			
			if ( t < 0.5f ) {
				return 0.5f * Curve ( kBounceOut, t * 2.0f );
			}
			else {
				return 0.5f + 0.5f * Curve ( kBounceIn, 2.0f * t - 1.f );
			}
			
		//................................................................
		case kElasticIn:
			
			if ( t == 0.0f ) {
				return 0.0f;
			}
			
			if ( t == 1.0f ) {
				return 1.0f;
			}
			
			p = 0.3f;
			s = 0.25f * p;
			return 1.0f + powf ( 2.0f, -10.f * t ) * sinf ( (t - s) * (2.0f * M_PI) / p );

		//................................................................
		case kElasticOut:
			
			if ( t == 0.0f ) {
				return 0.0f;
			}
			
			if ( t == 1.0f ) {
				return 1.0f;
			}
			
			p = 0.3f;
			s = 0.25f * p;
			t = t - 1.0f;
			return -powf ( 2.0f, 10.f * t ) * sinf ( (t - s) * (2.0f * M_PI) / p );

		//................................................................
		case kElasticSmooth:
			
			if ( t == 0.0f ) {
				return 0.0f;
			}
			
			if ( t == 1.0f ) {
				return 1.0f;
			}
			
			t = 2.0f * t;
			p = 0.3f * 1.5f;
			s = 0.25f * p;
			
			if ( t < 1.0f ) {
				t = t - 1.0f;
				return -0.5f * ( powf ( 2.0f, 10.f * t ) * sinf ( (t - s) * (2.0f * M_PI) / p ) );
			}
			t = t - 1.0f;
			return 1.0f + 0.5f * ( powf ( 2.0f, -10.f * t ) * sinf ( (t - s) * (2.0f * M_PI) / p ) );

		//................................................................
		case kBackEaseIn:
			
			s = 1.70158f;
			t = t - 1.0f;
			return t * t * ( (s + 1.0f) * t + s ) + 1.0f;
			
		//................................................................
		case kBackEaseOut:
			
			s = 1.70158f;
			return t * t * ( (s + 1.0f) * t - s );
			
		//................................................................
		case kBackSmooth:
			
			s = 1.70158f * 1.525f;
			t = 2.0f * t;
			if ( t < 1 ) {
				return 0.5f * ( t * t * ((s + 1.0f) * t - s) );
			}
			t = t - 2.0f;
			return 1.0f + 0.5f * (t * t * ((s + 1.0f) * t + s));

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
