// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERENTRY_H
#define	MOAIPROFILERENTRY_H

#include <moai-sim/MOAIProfilerEntryBase.h>

//================================================================//
// MOAIProfilerEntry
//================================================================//
class MOAIProfilerEntry :
	public MOAIProfilerEntryBase {
protected:
	
	//----------------------------------------------------------------//
	void				CopyFrom		( const MOAIProfilerEntryBase& other );
	void				SwapWith		( MOAIProfilerEntryBase& other );
	
public:

	enum {
		NUM_SAMPLES = 32
	};

	float		mRelativeDuration;
	float		mFrameRelativeDuration;

	u32			mHitCount;

	u32			mTotalDurationMicroSec;
	u32			mTotalDurationSamplesMicroSec [ NUM_SAMPLES ];

	u32			mMinDurationMicroSec;
	float		mAvgDurationMicroSec;
	u32			mMaxDurationMicroSec;

	u32			mUnusedFrames;

	//----------------------------------------------------------------//
	bool				IsLessThan					( const MOAIProfilerEntryBase& other ) const;
						MOAIProfilerEntry			( MOAIProfilerEntry* parent, const ZLHashedString& name );
	void				Reset						();
};

#endif
