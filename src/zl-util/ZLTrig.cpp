// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLMathConsts.h>
#include <zl-util/ZLTrig.h>
#include <math.h>

// these are wrappers for later optimization; central file to
// optimize and replace w/ platform specific quirks & optimizations

//================================================================//
// Trig
//================================================================//

//----------------------------------------------------------------//
real ACos ( real t ) {
	return ( real )acos ( t );
}

//----------------------------------------------------------------//
real ASin ( real t ) {
	return ( real )asin ( t );
}

//----------------------------------------------------------------//
real ATan ( real t ) {
	return ( real )atan ( t );
}

//----------------------------------------------------------------//
real ATan2 ( real x, real y ) {
	return ( real )atan2 ( x, y );
}

//----------------------------------------------------------------//
real Cos ( real radians ) {

	// use local sin wrapper to account for range bug (see comment on Sin ())
	return Sin ( radians + (( real )PI / 2.0f ));
}

//----------------------------------------------------------------//
real Cot ( real radians ) {
	return ( real )( 1.0 / tan ( radians ));
}

//----------------------------------------------------------------//
real Log ( real x ) {

	return ( real )log ( x );
}

//----------------------------------------------------------------//
real Log2 ( real x ) {

	return ( real )( log ( x ) * LOG2E );
}

//----------------------------------------------------------------//
real Log10 ( real x ) {

	return ( real )( log ( x ) * LOG10E );
}

//----------------------------------------------------------------//
real Sin ( real radians ) {

	// fixes quirk on legacy platform dealing with safe range for sinf
	// TODO: investigate if fix still required on current platforms
	if ( radians < PI )		return sinf ( radians );
	else					return -sinf ( radians - ( real )PI );
}

//----------------------------------------------------------------//
real Tan ( real radians ) {
	return tanf ( radians );
}

//----------------------------------------------------------------//
real Rand () {

	return ( real )rand () / ( real )RAND_MAX;
}

//----------------------------------------------------------------//
int Rand ( int lower, int upper ) {

	int value = lower + ( int )(( real )(( upper + 1 ) - lower ) * Rand ());
	if ( value > upper ) value = upper;
	
	return value;
}

//----------------------------------------------------------------//
real Sqrt ( real n ) {
	return sqrtf ( n );
}