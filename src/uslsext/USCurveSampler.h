// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USPIECEWISEBLDR_H
#define USPIECEWISEBLDR_H

class USAnimCurve;

//================================================================//
// USCurveSample
//================================================================//
class USCurveSample {
	public:

	bool	mIsKey;
	float	mTime;
	float	mValue;
	
	//----------------------------------------------------------------//
	USCurveSample () :
		mIsKey ( false ),
		mValue ( 0.0f ) {
	}
};

//================================================================//
// USCurveSampler
//================================================================//
class USCurveSampler {
public:

	bool			mIsCompressed;
	float			mMinTolerance;
	u32				mBuiltID;
	u32				mNumSamples;
	USCurveSample*	mSourceCurve;

	//----------------------------------------------------------------//
	void	Build				( USAnimCurve* piecewise );
	void	BuildKey			( USAnimCurve* piecewise, u32 t1, u32 t2 );
	void	CompressCurve		( float minTolerance );
	void	InitSourceCurve		( u32 numSamples );
	u32		PickKey				( u32 t1, u32 t2 );
	void	SetSample			( u32 id, float sample );
	void	SimplifyCurve		( u32 t1, u32 t2 );
			USCurveSampler		();
			~USCurveSampler		();
};

#endif