// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTRIG_H
#define	ZLTRIG_H

//----------------------------------------------------------------//
float	ACos	( float t );
float	ASin	( float t );
float	ATan	( float t );
float	ATan2	( float x, float y );
float	Cos		( float radians );
float	Cot		( float radians );
float	Log		( float x );
float	Log2	( float x );
float	Log10	( float x );
float	Sin		( float radians );
float	Tan		( float radians );

//----------------------------------------------------------------//
float	Rand	();
int		Rand	( int lower, int upper );
float	Sqrt	( float n );

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
