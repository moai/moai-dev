// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ROOT_FLOATOP_H
#define	ROOT_FLOATOP_H

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

// Some resources:

// Pierre Terdiman
// http://codercorner.com/RadixSortRevisited.htm

// Michael Herf
// http://stereopsis.com/radix.html
// http://stereopsis.com/FPU.html

// Chris Hecker
// http://chrishecker.com/Miscellaneous_Technical_Articles#Floating_Point

// "We should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil." - Donald Knuth

#ifndef EPSILON
	#define EPSILON 0.00001f
#endif

//================================================================//
// USFloat
//================================================================//
class USFloat {
public:

	// TODO: per platform profiling and optimization
	
	// This namespace is a placeholder for (to be) optimized floating point routines. Since we're targetting a few
	// different processors and are still pretty early in our cycle, we'll take Knuth's advice above and optimize here
	// after profiling on each platform. Right now these are just naive implementations and wrappers on the standard.

	//----------------------------------------------------------------//
	static inline float Abs ( float n ) {
		
		return n < 0.0f ? -n : n;
	}
	
	//----------------------------------------------------------------//
	static inline double Abs ( double n ) {
		
		return n < 0.0f ? -n : n;
	}

	//----------------------------------------------------------------//
	static inline float Ceil ( float n ) {
		
		return ceilf ( n );
	}
	
	//----------------------------------------------------------------//
	static inline double Ceil ( double n ) {
		
		return ceil ( n );
	}

	//----------------------------------------------------------------//
	static inline float Clamp ( float n, float l, float u ) {

		if ( n < l ) {
			return l;
		}
		else if ( n > u ) {
			return u;
		}

		return n;
	}

	//----------------------------------------------------------------//
	static inline float Damp ( float v, float damp ) {

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
	static inline float Decimal ( float n ) {

		if ( n < 0.0f ) {
			return n - ceilf ( n );
		}
		return n - floorf ( n );
	}
	
	//----------------------------------------------------------------//
	static inline double Decimal ( double n ) {

		if ( n < 0.0 ) {
			return n + floor ( n );
		}
		return n - floor ( n );
	}

	//----------------------------------------------------------------//
	static inline u32 FloatToIntKey ( float f ) {
		
		// Michael Herf says:
		// 1. Always flip the sign bit.
		// 2. If the sign bit was set, flip the other bits too.
		
		// - when the sign bit is set, xor with 0xFFFFFFFF (flip every bit)
		// - when the sign bit is unset, xor with 0x80000000 (flip the sign bit)
		
		u32 i;
		memcpy ( &i, &f, sizeof ( u32 ));
		u32 mask = ( u32 )( -s32( i >> 31 ) | 0x80000000 );
		return ( u32 )( i ^ mask );
	}

	//----------------------------------------------------------------//
	static inline float Floor ( float n ) {

		return floorf ( n );
	}
	
	//----------------------------------------------------------------//
	static inline double Floor ( double n ) {

		return floor ( n );
	}

	//----------------------------------------------------------------//
	static inline float FromBoolean ( bool b ) {
		return b ? 1.0f : 0.0f;
	}

	//----------------------------------------------------------------//
	static inline float IntKeyToFloat ( u32 i ) {

		// Michael Herf says:
		// 1. Always flip the sign bit.
		// 2. If the sign bit was *not* set, flip the other bits too.

		u32 mask = (( i >> 31 ) - 1 ) | 0x80000000;
		i = i ^ mask;
		float f;
		memcpy ( &f, &i, sizeof ( float ));
		return f;
	}

	//----------------------------------------------------------------//
	static inline u32 IntRange ( float n, float fbase, float ftop, u32 base, u32 top ) {

		return ( u32 )Round (( float )base + ((( n - fbase ) / ftop ) * ( float )top ));
	}

	//----------------------------------------------------------------//
	static inline u32 IntRangeNorm ( float n, u32 base, u32 top ) {

		return ( u32 )Round (( float )base + ( n * ( float )top ));
	}

	//----------------------------------------------------------------//
	static inline bool IsClose ( float x0, float x1, float epsilon ) {

		float diff = x1 - x0;
		diff = ( diff > 0.0f ) ? diff : -diff;
		bool result = ( diff <= epsilon );
		return result;
	}

	//----------------------------------------------------------------//
	static inline float Max ( float x, float y ) {

		return ( x > y ) ? x : y;
	}

	//----------------------------------------------------------------//
	float Min ( float x, float y ) {

		return ( x < y ) ? x : y;
	}

	//----------------------------------------------------------------//
	static inline float Mod ( float x, float m ) {

		float d = x / m;
		d = Decimal ( d );
		if ( d < 0.0f ) d += 1.0f;
		return d * m;
	}

	//----------------------------------------------------------------//
	static inline float Rand () {

		return ( float )rand () / ( float )RAND_MAX;
	}

	//----------------------------------------------------------------//
	static inline float Rand ( float range ) {

		return USFloat::Rand () * range;
	}

	//----------------------------------------------------------------//
	static inline float Rand ( float lower, float upper ) {

		return ( lower != upper ) ? lower + ( USFloat::Rand () * ( upper - lower )) : lower;
	}

	//----------------------------------------------------------------//
	static inline float Round ( float n ) {

		return ( float )floor ( n + 0.5f );
	}
	
	//----------------------------------------------------------------//
	static inline bool ToBoolean ( float n ) {
		
		return n == 0.0f ? false : true;
	}
	
	//----------------------------------------------------------------//
	static inline u32 ToIndex ( float n ) {
		
		return ( u32 )n;
	}
	
	//----------------------------------------------------------------//
	static inline int ToInt ( float n ) {
		
		return ( int )n;
	}
};

#endif
