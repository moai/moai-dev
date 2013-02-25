// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIProfileReport.h>

//================================================================//
// MOAIProfileReport
//================================================================//

#if USE_MOAI_PROFILER

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

//----------------------------------------------------------------//
const u32	kMaxEntryPoolSize	= 256;
const u32	kPurgeThreshold		= MOAIProfileEntry::NUM_SAMPLES;
const float kAverageUpdateAlpha = 0.01f;

//----------------------------------------------------------------//
MOAIProfileReport::MOAIProfileReport ( const USHashedString& name ) :
	MOAIProfileReportBase ( name, kMaxEntryPoolSize ),
	mTotalDurationMicroSec ( 0 ),
	mReportFinalized ( false ) {

}
	
//----------------------------------------------------------------//
void MOAIProfileReport::TraverseProfileEntries ( EntryCallbackPtr callback, void* userData ) {

	// Calculate the relative durations and sort the tree
	if ( !mReportFinalized ) {
		
		MOAIProfileEntry* rootEntry = (MOAIProfileEntry*)mRootEntry;
		rootEntry->mTotalDurationMicroSec = mTotalDurationMicroSec;

		_TraverseEntries ( mMetaRootEntry, &MOAIProfileReport::Visitor_FinalizeEntries );

		mReportFinalized = true;
	}

	MOAIProfileReportBase::TraverseProfileEntries ( callback, userData );
}

//----------------------------------------------------------------//
void MOAIProfileReport::_OnBeginUpdate () {

	// Reset all per-frame values
	_TraverseEntries ( mRootEntry, &MOAIProfileReport::Visitor_BeginUpdate );

	mTotalDurationMicroSec = 0;
	mReportFinalized = false;
}

//----------------------------------------------------------------//
void MOAIProfileReport::_OnEnterScope ( MOAIProfileEntryBase* entry ) {

	MOAIProfileEntry* curEntry = (MOAIProfileEntry*)entry;

	// Keep track how often the scope was entered
	curEntry->mHitCount++;

	// Make sure the entry isn't deleted yet
	curEntry->mUnusedFrames = 0;
}

//----------------------------------------------------------------//
void MOAIProfileReport::_OnLeaveScope ( MOAIProfileEntryBase* entry, const u32 durationMicroSec ) {
	
	MOAIProfileEntry* curEntry = (MOAIProfileEntry*)entry;

	// Set the per-frame values
	curEntry->mTotalDurationMicroSec += durationMicroSec;
	mTotalDurationMicroSec += durationMicroSec;
}

//----------------------------------------------------------------//
void MOAIProfileReport::_OnEndUpdate () {
	
	// Delete entries that haven't been used for a while
	_TraverseEntries ( mRootEntry, &MOAIProfileReport::Visitor_PurgeUnused );

	// Update the entry values and sort the entries
	_TraverseEntries ( mRootEntry, &MOAIProfileReport::Visitor_UpdateEntries );
}

//----------------------------------------------------------------//
MOAIProfileEntryBase* MOAIProfileReport::_OnCreateNewEntry ( MOAIProfileEntryBase* parent, const USHashedString& name ) {

	return new MOAIProfileEntry ( (MOAIProfileEntry*)parent, name );
}

//----------------------------------------------------------------//
u32 MOAIProfileReport::Visitor_BeginUpdate ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report ) {
	
	UNUSED ( report );

	MOAIProfileEntry* curEntry = (MOAIProfileEntry*)entry;

	// Reset pre-frame values
	curEntry->mRelativeDuration = 0;
	curEntry->mFrameRelativeDuration = 0;
	curEntry->mHitCount = 0;
	curEntry->mTotalDurationMicroSec = 0;

	// Increment unsued-for-n-frames counter
	curEntry->mUnusedFrames++;

	return TRAVERSAL_CONTINUE;
}

//----------------------------------------------------------------//
u32 MOAIProfileReport::Visitor_PurgeUnused ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report ) {
	
	UNUSED ( report );

	MOAIProfileEntry* curEntry = (MOAIProfileEntry*)entry;
	return curEntry->mUnusedFrames < kPurgeThreshold ? TRAVERSAL_CONTINUE : TRAVERSAL_DELETEENTRY;
}

//----------------------------------------------------------------//
u32 MOAIProfileReport::Visitor_UpdateEntries ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report ) {
	
	UNUSED ( report );

	MOAIProfileEntry* curEntry = (MOAIProfileEntry*)entry;

	if ( curEntry->mHitCount > 0 ) {

		// memcpy would be faster, but the data is aliased and the C++ standard says this behavior is undefined
		for ( int i = MOAIProfileEntry::NUM_SAMPLES - 1; i > 0; i-- ) {

			curEntry->mTotalDurationSamplesMicroSec [ i ] = curEntry->mTotalDurationSamplesMicroSec [ i - 1 ];
		}
		curEntry->mTotalDurationSamplesMicroSec [ 0 ] = curEntry->mTotalDurationMicroSec;
	}

	return TRAVERSAL_CONTINUE;
}

//----------------------------------------------------------------//
u32 MOAIProfileReport::Visitor_FinalizeEntries ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report ) {
	
	MOAIProfileEntry* curEntry = (MOAIProfileEntry*)entry;
	MOAIProfileReport* curReport = (MOAIProfileReport*)report;

	// Update average and extremes using the recorded samples
	curEntry->mMinDurationMicroSec = UINT_MAX;
	curEntry->mMaxDurationMicroSec = 0;
	float averageDurationMicorSec = 0;
	float numValidSamples = 0;

	for ( u32 i = 0; i < MOAIProfileEntry::NUM_SAMPLES; i++ ) {

		u32 durationMicroSec = curEntry->mTotalDurationSamplesMicroSec [ i ];
		if ( durationMicroSec > 0 ) {

			numValidSamples++;

			curEntry->mMinDurationMicroSec = min ( curEntry->mMinDurationMicroSec, durationMicroSec );
			curEntry->mMaxDurationMicroSec = max ( curEntry->mMaxDurationMicroSec, durationMicroSec );
			averageDurationMicorSec += (float) durationMicroSec;
		}
	}

	if ( numValidSamples > 0 ) {

		averageDurationMicorSec /= numValidSamples;
		curEntry->mAvgDurationMicroSec = curEntry->mAvgDurationMicroSec * ( 1.0f - kAverageUpdateAlpha ) +  averageDurationMicorSec * kAverageUpdateAlpha;
	}
	
	// Calculate global precentage
	float totalDuration = (float) curEntry->mTotalDurationMicroSec;
	curEntry->mFrameRelativeDuration = totalDuration / (float) curReport->mTotalDurationMicroSec;

	// Calculate the relative duration of all the sub scopes
	if ( entry->mNumChildren > 0 ) {
		
		float oneOverDuration = totalDuration > 0 ? 1.0f / totalDuration : 0.0f;

		MOAIProfileEntry* childEntry = (MOAIProfileEntry*)curEntry->mFirstChild;
		while ( childEntry ) {
			childEntry->mRelativeDuration = (float) childEntry->mTotalDurationMicroSec * oneOverDuration;
			childEntry = (MOAIProfileEntry*)childEntry->mNext;
		}

		entry->SortChildren ();
	}

	return TRAVERSAL_CONTINUE;
}

//================================================================//
// MOAIProfileEntry
//================================================================//

MOAIProfileEntry::MOAIProfileEntry ( MOAIProfileEntry* parent, const USHashedString& name ) : 
	MOAIProfileEntryBase ( parent, name ),
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
void MOAIProfileEntry::Reset () {

	MOAIProfileEntryBase::Reset ();

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
bool MOAIProfileEntry::IsLessThan ( const MOAIProfileEntryBase& otherBase ) const {

	const MOAIProfileEntry& other = (const MOAIProfileEntry&)otherBase;

	return mAvgDurationMicroSec < other.mAvgDurationMicroSec;
}

//----------------------------------------------------------------//
void MOAIProfileEntry::CopyFrom ( const MOAIProfileEntryBase& otherBase ) {

	const MOAIProfileEntry& other = (const MOAIProfileEntry&)otherBase;

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
void MOAIProfileEntry::SwapWith ( MOAIProfileEntryBase& otherBase ) {
	
	MOAIProfileEntry& other = (MOAIProfileEntry&)otherBase;

	static MOAIProfileEntry swap ( 0, USHashedString::Empty );

	swap.CopyFrom ( other );
	other.CopyFrom ( *this );
	CopyFrom ( swap );
}

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif

#endif