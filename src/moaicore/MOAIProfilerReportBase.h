// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERREPORTBASE_H
#define	MOAIPROFILERREPORTBASE_H

#include <moaicore/MOAIProfilerEntryBase.h>

#if USE_MOAI_PROFILER

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
	
	USHashedString				mName;

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
	MOAIProfilerEntryBase*			_CreateEntry			( MOAIProfilerEntryBase* parent, const USHashedString& name );
	void							_DeletePool				();
	virtual void					_OnBeginUpdate			() {}
	virtual void					_OnEnterScope			( MOAIProfilerEntryBase* entry ) { UNUSED ( entry ); }
	virtual void					_OnLeaveScope			( MOAIProfilerEntryBase* entry, const u32 durationMicroSec ) { UNUSED ( entry ); UNUSED ( durationMicroSec ); }
	virtual void					_OnEndUpdate			() {}
	virtual MOAIProfilerEntryBase*	_OnCreateNewEntry		( MOAIProfilerEntryBase* parent, const USHashedString& name ) = 0;
	void							_RecylceEntry			( MOAIProfilerEntryBase* entry );
	void							_TraverseEntries		( MOAIProfilerEntryBase* root, EntryReportCallbackPtr entryVisitor, bool depthFirst = true );

	//----------------------------------------------------------------//
									MOAIProfilerReportBase	( const USHashedString& name, const u32 maxEntryPoolSize );

public:
	
	//----------------------------------------------------------------//
	enum {
		TRAVERSAL_CONTINUE,
		TRAVERSAL_STOP,
		TRAVERSAL_DELETEENTRY
	};
	
	GET_CONST ( USHashedString, Name, mName );
	
	//----------------------------------------------------------------//
	void				BeginUpdate					();
	void				EndUpdate					();
	void				EnterScope					( const USHashedString& name );
	void				LeaveScope					( const USHashedString& name, const u32 durationMicroSec );
	virtual 			~MOAIProfilerReportBase		();
	virtual void		TraverseProfileEntries		( EntryCallbackPtr callback, void* userData );
};

#endif
#endif
