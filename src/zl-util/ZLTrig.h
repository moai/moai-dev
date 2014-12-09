// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTRIG_H
#define	ZLTRIG_H

//----------------------------------------------------------------//
real	ACos	( real t );
real	ASin	( real t );
real	ATan	( real t );
real	ATan2	( real x, real y );
real	Cos		( real radians );
real	Cot		( real radians );
real	Log		( real x );
real	Log2	( real x );
real	Log10	( real x );
real	Sin		( real radians );
real	Tan		( real radians );

//----------------------------------------------------------------//
real	Rand	();
int		Rand	( int lower, int upper );
real	Sqrt	( real n );

//----------------------------------------------------------------//
template < typename TYPE >
inline TYPE Min ( TYPE a, TYPE b ) {
	return ( a < b ) ? a : b;
}

//----------------------------------------------------------------//
template < typename TYPE >
inline TYPE Max ( TYPE a, TYPE b ) {
	return ( a > b ) ? a : b;
}

#endif
