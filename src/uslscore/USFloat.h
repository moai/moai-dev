// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ROOT_FLOATOP_H
#define	ROOT_FLOATOP_H

/** @addtogroup MathLib */

//================================================================//
// USFloat
//================================================================//
/**	@brief Floating point utility functions.
	@ingroup MathLib
*/
namespace USFloat {

	//----------------------------------------------------------------//
	float	Abs				( float n );
	float	Ceil			( float s );
	float	Clamp			( float n, float l, float u );
	float	Damp			( float v, float damp );
	float	Decimal			( float n );
	u32		FloatToIntKey	( float f );
	float	Floor			( float n );
	float	Integer			( float n );
	float	IntKeyToFloat	( u32 i );
	u32		IntRange		( float n, float fbase, float ftop, u32 base, u32 top );
	u32		IntRangeNorm	( float n, u32 base, u32 top );
	float	InvSqrt			( float x );
	bool	IsClose			( float x0, float x1, float epsilon );
	float	Max				( float x, float y );
	float	Min				( float x, float y );
	float	Mod				( float x, float m );
	float	Rand			();
	float	Rand			( float range );
	float	Rand			( float lower, float upper );
	float	Round			( float n );
	
}

#endif
