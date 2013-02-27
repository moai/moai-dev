// Copyright (c) 2012 - 2013 Double Fine Productions, Inc. All Rights Reserved.

#ifndef	PROFILE_REPORT_H
#define	PROFILE_REPORT_H

#include <uslscore/USProfileReportBase.h>

//================================================================//
// USProfileEntry
//================================================================//

#if PROFILE_ENABLED

class USProfileEntry : public USProfileEntryBase {

public:

	enum {

		NUM_SAMPLES = 32
	};

	//----------------------------------------------------------------//
						USProfileEntry ( USProfileEntry* parent, const USHashedString& name );
						
	//----------------------------------------------------------------//
	void				Reset ();
	
	bool				IsLessThan ( const USProfileEntryBase& other ) const;
	
protected:

	//----------------------------------------------------------------//
	void				CopyFrom ( const USProfileEntryBase& other );
	void				SwapWith ( USProfileEntryBase& other );

public:
						
	//----------------------------------------------------------------//
	float				mRelativeDuration;
	float				mFrameRelativeDuration;

	u32					mHitCount;

	u32					mTotalDurationMicroSec;
	u32					mTotalDurationSamplesMicroSec [ NUM_SAMPLES ];

	u32					mMinDurationMicroSec;
	float				mAvgDurationMicroSec;
	u32					mMaxDurationMicroSec;

	u32					mUnusedFrames;
};

//================================================================//
// USProfileReport
//================================================================//

class USProfileReport : public USProfileReportBase {

public:

	//----------------------------------------------------------------//
						USProfileReport ( const USHashedString& name );
	
	//----------------------------------------------------------------//
	void				TraverseProfileEntries ( EntryCallbackPtr callback, void* userData );

	//----------------------------------------------------------------//
	GET_CONST ( u32, TotalDurationMicroSec, mTotalDurationMicroSec );

protected:
	
	//----------------------------------------------------------------//
	void				_OnBeginUpdate ();

	void				_OnEnterScope ( USProfileEntryBase* entry );
	void				_OnLeaveScope ( USProfileEntryBase* entry, const u64 startTimeMicroSec, const u32 durationMicroSec );

	void				_OnEndUpdate ();

	USProfileEntryBase*	_OnCreateNewEntry ( USProfileEntryBase* parent, const USHashedString& name );

	//----------------------------------------------------------------//
	static u32			Visitor_BeginUpdate ( USProfileEntryBase* entry, USProfileReportBase* report );
	static u32			Visitor_PurgeUnused ( USProfileEntryBase* entry, USProfileReportBase* report );
	static u32			Visitor_UpdateEntries ( USProfileEntryBase* entry, USProfileReportBase* report );
	static u32			Visitor_FinalizeEntries ( USProfileEntryBase* entry, USProfileReportBase* report );

	//----------------------------------------------------------------//
	u32					mTotalDurationMicroSec;

	bool				mReportFinalized;
};

#endif

#endif
