// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfilerEntry.h>
#include <moai-sim/MOAIProfilerReport.h>

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerReport
//================================================================//

//----------------------------------------------------------------//
const u32	kMaxEntryPoolSize	= 256;
const u32	kPurgeThreshold		= MOAIProfilerEntry::NUM_SAMPLES;
const float kAverageUpdateAlpha = 0.01f;

//----------------------------------------------------------------//
MOAIProfilerReport::MOAIProfilerReport ( const ZLHashedString& name ) :
	MOAIProfilerReportBase ( name, kMaxEntryPoolSize ),
	mTotalDurationMicroSec ( 0 ),
	mReportFinalized ( false ) {

}
	
//----------------------------------------------------------------//
void MOAIProfilerReport::TraverseProfileEntries ( EntryCallbackPtr callback, void* userData ) {

	// Calculate the relative durations and sort the tree
	if ( !mReportFinalized ) {
		
		MOAIProfilerEntry* rootEntry = (MOAIProfilerEntry*)mRootEntry;
		rootEntry->mTotalDurationMicroSec = mTotalDurationMicroSec;

		_TraverseEntries ( mMetaRootEntry, &MOAIProfilerReport::Visitor_FinalizeEntries );

		mReportFinalized = true;
	}

	MOAIProfilerReportBase::TraverseProfileEntries ( callback, userData );
}

//----------------------------------------------------------------//
void MOAIProfilerReport::_OnBeginUpdate () {

	// Reset all per-frame values
	_TraverseEntries ( mRootEntry, &MOAIProfilerReport::Visitor_BeginUpdate );

	mTotalDurationMicroSec = 0;
	mReportFinalized = false;
}

//----------------------------------------------------------------//
void MOAIProfilerReport::_OnEnterScope ( MOAIProfilerEntryBase* entry ) {

	MOAIProfilerEntry* curEntry = (MOAIProfilerEntry*)entry;

	// Keep track how often the scope was entered
	curEntry->mHitCount++;

	// Make sure the entry isn't deleted yet
	curEntry->mUnusedFrames = 0;
}

//----------------------------------------------------------------//
void MOAIProfilerReport::_OnLeaveScope ( MOAIProfilerEntryBase* entry, const u64 startTimeMicroSec, const u32 durationMicroSec ) {
	
	UNUSED ( startTimeMicroSec );

	MOAIProfilerEntry* curEntry = ( MOAIProfilerEntry* )entry;

	// Set the per-frame values
	curEntry->mTotalDurationMicroSec += durationMicroSec;
	mTotalDurationMicroSec += durationMicroSec;
}

//----------------------------------------------------------------//
void MOAIProfilerReport::_OnEndUpdate () {
	
	// Delete entries that haven't been used for a while
	_TraverseEntries ( mRootEntry, &MOAIProfilerReport::Visitor_PurgeUnused );

	// Update the entry values and sort the entries
	_TraverseEntries ( mRootEntry, &MOAIProfilerReport::Visitor_UpdateEntries );
}

//----------------------------------------------------------------//
MOAIProfilerEntryBase* MOAIProfilerReport::_OnCreateNewEntry ( MOAIProfilerEntryBase* parent, const ZLHashedString& name ) {

	return new MOAIProfilerEntry ( (MOAIProfilerEntry*)parent, name );
}

//----------------------------------------------------------------//
u32 MOAIProfilerReport::Visitor_BeginUpdate ( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report ) {
	
	UNUSED ( report );

	MOAIProfilerEntry* curEntry = (MOAIProfilerEntry*)entry;

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
u32 MOAIProfilerReport::Visitor_PurgeUnused ( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report ) {
	
	UNUSED ( report );

	MOAIProfilerEntry* curEntry = (MOAIProfilerEntry*)entry;
	return curEntry->mUnusedFrames < kPurgeThreshold ? TRAVERSAL_CONTINUE : TRAVERSAL_DELETEENTRY;
}

//----------------------------------------------------------------//
u32 MOAIProfilerReport::Visitor_UpdateEntries ( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report ) {
	
	UNUSED ( report );

	MOAIProfilerEntry* curEntry = (MOAIProfilerEntry*)entry;

	if ( curEntry->mHitCount > 0 ) {

		// memcpy would be faster, but the data is aliased and the C++ standard says this behavior is undefined
		for ( int i = MOAIProfilerEntry::NUM_SAMPLES - 1; i > 0; i-- ) {

			curEntry->mTotalDurationSamplesMicroSec [ i ] = curEntry->mTotalDurationSamplesMicroSec [ i - 1 ];
		}
		curEntry->mTotalDurationSamplesMicroSec [ 0 ] = curEntry->mTotalDurationMicroSec;
	}

	return TRAVERSAL_CONTINUE;
}

//----------------------------------------------------------------//
u32 MOAIProfilerReport::Visitor_FinalizeEntries ( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report ) {
	
	MOAIProfilerEntry* curEntry = (MOAIProfilerEntry*)entry;
	MOAIProfilerReport* curReport = (MOAIProfilerReport*)report;

	// Update average and extremes using the recorded samples
	curEntry->mMinDurationMicroSec = UINT_MAX;
	curEntry->mMaxDurationMicroSec = 0;
	float averageDurationMicorSec = 0;
	float numValidSamples = 0;

	for ( u32 i = 0; i < MOAIProfilerEntry::NUM_SAMPLES; i++ ) {

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

		MOAIProfilerEntry* childEntry = (MOAIProfilerEntry*)curEntry->mFirstChild;
		while ( childEntry ) {
			childEntry->mRelativeDuration = (float) childEntry->mTotalDurationMicroSec * oneOverDuration;
			childEntry = (MOAIProfilerEntry*)childEntry->mNext;
		}

		entry->SortChildren ();
	}

	return TRAVERSAL_CONTINUE;
}

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif
