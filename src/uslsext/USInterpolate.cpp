// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <uslsext/USTrig.h>
#include <uslsext/USInterpolate.h>

//================================================================//
// USInterpolate
//================================================================//

//----------------------------------------------------------------//
float USInterpolate::Curve ( u32 mode, float t ) {

	switch ( mode ) {
		
		case kEaseIn:
		
			t = t - 1.0f;
			return 1.0f - ( t * t * t * t );
		
		case kEaseOut:
		
			return t * t * t * t;
			
		case kFlat:
		
			return ( t < 1.0f ) ? 0.0f : 1.0f;
		
		case kLinear:
		
			return t;
		
		case kSharpEaseIn:
		
			t = t - 1.0f;
			return 1.0f - ( t * t * t * t * t * t * t * t );
			
		case kSharpEaseOut:
		
			return t * t * t * t * t * t;
		
		case kSharpSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return ( t * t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - ( t * t * t * t * t * t )) * 0.5f;
		
		case kSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return ( t * t * t * t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - ( t * t * t * t )) * 0.5f;
		
		case kSoftEaseIn:
		
			t = t - 1.0f;
			return 1.0f - ( t * t );
			
		case kSoftEaseOut:
		
			return t * t;
		
		case kSoftSmooth:
		
			if ( t < 0.5f ) {
				t = t * 2.0f;
				return ( t * t ) * 0.5f;
			}
			t = ( t * 2.0f ) - 2.0f;
			return ( 2.0f - ( t * t )) * 0.5f;

	}
	return 0.0f;
}

//----------------------------------------------------------------//
float USInterpolate::Interpolate ( u32 mode, float x0, float x1, float t ) {

	if ( mode == kFlat ) {
		return ( t < 1.0f ) ? x0 : x1;
	}
	float s = Curve ( mode, t );
	return x0 + (( x1 - x0 ) * s );
}


//----------------------------------------------------------------//
float USInterpolate::Interpolate ( u32 mode, float x0, float x1, float t, float w ) {

	float v0 = Interpolate ( mode, x0, x1, t );
	
	if ( w == 1.0f ) {
		return v0;
	}
	float v1 = Interpolate ( kLinear, x0, x1, t );
	return Interpolate ( kLinear, v1, v0, w );
}

