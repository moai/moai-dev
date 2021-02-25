// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	ZLReal	Curve			( u32 mode, ZLReal t );
	ZLReal	Curve			( u32 mode, ZLReal t, ZLReal w );
	ZLReal	Interpolate		( u32 mode, ZLReal x0, ZLReal x1, ZLReal t );
	ZLReal	Interpolate		( u32 mode, ZLReal x0, ZLReal x1, ZLReal t, ZLReal w );
	
} // namespace ZLDist

#endif
