// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILEREPORT_H
#define	MOAIPROFILEREPORT_H

#include <moaicore/MOAIProfileReportBase.h>

//================================================================//
// MOAIProfileEntry
//================================================================//

#if USE_MOAI_PROFILER

class MOAIProfileEntry : public MOAIProfileEntryBase {

public:

	enum {

		NUM_SAMPLES = 32
	};

	//----------------------------------------------------------------//
						MOAIProfileEntry ( MOAIProfileEntry* parent, const USHashedString& name );
						
	//----------------------------------------------------------------//
	void				Reset ();
	
	bool				IsLessThan ( const MOAIProfileEntryBase& other ) const;
	
protected:

	//----------------------------------------------------------------//
	void				CopyFrom ( const MOAIProfileEntryBase& other );
	void				SwapWith ( MOAIProfileEntryBase& other );

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
// MOAIProfileReport
//================================================================//

class MOAIProfileReport : public MOAIProfileReportBase {

public:

	//----------------------------------------------------------------//
						MOAIProfileReport ( const USHashedString& name );
	
	//----------------------------------------------------------------//
	void				TraverseProfileEntries ( EntryCallbackPtr callback, void* userData );

	//----------------------------------------------------------------//
	GET_CONST ( u32, TotalDurationMicroSec, mTotalDurationMicroSec );

protected:
	
	//----------------------------------------------------------------//
	void				_OnBeginUpdate ();

	void				_OnEnterScope ( MOAIProfileEntryBase* entry );
	void				_OnLeaveScope ( MOAIProfileEntryBase* entry, const u32 durationMicroSec );

	void				_OnEndUpdate ();

	MOAIProfileEntryBase*	_OnCreateNewEntry ( MOAIProfileEntryBase* parent, const USHashedString& name );

	//----------------------------------------------------------------//
	static u32			Visitor_BeginUpdate ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report );
	static u32			Visitor_PurgeUnused ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report );
	static u32			Visitor_UpdateEntries ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report );
	static u32			Visitor_FinalizeEntries ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report );

	//----------------------------------------------------------------//
	u32					mTotalDurationMicroSec;

	bool				mReportFinalized;
};

#endif

#endif
