// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINTERPOLATE_H
#define	ZLINTERPOLATE_H

//================================================================//
// ZLInterpolate
//================================================================//
namespace ZLInterpolate {
	
	enum {
		kEaseIn,
		kEaseOut,
		kExtraSharpEaseIn,
		kExtraSharpEaseOut,
		kExtraSharpSmooth,
		kExtraSharpSmoothEaseOut,
		kFlat,
		kLinear,
		kSharpEaseIn,
		kSharpEaseOut,
		kSharpSmooth,
		kSharpSmoothEaseOut,
		kSmooth,
		kSmoothEaseOut,
		kSoftEaseIn,
		kSoftEaseOut,
		kSoftSmooth,
		kSoftSmoothEaseOut,
		kSineEaseIn,
		kSineEaseOut,
		kSineSmooth,
		kCircEaseIn,
		kCircEaseOut,
		kCircSmooth,
		kBounceIn,
		kBounceOut,
		kBounceSmooth,
		kElasticIn,
		kElasticOut,
		kElasticSmooth,
		kBackEaseIn,
		kBackEaseOut,
		kBackSmooth,
	};
	
	//----------------------------------------------------------------//
	float	Curve			( u32 mode, float t );
	float	Curve			( u32 mode, float t, float w );
	float	Interpolate		( u32 mode, float x0, float x1, float t );
	float	Interpolate		( u32 mode, float x0, float x1, float t, float w );
	
} // namespace ZLDist

#endif
