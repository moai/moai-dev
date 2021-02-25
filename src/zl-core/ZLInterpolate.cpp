// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <zl-core/ZLTrig.h>
#include <zl-core/ZLInterpolate.h>

//----------------------------------------------------------------//
// t ^ 4
static ZLReal _pow ( ZLReal t ) {

	return t * t * t * t;
}

//----------------------------------------------------------------//
// t ^ 16
static ZLReal _pow_extra_sharp ( ZLReal t ) {

	t = t * t * t * t;
	t = t * t;
	t = t * t;
	return t;
}

//----------------------------------------------------------------//
// t ^ 8
static ZLReal _pow_sharp ( ZLReal t ) {

	t = t * t * t * t;
	t = t * t;
	return t;
}

//----------------------------------------------------------------//
// t ^ 2
static ZLReal _pow_soft ( ZLReal t ) {

	return t * t;
}

//================================================================//
// ZLInterpolate
//================================================================//

//----------------------------------------------------------------//
ZLReal ZLInterpolate::Curve ( u32 mode, ZLReal t ) {
	
	ZLReal p, s;
	
	switch ( mode ) {

		//................................................................
		case kEaseIn:
		
			t = t - 1.0;
			return 1.0 - _pow ( t );
		
		//................................................................
		case kEaseOut:
		
			return _pow ( t );
		
		//................................................................
		case kExtraSharpEaseIn:
		
			t = t - 1.0;
			return 1.0 - _pow_extra_sharp ( t );
		
		//................................................................
		case kExtraSharpEaseOut:
		
			return _pow_extra_sharp ( t );
		
		//................................................................
		case kExtraSharpSmooth:
		
			if ( t < 0.5 ) {
				t = t * 2.0;
				return _pow_extra_sharp ( t ) * 0.5;
			}
			t = ( t * 2.0 ) - 2.0;
			return ( 2.0 - _pow_extra_sharp ( t )) * 0.5;
		
		//................................................................
		case kExtraSharpSmoothEaseOut:
		
			return Curve ( kExtraSharpSmooth, _pow ( t ));
		
		//................................................................
		case kFlat:
		
			return ( t < 1.0 ) ? 0.0 : 1.0;
		
		//................................................................
		case kLinear:
		
			return t;
		
		//................................................................
		case kSharpEaseIn:
		
			t = t - 1.0;
			return 1.0 - _pow_sharp ( t );
		
		//................................................................
		case kSharpEaseOut:
		
			return _pow_sharp ( t );
		
		//................................................................
		case kSharpSmooth:
		
			if ( t < 0.5 ) {
				t = t * 2.0;
				return _pow_sharp ( t ) * 0.5;
			}
			t = ( t * 2.0 ) - 2.0;
			return ( 2.0 - _pow_sharp ( t )) * 0.5;
		
		//................................................................
		case kSharpSmoothEaseOut:
		
			return Curve ( kSharpSmooth, _pow ( t ));
		
		//................................................................
		case kSmooth:
		
			if ( t < 0.5 ) {
				t = t * 2.0;
				return _pow ( t ) * 0.5;
			}
			t = ( t * 2.0 ) - 2.0;
			return ( 2.0 - _pow ( t )) * 0.5;
		
		//................................................................
		case kSmoothEaseOut:
		
			return Curve ( kSmooth, _pow ( t ));
		
		//................................................................
		case kSoftEaseIn:
		
			t = t - 1.0;
			return 1.0 - _pow_soft ( t );
		
		//................................................................
		case kSoftEaseOut:
		
			return _pow_soft ( t );
		
		//................................................................
		case kSoftSmooth:
		
			if ( t < 0.5 ) {
				t = t * 2.0;
				return _pow_soft ( t ) * 0.5;
			}
			t = ( t * 2.0 ) - 2.0;
			return ( 2.0 - _pow_soft ( t )) * 0.5;
		
		//................................................................
		case kSoftSmoothEaseOut:
		
			return Curve ( kSoftSmooth, _pow ( t ));

		//................................................................
		case kSineEaseIn:
			
			return ( ZLReal )sin ( t * M_PI_2 );
		
		//................................................................
		case kSineEaseOut:
			
			return 1.0 - ( ZLReal )cos ( t * M_PI_2 );

		//................................................................
		case kSineSmooth:
			
			return 0.5 - 0.5 * ( ZLReal )cos ( t * M_PI );

		//................................................................
		case kCircEaseIn:
			
			t = t - 1.0;
			return ( ZLReal )sqrt ( 1.0 - t * t );

		//................................................................
		case kCircEaseOut:
			
			return 1.0 - ( ZLReal )sqrt ( 1.0 - t * t );

		//................................................................
		case kCircSmooth:
			
			if ( t < 0.5 ) {
				t = t * 2.0;
				return 0.5 - 0.5 * sqrtf ( 1.0 - t * t );
			}
			t = t * 2.0 - 2.0;
			return 0.5 + 0.5 * sqrtf ( 1.0 - t * t );
			
		//................................................................
		case kBounceIn:
			
			if ( t < (1.0 / 2.75) ) {
				return 7.5625 * t * t;
			}
			else if ( t < (2.0 / 2.75) ) {
				t = t - 1.5 / 2.75;
				return 7.5625 * t * t + 0.75;
			}
			else if ( t < (2.5 / 2.75) ) {
				t = t - 2.25 / 2.75;
				return 7.5625 * t * t + 0.9375;
			}
			else {
				t = t - 2.625 / 2.75;
				return 7.5625 * t * t + 0.984375;
			}

		//................................................................
		case kBounceOut:
			
			return 1.0 - Curve ( kBounceIn, 1.0 - t );

		//................................................................
		case kBounceSmooth:
			
			if ( t < 0.5 ) {
				return 0.5 * Curve ( kBounceOut, t * 2.0 );
			}
			else {
				return 0.5 + 0.5 * Curve ( kBounceIn, 2.0 * t - 1.0 );
			}
			
		//................................................................
		case kElasticIn:
			
			if ( t == 0.0 ) {
				return 0.0;
			}
			
			if ( t == 1.0 ) {
				return 1.0;
			}
			
			p = 0.3;
			s = 0.25 * p;
			return ( ZLReal )( 1.0 + pow ( 2.0, -10.0 * t ) * sin (( t - s ) * ( 2.0 * M_PI ) / p ));

		//................................................................
		case kElasticOut:
			
			if ( t == 0.0 ) {
				return 0.0;
			}
			
			if ( t == 1.0 ) {
				return 1.0;
			}
			
			p = 0.3;
			s = 0.25 * p;
			t = t - 1.0;
			return ( ZLReal )( -pow ( 2.0, 10.0 * t ) * sin (( t - s ) * ( 2.0 * M_PI ) / p ));

		//................................................................
		case kElasticSmooth:
			
			if ( t == 0.0 ) {
				return 0.0;
			}
			
			if ( t == 1.0 ) {
				return 1.0;
			}
			
			t = 2.0 * t;
			p = 0.3 * 1.5;
			s = 0.25 * p;
			
			if ( t < 1.0 ) {
				t = t - 1.0;
				return ( ZLReal )( -0.5 * ( pow ( 2.0, 10.0 * t ) * sin (( t - s ) * ( 2.0 * M_PI ) / p )));
			}
			t = t - 1.0;
			return ( ZLReal )( 1.0 + 0.5 * ( pow ( 2.0, -10.0 * t ) * sin (( t - s ) * ( 2.0 * M_PI ) / p )));

		//................................................................
		case kBackEaseIn:
			
			s = 1.70158;
			t = t - 1.0;
			return t * t * ( (s + 1.0) * t + s ) + 1.0;
			
		//................................................................
		case kBackEaseOut:
			
			s = 1.70158;
			return t * t * ( (s + 1.0) * t - s );
			
		//................................................................
		case kBackSmooth:
			
			s = 1.70158 * 1.525;
			t = 2.0 * t;
			if ( t < 1 ) {
				return 0.5 * ( t * t * ((s + 1.0) * t - s) );
			}
			t = t - 2.0;
			return 1.0 + 0.5 * (t * t * ((s + 1.0) * t + s));

	}
	return 0.0;
}

//----------------------------------------------------------------//
ZLReal ZLInterpolate::Curve ( u32 mode, ZLReal t, ZLReal w ) {

	ZLReal v0 = Curve ( kLinear, t );
	ZLReal v1 = Curve ( mode, t );
	
	return Interpolate ( kLinear, v0, v1, w );
}

//----------------------------------------------------------------//
ZLReal ZLInterpolate::Interpolate ( u32 mode, ZLReal x0, ZLReal x1, ZLReal t ) {

	if ( mode == kFlat ) {
		return ( t < 1.0 ) ? x0 : x1;
	}
	ZLReal s = Curve ( mode, t );
	return x0 + (( x1 - x0 ) * s );
}

//----------------------------------------------------------------//
ZLReal ZLInterpolate::Interpolate ( u32 mode, ZLReal x0, ZLReal x1, ZLReal t, ZLReal w ) {
	
	ZLReal s = Curve ( mode, t, w );
	return x0 + (( x1 - x0 ) * s );
}
