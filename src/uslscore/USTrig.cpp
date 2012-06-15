// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USMathConsts.h>
#include <uslscore/USTrig.h>
#include <math.h>

// these are wrappers for later optimization; central file to
// optimize and replace w/ platform specific quirks & optimizations

//================================================================//
// Trig
//================================================================//

//----------------------------------------------------------------//
float ACos ( float t ) {
	return acosf ( t );
}

//----------------------------------------------------------------//
float ASin ( float t ) {
	return asinf ( t );
}

//----------------------------------------------------------------//
float ATan ( float t ) {
	return atanf ( t );
}

//----------------------------------------------------------------//
float Cos ( float radians ) {

	// use local sin wrapper to account for range bug (see comment on Sin ())
	return Sin ( radians + (( float )PI / 2.0f ));
}

//----------------------------------------------------------------//
float Cot ( float radians ) {
	return 1.0f / tanf ( radians );
}

//----------------------------------------------------------------//
float Log ( float x ) {

	return logf ( x );
}

//----------------------------------------------------------------//
float Log2 ( float x ) {

	return ( float )( log ( x ) * LOG2E );
}

//----------------------------------------------------------------//
float Log10 ( float x ) {

	return ( float )( log ( x ) * LOG10E );
}

//----------------------------------------------------------------//
float Sin ( float radians ) {

	// fixes quirk on legacy platform dealing with safe range for sinf
	// TODO: investigate if fix still required on current platforms
	if ( radians < PI )		return sinf ( radians );
	else					return -sinf ( radians - ( float )PI );
}

//----------------------------------------------------------------//
float Tan ( float radians ) {
	return tanf ( radians );
}

//----------------------------------------------------------------//
float Rand () {

	return ( float )rand () / ( float )RAND_MAX;
}

//----------------------------------------------------------------//
int Rand ( int lower, int upper ) {

	int value = lower + ( int )(( float )(( upper + 1 ) - lower ) * Rand ());
	if ( value > upper ) value = upper;
	
	return value;
}

//----------------------------------------------------------------//
float Sqrt ( float n ) {
	return sqrtf ( n );
}