// Copyright (c) 2012 - 2013 Double Fine Productions, Inc. All Rights Reserved.

#ifndef	PROFILE_REPORT_BASE_H
#define	PROFILE_REPORT_BASE_H

#include <uslscore/STLList.h>
#include <uslscore/USAccessors.h>
#include <uslscore/USHashedString.h>
#include <uslscore/USProfileConfig.h>

//================================================================//
// USProfileEntryBase
//================================================================//

#if PROFILE_ENABLED

class USProfileEntryBase {

protected:
	
	//----------------------------------------------------------------//
								USProfileEntryBase ( USProfileEntryBase* parent, const USHashedString& name );

public:
	
	//----------------------------------------------------------------//
	virtual						~USProfileEntryBase () {}
	
	//----------------------------------------------------------------//
	virtual void				Reset ();

	void						AddChildEntry ( USProfileEntryBase* child );

	void						SortChildren ();
	virtual bool				IsLessThan ( const USProfileEntryBase& other ) const = 0;
	
protected:

	//----------------------------------------------------------------//
	virtual void				CopyFrom ( const USProfileEntryBase& other );
	virtual void				SwapWith ( USProfileEntryBase& other ) = 0;

private:
	
	//----------------------------------------------------------------//
	static void					SwapChildren ( USProfileEntryBase& left, USProfileEntryBase& right );
	
	//----------------------------------------------------------------//
	void						DebugChildren ( std::string& childNames ) const;
	
public:

	//----------------------------------------------------------------//
	USProfileEntryBase*			mNext;

	//----------------------------------------------------------------//
	USHashedString				mName;

	u16							mDepth;

	USProfileEntryBase*			mParent;

	USProfileEntryBase*			mFirstChild;
	USProfileEntryBase*			mLastChild;
	u32							mNumChildren;
};

//================================================================//
// USProfileReportBase
//================================================================//

class USProfileReportBase {

public:

	//----------------------------------------------------------------//
	enum {

		TRAVERSAL_CONTINUE,
		TRAVERSAL_STOP,
		TRAVERSAL_DELETEENTRY
	};

protected:

	//----------------------------------------------------------------//
								USProfileReportBase ( const USHashedString& name, const u32 maxEntryPoolSize );

public:
	
	//----------------------------------------------------------------//
	virtual 					~USProfileReportBase ();
	

	//----------------------------------------------------------------//
	void						BeginUpdate ();

	void						EnterScope ( const USHashedString& name );
	void						LeaveScope ( const USHashedString& name, const u32 durationMicroSec );

	void						EndUpdate ();

	//----------------------------------------------------------------//
	typedef u32					(*EntryCallbackPtr) ( USProfileEntryBase* entry, void* userData );
	virtual void				TraverseProfileEntries ( EntryCallbackPtr callback, void* userData );
	
	//----------------------------------------------------------------//
	GET_CONST ( USHashedString, Name, mName );

protected:

	//----------------------------------------------------------------//
	USProfileEntryBase*			_CreateEntry ( USProfileEntryBase* parent, const USHashedString& name );
	void						_RecylceEntry ( USProfileEntryBase* entry );
	void						_DeletePool ();

private:

	//----------------------------------------------------------------//
	u32							Visitor_RecycleEntries ( USProfileEntryBase* entry );
	u32							Visitor_ReportCallback ( USProfileEntryBase* entry );
	u32							Visitor_UserCallback ( USProfileEntryBase* entry );
	
	//----------------------------------------------------------------//
	typedef u32					(USProfileReportBase::*EntryVisitorPtr) ( USProfileEntryBase* entry );
	void						_TraverseEntries ( USProfileEntryBase* root, EntryVisitorPtr entryVisitor, bool depthFirst = true );
	
	//----------------------------------------------------------------//
	// Fix Visual Studio compiler warning
	USProfileReportBase&		operator = ( const USProfileReportBase& other ) { UNUSED ( other ); assert ( 0 ); return *this; }

protected:
	
	//----------------------------------------------------------------//
	typedef u32					(*EntryReportCallbackPtr) ( USProfileEntryBase* entry, USProfileReportBase* report );
	void						_TraverseEntries ( USProfileEntryBase* root, EntryReportCallbackPtr entryVisitor, bool depthFirst = true );
	
	//----------------------------------------------------------------//
	virtual void				_OnBeginUpdate () {}

	virtual void				_OnEnterScope ( USProfileEntryBase* entry ) { UNUSED ( entry ); }
	virtual void				_OnLeaveScope ( USProfileEntryBase* entry, const u32 durationMicroSec ) { UNUSED ( entry ); UNUSED ( durationMicroSec ); }

	virtual void				_OnEndUpdate () {}

	virtual USProfileEntryBase*	_OnCreateNewEntry ( USProfileEntryBase* parent, const USHashedString& name ) = 0;

	//----------------------------------------------------------------//
	USHashedString				mName;

	USProfileEntryBase*			mRootEntry;
	USProfileEntryBase*			mMetaRootEntry;
	USProfileEntryBase*			mCurrentEntry;

	EntryReportCallbackPtr		mCurrentReportCallback;

	EntryCallbackPtr			mCurrentUserCallback;
	void*						mCurrentUserData;

	USProfileEntryBase*			mEntryPool;
	const u32					mMaxEntryPoolSize;
	u32							mEntryPoolSize;
};

#endif

#endif
