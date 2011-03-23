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
		case kEaseIn:		return EaseIn ( t );
		case kEaseOut:		return EaseOut ( t );
		case kFlat:			return Flat ( t );
		case kLinear:		return Linear ( t );
		case kSmooth:		return Smooth ( t );
		case kSoftEaseIn:	return SoftEaseIn ( t );
		case kSoftEaseOut:	return SoftEaseOut ( t );
		case kSoftSmooth:	return SoftSmooth ( t );
	}
	assert ( false );
	return 0.0f;
}

//----------------------------------------------------------------//
float USInterpolate::EaseIn ( float t ) {

	// ease in
	t = t - 1.0f;
	return 1.0f - ( t * t * t * t );
}

//----------------------------------------------------------------//
float USInterpolate::EaseIn ( float x0, float x1, float t ) {

	// ease in
	float s = EaseIn ( t );
	return x0 + (( x1 - x0 ) * s );
}

//----------------------------------------------------------------//
float USInterpolate::EaseOut ( float t ) {

	// ease out
	return t * t * t * t;
}

//----------------------------------------------------------------//
float USInterpolate::EaseOut ( float x0, float x1, float t ) {

	// ease out
	float s = EaseOut ( t );
	return x0 + (( x1 - x0 ) * s );
}

//----------------------------------------------------------------//
float USInterpolate::Flat ( float t ) {

	// flat
	return ( t < 1.0f ) ? 0.0f : 1.0f;
}

//----------------------------------------------------------------//
float USInterpolate::Flat ( float x0, float x1, float t ) {

	// flat
	return ( t < 1.0f ) ? x0 : x1;
}

//----------------------------------------------------------------//
float USInterpolate::Interpolate ( u32 mode, float x0, float x1, float t ) {

	switch ( mode ) {
		case kEaseIn:		return EaseIn ( x0, x1, t );
		case kEaseOut:		return EaseOut ( x0, x1, t );
		case kFlat:			return Flat ( x0, x1, t );
		case kLinear:		return Linear ( x0, x1, t );
		case kSmooth:		return Smooth ( x0, x1, t );
		case kSoftEaseIn:	return SoftEaseIn ( x0, x1, t );
		case kSoftEaseOut:	return SoftEaseOut ( x0, x1, t );
		case kSoftSmooth:	return SoftSmooth ( x0, x1, t );
	}
	assert ( false );
	return 0.0f;
}

//----------------------------------------------------------------//
float USInterpolate::Linear ( float t ) {

	// linear
	return t;
}

//----------------------------------------------------------------//
float USInterpolate::Linear ( float x0, float x1, float t ) {

	// linear
	return x0 + (( x1 - x0 ) * t );
}

//----------------------------------------------------------------//
float USInterpolate::Smooth ( float t ) {

	// smooth
	if ( t < 0.5f ) {
		
		t = t * 2.0f;
		return ( t * t * t * t ) * 0.5f;
	}
	
	t = ( t * 2.0f ) - 2.0f;
	return ( 2.0f - ( t * t * t * t )) * 0.5f;
}

//----------------------------------------------------------------//
float USInterpolate::Smooth ( float x0, float x1, float t ) {

	// smooth
	float s = Smooth ( t );
	return x0 + (( x1 - x0 ) * s );
}

//----------------------------------------------------------------//
float USInterpolate::SoftEaseIn ( float t ) {

	// ease in
	t = t - 1.0f;
	return 1.0f - ( t * t );
}

//----------------------------------------------------------------//
float USInterpolate::SoftEaseIn ( float x0, float x1, float t ) {

	// ease in
	float s = SoftEaseIn ( t );
	return x0 + (( x1 - x0 ) * s );
}

//----------------------------------------------------------------//
float USInterpolate::SoftEaseOut ( float t ) {

	// ease out
	return t * t;
}

//----------------------------------------------------------------//
float USInterpolate::SoftEaseOut ( float x0, float x1, float t ) {

	// ease out
	float s = SoftEaseOut ( t );
	return x0 + (( x1 - x0 ) * s );
}

//----------------------------------------------------------------//
float USInterpolate::SoftSmooth ( float t ) {

	// smooth
	if ( t < 0.5f ) {
		
		t = t * 2.0f;
		return ( t * t ) * 0.5f;
	}
	
	t = ( t * 2.0f ) - 2.0f;
	return ( 2.0f - ( t * t )) * 0.5f;
}

//----------------------------------------------------------------//
float USInterpolate::SoftSmooth ( float x0, float x1, float t ) {

	// smooth
	float s = SoftSmooth ( t );
	return x0 + (( x1 - x0 ) * s );
}
