// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERREPORTBASE_H
#define	MOAIPROFILERREPORTBASE_H

#include <moai-sim/MOAIProfilerEntryBase.h>

//================================================================//
// MOAIProfilerReportBase
//================================================================//
class MOAIProfilerReportBase {
public:

	typedef u32			( *EntryCallbackPtr )			( MOAIProfilerEntryBase* entry, void* userData );
	typedef u32			( *EntryReportCallbackPtr )		( MOAIProfilerEntryBase* entry, MOAIProfilerReportBase* report );

private:

	typedef u32			( MOAIProfilerReportBase::*EntryVisitorPtr )			( MOAIProfilerEntryBase* entry );

	//----------------------------------------------------------------//
	void				_TraverseEntries				( MOAIProfilerEntryBase* root, EntryVisitorPtr entryVisitor, bool depthFirst = true );
	u32					Visitor_RecycleEntries			( MOAIProfilerEntryBase* entry );
	u32					Visitor_ReportCallback			( MOAIProfilerEntryBase* entry );
	u32					Visitor_UserCallback			( MOAIProfilerEntryBase* entry );
	
	//----------------------------------------------------------------//
	// Fix Visual Studio compiler warning
	MOAIProfilerReportBase& operator = ( const MOAIProfilerReportBase& other ) {
		UNUSED ( other );
		assert ( 0 );
		return *this;
	}
	
protected:
	
	ZLHashedString				mName;

	MOAIProfilerEntryBase*		mRootEntry;
	MOAIProfilerEntryBase*		mMetaRootEntry;
	MOAIProfilerEntryBase*		mCurrentEntry;

	EntryReportCallbackPtr		mCurrentReportCallback;

	EntryCallbackPtr			mCurrentUserCallback;
	void*						mCurrentUserData;

	MOAIProfilerEntryBase*		mEntryPool;
	const u32					mMaxEntryPoolSize;
	u32							mEntryPoolSize;
	
	//----------------------------------------------------------------//
	MOAIProfilerEntryBase*			_CreateEntry			( MOAIProfilerEntryBase* parent, const ZLHashedString& name );
	void							_DeletePool				();
	virtual void					_OnBeginUpdate			() = 0;
	virtual void					_OnEnterScope			( MOAIProfilerEntryBase* entry ) = 0;
	virtual void					_OnLeaveScope			( MOAIProfilerEntryBase* entry, const u64 startTimeMicroSec, const u32 durationMicroSec ) = 0;
	virtual void					_OnEndUpdate			() = 0;
	virtual MOAIProfilerEntryBase*	_OnCreateNewEntry		( MOAIProfilerEntryBase* parent, const ZLHashedString& name ) = 0;
	void							_RecylceEntry			( MOAIProfilerEntryBase* entry );
	void							_TraverseEntries		( MOAIProfilerEntryBase* root, EntryReportCallbackPtr entryVisitor, bool depthFirst = true );

	//----------------------------------------------------------------//
									MOAIProfilerReportBase	( const ZLHashedString& name, const u32 maxEntryPoolSize );

public:
	
	//----------------------------------------------------------------//
	enum {
		TRAVERSAL_CONTINUE,
		TRAVERSAL_STOP,
		TRAVERSAL_DELETEENTRY
	};
	
	GET_CONST ( ZLHashedString, Name, mName );
	
	//----------------------------------------------------------------//
	void				BeginUpdate					();
	void				EndUpdate					();
	void				EnterScope					( const ZLHashedString& name );
	void				LeaveScope					( const ZLHashedString& name, const u64 startTimeMicroSec, const u32 durationMicroSec );
	virtual 			~MOAIProfilerReportBase		();
	virtual void		TraverseProfileEntries		( EntryCallbackPtr callback, void* userData );
};

#endif
