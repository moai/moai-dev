// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERREPORT_H
#define	MOAIPROFILERREPORT_H

#include <moai-sim/MOAIProfilerReportBase.h>

//================================================================//
// MOAIProfilerReport
//================================================================//
class MOAIProfilerReport :
	public MOAIProfilerReportBase {
protected:
	
	u32					mTotalDurationMicroSec;
	bool				mReportFinalized;
	
	//----------------------------------------------------------------//
	void					_OnBeginUpdate			();
	void					_OnEnterScope			( MOAIProfilerEntryBase* entry );
	void					_OnLeaveScope			( MOAIProfilerEntryBase* entry, const u64 startTimeMicroSec, const u32 durationMicroSec );
	void					_OnEndUpdate			();
	MOAIProfilerEntryBase*	_OnCreateNewEntry		( MOAIProfilerEntryBase* parent, const ZLHashedString& name );

	//----------------------------------------------------------------//
	static u32			Visitor_BeginUpdate			( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report );
	static u32			Visitor_PurgeUnused			( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report );
	static u32			Visitor_UpdateEntries		( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report );
	static u32			Visitor_FinalizeEntries		( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report );

public:

	GET_CONST ( u32, TotalDurationMicroSec, mTotalDurationMicroSec );

	//----------------------------------------------------------------//
						MOAIProfilerReport			( const ZLHashedString& name );
	void				TraverseProfileEntries		( EntryCallbackPtr callback, void* userData );

};

#endif
