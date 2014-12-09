// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREAL_H
#define	ZLREAL_H

// IEEE realing point encoding
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
// ZLReal
//================================================================//
class ZLReal {
public:

	// TODO: per platform profiling and optimization
	
	// This namespace is a placeholder for (to be) optimized realing point routines. Since we're targetting a few
	// different processors and are still pretty early in our cycle, we'll take Knuth's advice above and optimize here
	// after profiling on each platform. Right now these are just naive implementations and wrappers on the standard.

	//----------------------------------------------------------------//
	static inline real Abs ( real n ) {
		
		return n < 0.0f ? -n : n;
	}

	//----------------------------------------------------------------//
	static inline real Ceil ( real n ) {
		
		return ( real )ceil ( n );
	}

	//----------------------------------------------------------------//
	static inline real Clamp ( real n, real l, real u ) {

		if ( n < l ) {
			return l;
		}
		else if ( n > u ) {
			return u;
		}

		return n;
	}

	//----------------------------------------------------------------//
	static real* CopyDoublesToReals ( real* dest, const double* src, size_t count ) {
	
		#if ZL_SIZEOF_REAL == 4
		
			for ( size_t i = 0; i < count; ++i ) {
				dest [ i ] = ( float )src [ i ];
			}
			return dest;
		
		#elif ZL_SIZEOF_REAL == 8
		
			return ( real* )memcpy ( dest, src, count * 8 );
		
		#else
		
			#error "ZL_SIZEOF_REAL must be 4 or 8"
		
		#endif
	}

	//----------------------------------------------------------------//
	static real* CopyFloatsToReals ( real* dest, const float* src, size_t count ) {
	
		#if ZL_SIZEOF_REAL == 4
		
			return ( real* )memcpy ( dest, src, count * 4 );
		
		#elif ZL_SIZEOF_REAL == 8
		
			for ( size_t i = 0; i < count; ++i ) {
				dest [ i ] = ( double )src [ i ];
			}
			return dest;
		
		#else
		
			#error "ZL_SIZEOF_REAL must be 4 or 8"
		
		#endif
	}

	//----------------------------------------------------------------//
	static double* CopyRealsToDoubles ( double* dest, const real* src, size_t count ) {
	
		#if ZL_SIZEOF_REAL == 4
		
			for ( size_t i = 0; i < count; ++i ) {
				dest [ i ] = ( double )src [ i ];
			}
			return dest;
		
		#elif ZL_SIZEOF_REAL == 8
		
			return ( double* )memcpy ( dest, src, count * 8 );
		
		#else
		
			#error "ZL_SIZEOF_REAL must be 4 or 8"
		
		#endif
	}

	//----------------------------------------------------------------//
	static float* CopyRealsToFloats ( float* dest, const real* src, size_t count ) {
	
		#if ZL_SIZEOF_REAL == 4
		
			return ( float* )memcpy ( dest, src, count * 4 );
		
		#elif ZL_SIZEOF_REAL == 8
		
			for ( size_t i = 0; i < count; ++i ) {
				dest [ i ] = ( float )src [ i ];
			}
			return dest;
		
		#else
		
			#error "ZL_SIZEOF_REAL must be 4 or 8"
		
		#endif
	}

	//----------------------------------------------------------------//
	static inline real Damp ( real v, real damp ) {

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
	static inline real Decimal ( real n ) {

		if ( n < 0.0f ) {
			return n - ( real )ceil ( n );
		}
		return n - ( real )floor ( n );
	}

	//----------------------------------------------------------------//
	static inline u32 FloatToIntKey ( real num ) {
		
		// Michael Herf says:
		// 1. Always flip the sign bit.
		// 2. If the sign bit was set, flip the other bits too.
		
		// - when the sign bit is set, xor with 0xFFFFFFFF (flip every bit)
		// - when the sign bit is unset, xor with 0x80000000 (flip the sign bit)
		
		float f = ( float )num;
		
		u32 i;
		memcpy ( &i, &f, sizeof ( u32 ));
		u32 mask = ( u32 )( -s32( i >> 31 ) | 0x80000000 );
		return ( u32 )( i ^ mask );
	}

	//----------------------------------------------------------------//
	static inline real Floor ( real n ) {

		return ( real )floor ( n );
	}

	//----------------------------------------------------------------//
	static inline real FromBoolean ( bool b ) {
		return b ? 1.0 : 0.0;
	}

	//----------------------------------------------------------------//
	static inline real IntKeyToFloat ( u32 i ) {

		// Michael Herf says:
		// 1. Always flip the sign bit.
		// 2. If the sign bit was *not* set, flip the other bits too.

		u32 mask = (( i >> 31 ) - 1 ) | 0x80000000;
		i = i ^ mask;
		float f;
		memcpy ( &f, &i, sizeof ( real ));
		return ( real )f;
	}

	//----------------------------------------------------------------//
	static inline u32 IntRange ( real n, real fbase, real ftop, u32 base, u32 top ) {

		return ( u32 )Round (( real )base + ((( n - fbase ) / ftop ) * ( real )top ));
	}

	//----------------------------------------------------------------//
	static inline u32 IntRangeNorm ( real n, u32 base, u32 top ) {

		return ( u32 )Round (( real )base + ( n * ( real )top ));
	}

	//----------------------------------------------------------------//
	static inline bool IsClose ( real x0, real x1, real epsilon ) {

		real diff = x1 - x0;
		diff = ( diff > 0.0f ) ? diff : -diff;
		bool result = ( diff <= epsilon );
		return result;
	}

	//----------------------------------------------------------------//
	static inline real Max ( real x, real y ) {

		return ( x > y ) ? x : y;
	}
	
	//----------------------------------------------------------------//
	static inline real Max ( real x, real y, real z ) {

		return ( x > y ) ? Max ( x, z ) : Max ( y, z );
	}

	//----------------------------------------------------------------//
	static inline real Min ( real x, real y ) {

		return ( x < y ) ? x : y;
	}

	//----------------------------------------------------------------//
	static inline real Min ( real x, real y, real z ) {

		return ( x < y ) ? Min ( x, z ) : Min ( y, z );
	}

	//----------------------------------------------------------------//
	static inline real Mod ( real x, real m ) {

		real d = x / m;
		d = Decimal ( d );
		if ( d < 0.0f ) d += 1.0f;
		return d * m;
	}

	//----------------------------------------------------------------//
	static inline real Rand () {

		return ( real )rand () / ( real )RAND_MAX;
	}

	//----------------------------------------------------------------//
	static inline real Rand ( real range ) {

		return ZLReal::Rand () * range;
	}

	//----------------------------------------------------------------//
	static inline real Rand ( real lower, real upper ) {

		return ( lower != upper ) ? lower + ( ZLReal::Rand () * ( upper - lower )) : lower;
	}

	//----------------------------------------------------------------//
	static inline real Round ( real n ) {

		return ( real )floor ( n + 0.5f );
	}
	
	//----------------------------------------------------------------//
	static inline bool ToBoolean ( real n ) {
		
		return n == 0.0f ? false : true;
	}
	
	//----------------------------------------------------------------//
	static inline u32 ToIndex ( real n ) {
		
		return ( u32 )n;
	}
	
	//----------------------------------------------------------------//
	static inline int ToInt ( real n ) {
		
		return ( int )n;
	}
};

#endif
