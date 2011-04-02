// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	INTERPOLATE_H
#define	INTERPOLATE_H

//================================================================//
// USInterpolate
//================================================================//
namespace USInterpolate {
	
	enum {
		kEaseIn,
		kEaseOut,
		kFlat,
		kLinear,
		kSmooth,
		kSoftEaseIn,
		kSoftEaseOut,
		kSoftSmooth,
	};
	
	//----------------------------------------------------------------//
	float	Curve			( u32 mode, float t );
	float	EaseIn			( float t );
	float	EaseIn			( float x0, float x1, float t );
	float	EaseOut			( float t );
	float	EaseOut			( float x0, float x1, float t );
	float	Flat			( float t );
	float	Flat			( float x0, float x1, float t );
	float	Interpolate		( u32 mode, float x0, float x1, float t );
	float	Interpolate		( u32 mode, float x0, float x1, float t, float w );
	float	Linear			( float t );
	float	Linear			( float x0, float x1, float t );
	float	Smooth			( float t );
	float	Smooth			( float x0, float x1, float t );
	float	SoftEaseIn		( float t );
	float	SoftEaseIn		( float x0, float x1, float t );
	float	SoftEaseOut		( float t );
	float	SoftEaseOut		( float x0, float x1, float t );
	float	SoftSmooth		( float t );
	float	SoftSmooth		( float x0, float x1, float t );
	
} // namespace USDist

#endif
