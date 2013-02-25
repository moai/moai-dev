// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILEREPORTBASE_H
#define	MOAIPROFILEREPORTBASE_H

//================================================================//
// MOAIProfileEntryBase
//================================================================//

#if USE_MOAI_PROFILER

class MOAIProfileEntryBase {

protected:
	
	//----------------------------------------------------------------//
								MOAIProfileEntryBase ( MOAIProfileEntryBase* parent, const USHashedString& name );

public:
	
	//----------------------------------------------------------------//
	virtual						~MOAIProfileEntryBase () {}
	
	//----------------------------------------------------------------//
	virtual void				Reset ();

	void						AddChildEntry ( MOAIProfileEntryBase* child );

	void						SortChildren ();
	virtual bool				IsLessThan ( const MOAIProfileEntryBase& other ) const = 0;
	
protected:

	//----------------------------------------------------------------//
	virtual void				CopyFrom ( const MOAIProfileEntryBase& other );
	virtual void				SwapWith ( MOAIProfileEntryBase& other ) = 0;

private:
	
	//----------------------------------------------------------------//
	static void					SwapChildren ( MOAIProfileEntryBase& left, MOAIProfileEntryBase& right );
	
	//----------------------------------------------------------------//
	void						DebugChildren ( std::string& childNames ) const;
	
public:

	//----------------------------------------------------------------//
	MOAIProfileEntryBase*		mNext;

	//----------------------------------------------------------------//
	USHashedString				mName;

	u16							mDepth;

	MOAIProfileEntryBase*		mParent;

	MOAIProfileEntryBase*		mFirstChild;
	MOAIProfileEntryBase*		mLastChild;
	u32							mNumChildren;
};

//================================================================//
// MOAIProfileReportBase
//================================================================//

class MOAIProfileReportBase {

public:

	//----------------------------------------------------------------//
	enum {

		TRAVERSAL_CONTINUE,
		TRAVERSAL_STOP,
		TRAVERSAL_DELETEENTRY
	};

protected:

	//----------------------------------------------------------------//
								MOAIProfileReportBase ( const USHashedString& name, const u32 maxEntryPoolSize );

public:
	
	//----------------------------------------------------------------//
	virtual 					~MOAIProfileReportBase ();
	

	//----------------------------------------------------------------//
	void						BeginUpdate ();

	void						EnterScope ( const USHashedString& name );
	void						LeaveScope ( const USHashedString& name, const u32 durationMicroSec );

	void						EndUpdate ();

	//----------------------------------------------------------------//
	typedef u32					(*EntryCallbackPtr) ( MOAIProfileEntryBase* entry, void* userData );
	virtual void				TraverseProfileEntries ( EntryCallbackPtr callback, void* userData );
	
	//----------------------------------------------------------------//
	GET_CONST ( USHashedString, Name, mName );

protected:

	//----------------------------------------------------------------//
	MOAIProfileEntryBase*			_CreateEntry ( MOAIProfileEntryBase* parent, const USHashedString& name );
	void						_RecylceEntry ( MOAIProfileEntryBase* entry );
	void						_DeletePool ();

private:

	//----------------------------------------------------------------//
	u32							Visitor_RecycleEntries ( MOAIProfileEntryBase* entry );
	u32							Visitor_ReportCallback ( MOAIProfileEntryBase* entry );
	u32							Visitor_UserCallback ( MOAIProfileEntryBase* entry );
	
	//----------------------------------------------------------------//
	typedef u32					(MOAIProfileReportBase::*EntryVisitorPtr) ( MOAIProfileEntryBase* entry );
	void						_TraverseEntries ( MOAIProfileEntryBase* root, EntryVisitorPtr entryVisitor, bool depthFirst = true );
	
	//----------------------------------------------------------------//
	// Fix Visual Studio compiler warning
	MOAIProfileReportBase&		operator = ( const MOAIProfileReportBase& other ) { UNUSED ( other ); assert ( 0 ); return *this; }

protected:
	
	//----------------------------------------------------------------//
	typedef u32					(*EntryReportCallbackPtr) ( MOAIProfileEntryBase* entry, MOAIProfileReportBase* report );
	void						_TraverseEntries ( MOAIProfileEntryBase* root, EntryReportCallbackPtr entryVisitor, bool depthFirst = true );
	
	//----------------------------------------------------------------//
	virtual void				_OnBeginUpdate () {}

	virtual void				_OnEnterScope ( MOAIProfileEntryBase* entry ) { UNUSED ( entry ); }
	virtual void				_OnLeaveScope ( MOAIProfileEntryBase* entry, const u32 durationMicroSec ) { UNUSED ( entry ); UNUSED ( durationMicroSec ); }

	virtual void				_OnEndUpdate () {}

	virtual MOAIProfileEntryBase*	_OnCreateNewEntry ( MOAIProfileEntryBase* parent, const USHashedString& name ) = 0;

	//----------------------------------------------------------------//
	USHashedString				mName;

	MOAIProfileEntryBase*			mRootEntry;
	MOAIProfileEntryBase*			mMetaRootEntry;
	MOAIProfileEntryBase*			mCurrentEntry;

	EntryReportCallbackPtr		mCurrentReportCallback;

	EntryCallbackPtr			mCurrentUserCallback;
	void*						mCurrentUserData;

	MOAIProfileEntryBase*			mEntryPool;
	const u32					mMaxEntryPoolSize;
	u32							mEntryPoolSize;
};

#endif

#endif
