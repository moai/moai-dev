// Copyright (c) 2012 Double Fine Productions, Inc. All Rights Reserved.

#include "pch.h"
#include <uslscore/USProfileReport.h>

//================================================================//
// USProfileReport
//================================================================//

#if PROFILE_ENABLED

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

//----------------------------------------------------------------//
const u32	kMaxEntryPoolSize	= 256;
const u32	kPurgeThreshold		= USProfileEntry::NUM_SAMPLES;
const float kAverageUpdateAlpha = 0.01f;

//----------------------------------------------------------------//
USProfileReport::USProfileReport ( const USHashedString& name ) :
	USProfileReportBase ( name, kMaxEntryPoolSize ),
	mTotalDurationMicroSec ( 0 ),
	mReportFinalized ( false ) {

}
	
//----------------------------------------------------------------//
void USProfileReport::TraverseProfileEntries ( EntryCallbackPtr callback, void* userData ) {

	// Calculate the relative durations and sort the tree
	if ( !mReportFinalized ) {
		
		USProfileEntry* rootEntry = (USProfileEntry*)mRootEntry;
		rootEntry->mTotalDurationMicroSec = mTotalDurationMicroSec;

		_TraverseEntries ( mMetaRootEntry, &USProfileReport::Visitor_FinalizeEntries );

		mReportFinalized = true;
	}

	USProfileReportBase::TraverseProfileEntries ( callback, userData );
}

//----------------------------------------------------------------//
void USProfileReport::_OnBeginUpdate () {

	// Reset all per-frame values
	_TraverseEntries ( mRootEntry, &USProfileReport::Visitor_BeginUpdate );

	mTotalDurationMicroSec = 0;
	mReportFinalized = false;
}

//----------------------------------------------------------------//
void USProfileReport::_OnEnterScope ( USProfileEntryBase* entry ) {

	USProfileEntry* curEntry = (USProfileEntry*)entry;

	// Keep track how often the scope was entered
	curEntry->mHitCount++;

	// Make sure the entry isn't deleted yet
	curEntry->mUnusedFrames = 0;
}

//----------------------------------------------------------------//
void USProfileReport::_OnLeaveScope ( USProfileEntryBase* entry, const u32 durationMicroSec ) {
	
	USProfileEntry* curEntry = (USProfileEntry*)entry;

	// Set the per-frame values
	curEntry->mTotalDurationMicroSec += durationMicroSec;
	mTotalDurationMicroSec += durationMicroSec;
}

//----------------------------------------------------------------//
void USProfileReport::_OnEndUpdate () {
	
	// Delete entries that haven't been used for a while
	_TraverseEntries ( mRootEntry, &USProfileReport::Visitor_PurgeUnused );

	// Update the entry values and sort the entries
	_TraverseEntries ( mRootEntry, &USProfileReport::Visitor_UpdateEntries );
}

//----------------------------------------------------------------//
USProfileEntryBase* USProfileReport::_OnCreateNewEntry ( USProfileEntryBase* parent, const USHashedString& name ) {

	return new USProfileEntry ( (USProfileEntry*)parent, name );
}

//----------------------------------------------------------------//
u32 USProfileReport::Visitor_BeginUpdate ( USProfileEntryBase* entry, USProfileReportBase* report ) {
	
	UNUSED ( report );

	USProfileEntry* curEntry = (USProfileEntry*)entry;

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
u32 USProfileReport::Visitor_PurgeUnused ( USProfileEntryBase* entry, USProfileReportBase* report ) {
	
	UNUSED ( report );

	USProfileEntry* curEntry = (USProfileEntry*)entry;
	return curEntry->mUnusedFrames < kPurgeThreshold ? TRAVERSAL_CONTINUE : TRAVERSAL_DELETEENTRY;
}

//----------------------------------------------------------------//
u32 USProfileReport::Visitor_UpdateEntries ( USProfileEntryBase* entry, USProfileReportBase* report ) {
	
	UNUSED ( report );

	USProfileEntry* curEntry = (USProfileEntry*)entry;

	if ( curEntry->mHitCount > 0 ) {

		// memcpy would be faster, but the data is aliased and the C++ standard says this behavior is undefined
		for ( int i = USProfileEntry::NUM_SAMPLES - 1; i > 0; i-- ) {

			curEntry->mTotalDurationSamplesMicroSec [ i ] = curEntry->mTotalDurationSamplesMicroSec [ i - 1 ];
		}
		curEntry->mTotalDurationSamplesMicroSec [ 0 ] = curEntry->mTotalDurationMicroSec;
	}

	return TRAVERSAL_CONTINUE;
}

//----------------------------------------------------------------//
u32 USProfileReport::Visitor_FinalizeEntries ( USProfileEntryBase* entry, USProfileReportBase* report ) {
	
	USProfileEntry* curEntry = (USProfileEntry*)entry;
	USProfileReport* curReport = (USProfileReport*)report;

	// Update average and extremes using the recorded samples
	curEntry->mMinDurationMicroSec = UINT_MAX;
	curEntry->mMaxDurationMicroSec = 0;
	float averageDurationMicorSec = 0;
	float numValidSamples = 0;

	for ( u32 i = 0; i < USProfileEntry::NUM_SAMPLES; i++ ) {

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

		USProfileEntry* childEntry = (USProfileEntry*)curEntry->mFirstChild;
		while ( childEntry ) {
			childEntry->mRelativeDuration = (float) childEntry->mTotalDurationMicroSec * oneOverDuration;
			childEntry = (USProfileEntry*)childEntry->mNext;
		}

		entry->SortChildren ();
	}

	return TRAVERSAL_CONTINUE;
}

//================================================================//
// USProfileEntry
//================================================================//

USProfileEntry::USProfileEntry ( USProfileEntry* parent, const USHashedString& name ) : 
	USProfileEntryBase ( parent, name ),
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
void USProfileEntry::Reset () {

	USProfileEntryBase::Reset ();

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
bool USProfileEntry::IsLessThan ( const USProfileEntryBase& otherBase ) const {

	const USProfileEntry& other = (const USProfileEntry&)otherBase;

	return mAvgDurationMicroSec < other.mAvgDurationMicroSec;
}

//----------------------------------------------------------------//
void USProfileEntry::CopyFrom ( const USProfileEntryBase& otherBase ) {

	const USProfileEntry& other = (const USProfileEntry&)otherBase;

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
void USProfileEntry::SwapWith ( USProfileEntryBase& otherBase ) {
	
	USProfileEntry& other = (USProfileEntry&)otherBase;

	static USProfileEntry swap ( 0, USHashedString::Empty );

	swap.CopyFrom ( other );
	other.CopyFrom ( *this );
	CopyFrom ( swap );
}

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif

#endif