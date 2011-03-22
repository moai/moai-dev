// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USDistance.h>
#include <uslsext/USAnimCurve.h>
#include <uslsext/USCurveSampler.h>
#include <uslsext/USInterpolate.h>

//================================================================//
// USCurveSampler
//================================================================//

//----------------------------------------------------------------//
void USCurveSampler::Build ( USAnimCurve* piecewise ) {

	assert ( mIsCompressed );
	piecewise->Init ( mNumSamples );
	BuildKey ( piecewise, 0, mNumSamples - 1 );
}

//----------------------------------------------------------------//
void USCurveSampler::BuildKey ( USAnimCurve* piecewise, u32 t1, u32 t2 ) {

	if ( t1 == t2 ) {
		piecewise->SetKey ( mBuiltID++, mSourceCurve [ t1 ].mTime, mSourceCurve [ t1 ].mValue, USInterpolate::kLinear );
		return;
	}

	if (( t2 - t1 ) == 1 ) {
		piecewise->SetKey ( mBuiltID++, mSourceCurve [ t1 ].mTime, mSourceCurve [ t1 ].mValue, USInterpolate::kLinear );
		piecewise->SetKey ( mBuiltID++, mSourceCurve [ t2 ].mTime, mSourceCurve [ t2 ].mValue, USInterpolate::kLinear );
		return;
	}

	u32 mid = t1 + (( t2 - t1 ) >> 1 );
	
	piecewise->SetKey ( mBuiltID++, mSourceCurve [ mid ].mTime, mSourceCurve [ mid ].mValue, USInterpolate::kLinear );
	
	BuildKey ( piecewise, t1, mid - 1 );
	BuildKey ( piecewise, mid + 1, t2 );
}

//----------------------------------------------------------------//
void USCurveSampler::CompressCurve ( float minTolerance ) {

	assert ( !mIsCompressed );
	mIsCompressed = true;

	mMinTolerance = minTolerance;

	SimplifyCurve ( 0, mNumSamples - 1 );

	u32 top = 0;
	for ( u32 i = 0; i < mNumSamples; ++i ) {
		if ( mSourceCurve [ i ].mIsKey ) {
			if ( top != i ) mSourceCurve [ top ] = mSourceCurve [ i ];
			++top;
		}
	}
	
	mNumSamples = top;
}

//----------------------------------------------------------------//
void USCurveSampler::InitSourceCurve ( u32 numSamples ) {

	mNumSamples = numSamples;
	mSourceCurve = new USCurveSample [ numSamples ];
	assert ( mSourceCurve );
}

//----------------------------------------------------------------//
u32 USCurveSampler::PickKey ( u32 t1, u32 t2 ) {

	// The the end points of the line segment
	USVec3D b ( mSourceCurve [ t1 ].mTime, mSourceCurve [ t1 ].mValue, 0.0f );
	USVec3D m ( mSourceCurve [ t2 ].mTime, mSourceCurve [ t2 ].mValue, 0.0f );

	// Get the normalized ray of the line segment
	USVec3D bm = m;
	bm.Sub ( b );
	bm.Norm ();

    float sampleDiff;
    u32 bestSampleID = t1;
    float bestSampleDiff = mMinTolerance;

    for ( u32 i = t1; i < t2; ++i ) {

		// Get the sample point
		USVec3D p ( mSourceCurve [ i ].mTime, mSourceCurve [ i ].mValue, 0.0f );

		// Take the sample point into the line segment's space
		p.Sub ( b );

		// Project the sample point onto the ray
		float t = p.Dot ( bm );

		// Get the closest point on the ray to the sample point
		USVec3D q = bm;
		q.Scale ( t );

		// Find the distance between the two points
		sampleDiff = USDist::VecToVec ( p, q );
		
        if ( sampleDiff < 0.0f ) sampleDiff = -sampleDiff;
        if ( sampleDiff > bestSampleDiff ) {
            bestSampleDiff = sampleDiff;
            bestSampleID = i;
        }
    }

	return bestSampleID;
}

//----------------------------------------------------------------//
void USCurveSampler::SetSample ( u32 id, float sample ) {

	assert ( id < mNumSamples );
	mSourceCurve [ id ].mTime = ( float )id;
	mSourceCurve [ id ].mValue = sample;
}

//----------------------------------------------------------------//
void USCurveSampler::SimplifyCurve ( u32 t1, u32 t2 ) {

	mSourceCurve [ t1 ].mIsKey = true;
	mSourceCurve [ t2 ].mIsKey = true;

	if (( t2 - t1 ) < 2 ) return;

	u32 keyID = PickKey ( t1, t2 );
	
	if (( keyID == t1 ) || ( keyID == t2 )) return;
	
	SimplifyCurve ( t1, keyID );
	SimplifyCurve ( keyID, t2 );
}

//----------------------------------------------------------------//
USCurveSampler::USCurveSampler () :
	mIsCompressed ( false ),
	mBuiltID ( 0 ),
	mSourceCurve ( 0 ) {
}

//----------------------------------------------------------------//
USCurveSampler::~USCurveSampler () {
	if ( mSourceCurve ) delete mSourceCurve;
}
