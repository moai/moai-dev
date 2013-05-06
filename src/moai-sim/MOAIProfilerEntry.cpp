// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfilerEntry.h>

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerEntry
//================================================================//

MOAIProfilerEntry::MOAIProfilerEntry ( MOAIProfilerEntry* parent, const ZLHashedString& name ) : 
	MOAIProfilerEntryBase ( parent, name ),
	mRelativeDuration ( 0 ),
	mFrameRelativeDuration ( 0 ),
	mHitCount ( 0 ),
	mTotalDurationMicroSec ( 0 ),
	mMinDurationMicroSec ( 0 ),
	mAvgDurationMicroSec ( 0 ),
	mMaxDurationMicroSec ( 0 ),
	mUnusedFrames ( 0 ) {

	memset ( mTotalDurationSamplesMicroSec, 0, sizeof( mTotalDurationSamplesMicroSec ) );
}
	
//----------------------------------------------------------------//
void MOAIProfilerEntry::Reset () {

	MOAIProfilerEntryBase::Reset ();

	mRelativeDuration = 0;
	mFrameRelativeDuration = 0;

	mHitCount = 0;

	mTotalDurationMicroSec = 0;
	memset ( mTotalDurationSamplesMicroSec, 0, sizeof( mTotalDurationSamplesMicroSec ) );

	mMinDurationMicroSec = UINT_MAX;
	mAvgDurationMicroSec = 0;
	mMaxDurationMicroSec = 0;

	mUnusedFrames = 0;
}

//----------------------------------------------------------------//
bool MOAIProfilerEntry::IsLessThan ( const MOAIProfilerEntryBase& otherBase ) const {

	const MOAIProfilerEntry& other = (const MOAIProfilerEntry&)otherBase;

	return mAvgDurationMicroSec < other.mAvgDurationMicroSec;
}

//----------------------------------------------------------------//
void MOAIProfilerEntry::CopyFrom ( const MOAIProfilerEntryBase& otherBase ) {

	MOAIProfilerEntryBase::CopyFrom ( otherBase );

	const MOAIProfilerEntry& other = (const MOAIProfilerEntry&)otherBase;

	mRelativeDuration = other.mRelativeDuration;
	mFrameRelativeDuration = other.mFrameRelativeDuration;

	mHitCount = other.mHitCount;

	mTotalDurationMicroSec = other.mTotalDurationMicroSec;
	memcpy ( mTotalDurationSamplesMicroSec, other.mTotalDurationSamplesMicroSec, sizeof( mTotalDurationSamplesMicroSec ) );

	mMinDurationMicroSec = other.mMinDurationMicroSec;
	mAvgDurationMicroSec = other.mAvgDurationMicroSec;
	mMaxDurationMicroSec = other.mMaxDurationMicroSec;

	mUnusedFrames = other.mUnusedFrames;
}

//----------------------------------------------------------------//
void MOAIProfilerEntry::SwapWith ( MOAIProfilerEntryBase& otherBase ) {
	
	MOAIProfilerEntry& other = (MOAIProfilerEntry&)otherBase;

	static MOAIProfilerEntry swap ( 0, ZLHashedString::Empty );

	swap.CopyFrom ( other );
	other.CopyFrom ( *this );
	CopyFrom ( swap );
}

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif

