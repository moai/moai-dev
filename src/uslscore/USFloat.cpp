// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <uslscore/USFloat.h>

// IEEE floating point encoding
// 32 bit: 1-bit sign | 8-bit exponent (+127) | 23-bit mantissa
// 64 bit: 1-bit sign | 11-bit exponent (+1023) | 52-bit mantissa
// to decode:
//		exponent (-127 or -1023) places decimal (in binary)
//		for integer: shift mantissa left by exponent
//		for decimal: (counting right from binary decimal) - 1/2 1/4 1/8 1/16...

//	For example (32-bit):
//		43 4D 40 00 (hex)
//	re-written in binary:
//		0100 0011  0100 1101  0100 0000 0000 0000
//	re-grouped:
//		sign:			exponent(+127):		mantissa:
//		0				10000110			(1.)100110101000...
//		(positive)		134(dec)
//	exponent: 134-127 = 7
//	moving the decimal point 7 positions to the right:
//		1   1  0  0  1  1  0  1 .  0   1
//		128 64 32 16  8  4  2  1   1/2 1/4 (weights)
//		= +205.25

//----------------------------------------------------------------//
float USFloat::Abs ( float n ) {

	return ( n < 0.0f ) ? -n : n;
}

//----------------------------------------------------------------//
float USFloat::Ceil ( float n ) {
	
	return ceilf ( n );
}

//----------------------------------------------------------------//
/** @brief Clamps n to [l, u].
	@param n A floating point number.
	@param l Lower bound of the range.
	@param u Upper bound of the range.
	@return n clamped to the range [l, n].
*/
float USFloat::Clamp ( float n, float l, float u ) {

	if ( n < l ) {
		return l;
	}
	else if ( n > u ) {
		return u;
	}

	return n;
}

//----------------------------------------------------------------//
float USFloat::Damp ( float v, float damp ) {

	damp = damp < 0.0f ? - damp : damp;

	if ( v > 0.0f ) {
		v -= damp;
		if ( v < 0.0f ) v = 0.0f;
	}
	else if ( v < 0.0f ) {
		v += damp;
		if ( v > 0.0f ) v = 0.0f;
	}
	return v;
}

//----------------------------------------------------------------//
float USFloat::Decimal ( float n ) {

	// TODO: optimize me
	if ( n < 0.0f ) {
		return n + ( float )(( int )n );
	}
	return n - ( float )(( int )n );
}

//----------------------------------------------------------------//
/** @brief Convert a floating point number to an integer key suitable for radix sort.
	@param f A floating point number.
	@return An integer key.
*/
u32 USFloat::FloatToIntKey ( float f ) {
	
	// Michael Herf says:
	// 1. Always flip the sign bit.
	// 2. If the sign bit was set, flip the other bits too.
	
	// - when the sign bit is set, xor with 0xFFFFFFFF (flip every bit)
	// - when the sign bit is unset, xor with 0x80000000 (flip the sign bit)
	
	u32 i = ( u32& )f;
	u32 mask = ( u32 )( -s32( i >> 31 ) | 0x80000000 );
	return ( u32 )( i ^ mask );
}

//----------------------------------------------------------------//
float USFloat::Floor ( float n ) {

	return floorf ( n );
}

//----------------------------------------------------------------//
float USFloat::Integer ( float n ) {

	// TODO: optimize me
	return ( float )(( int )n );
}

//----------------------------------------------------------------//
/** @brief Convert an integet key back to the original floating point number.
	@param i An integer key.
	@return A floating point number.
*/
float USFloat::IntKeyToFloat ( u32 i ) {

	// Michael Herf says:
	// 1. Always flip the sign bit.
	// 2. If the sign bit was *not* set, flip the other bits too.

	u32 mask = (( i >> 31 ) - 1 ) | 0x80000000;
	i = i ^ mask;
	return ( float& )i;
}

//----------------------------------------------------------------//
u32 USFloat::IntRange ( float n, float fbase, float ftop, u32 base, u32 top ) {

	return ( u32 )Round (( float )base + ((( n - fbase ) / ftop ) * ( float )top ));
}

//----------------------------------------------------------------//
u32 USFloat::IntRangeNorm ( float n, u32 base, u32 top ) {

	return ( u32 )Round (( float )base + ( n * ( float )top ));
}

//----------------------------------------------------------------//
float USFloat::InvSqrt ( float x ) {

    float xhalf = 0.5f * x;
    int i = *( int* )&x;
    i = 0x5f3759df - ( i >> 1 );
    x = *( float* )&i;
    x = x * ( 1.5f - xhalf * x * x );
    return x;
}

//----------------------------------------------------------------//
bool USFloat::IsClose ( float x0, float x1, float epsilon ) {

	float diff = x1 - x0;
	diff = ( diff > 0.0f ) ? diff : -diff;
	bool result = ( diff <= epsilon );
	return result;
}

//----------------------------------------------------------------//
float USFloat::Max ( float x, float y ) {

	return ( x > y ) ? x : y;
}

//----------------------------------------------------------------//
float USFloat::Min ( float x, float y ) {

	return ( x < y ) ? x : y;
}

//----------------------------------------------------------------//
float USFloat::Mod ( float x, float m ) {

	float d = x / m;
	d = Decimal ( d );
	if ( d < 0.0f ) d += 1.0f;
	return d * m;
}

//----------------------------------------------------------------//
float USFloat::Rand () {

	return ( float )rand () / ( float )RAND_MAX;
}

//----------------------------------------------------------------//
float USFloat::Rand ( float range ) {

	return USFloat::Rand () * range;
}

//----------------------------------------------------------------//
float USFloat::Rand ( float lower, float upper ) {

	return lower + ( USFloat::Rand () * ( upper - lower ));
}

//----------------------------------------------------------------//
float USFloat::Round ( float n ) {

	float integer = ( float )(( u32 )n );
	float decimal = n - integer;

	if ( decimal >= 0.5f ) integer += 1.0f;
	return integer;
}
