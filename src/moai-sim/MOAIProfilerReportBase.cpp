// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfilerReportBase.h>

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerReportBase
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerReportBase::BeginUpdate () {

	if ( mRootEntry == 0 ) {
	
		// Setup the root nodes
		mRootEntry = _CreateEntry ( 0, ZLHashedString ( "_root" ) );
		mMetaRootEntry = _CreateEntry ( 0, ZLHashedString ( "_meta_root" ) );
		mMetaRootEntry->AddChildEntry ( mRootEntry );
	}

	_OnBeginUpdate ();

	// 'Bind' the root scope
	mCurrentEntry = mRootEntry;
}

//----------------------------------------------------------------//
MOAIProfilerEntryBase* MOAIProfilerReportBase::_CreateEntry ( MOAIProfilerEntryBase* parent, const ZLHashedString& name ) {
	
	MOAIProfilerEntryBase* result = 0;	

	if ( mEntryPool ) {
		
		result = mEntryPool;
		mEntryPool = mEntryPool->mNext;

		assert ( mEntryPoolSize > 0 );
		mEntryPoolSize--;
		
		// Reset values
		result->Reset ();

		result->mName = name;
		result->mDepth = parent ? parent->mDepth + 1 : 0;
		result->mParent = parent;
	}
	else {
		
		result = _OnCreateNewEntry ( parent, name );
	}

	if ( parent != 0 ) {

		parent->AddChildEntry ( result );
	}

	return result;
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::_DeletePool () {

	while ( mEntryPool ) {

		MOAIProfilerEntryBase* entry = mEntryPool;
		mEntryPool = mEntryPool->mNext;
		delete entry;
	}

	mEntryPoolSize = 0;
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::EndUpdate () {

	// Sanity check
	assert ( mCurrentEntry == mRootEntry );
	mCurrentEntry = 0;
	
	_OnEndUpdate ();
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::EnterScope ( const ZLHashedString& name ) {

	MOAIProfilerEntryBase* newRoot = 0;

	// Check if we had the same entry last frame too
	MOAIProfilerEntryBase* entry = mCurrentEntry->mFirstChild;
	while ( entry ) {

		if ( entry->mName == name ) {

			newRoot = entry;
			break;
		}

		entry = entry->mNext;
	}

	// It's a completely new entry, so let's add it
	if ( newRoot == 0 ) {

		newRoot = _CreateEntry ( mCurrentEntry, name );
	}

	_OnEnterScope ( newRoot );

	// Enter the new scope
	mCurrentEntry = newRoot;
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::LeaveScope ( const ZLHashedString& name, const u64 startTimeMicroSec, const u32 durationMicroSec ) {
	
	// Sanity check
	assert ( mCurrentEntry->mName == name );

	_OnLeaveScope ( mCurrentEntry, startTimeMicroSec, durationMicroSec );

	// Leave the current scope
	mCurrentEntry = mCurrentEntry->mParent;
}

//----------------------------------------------------------------//
MOAIProfilerReportBase::MOAIProfilerReportBase ( const ZLHashedString& name, const u32 maxEntryPoolSize ) :
	mName( name ),
	mRootEntry ( 0 ),
	mMetaRootEntry ( 0 ),
	mCurrentEntry ( 0 ),
	mCurrentReportCallback ( 0 ),
	mCurrentUserCallback ( 0 ),
	mCurrentUserData ( 0 ),
	mEntryPool ( 0 ),
	mMaxEntryPoolSize ( maxEntryPoolSize ),
	mEntryPoolSize ( 0 ) {
}

//----------------------------------------------------------------//
MOAIProfilerReportBase::~MOAIProfilerReportBase () {

	// Return all entries to the pool...
	_TraverseEntries ( mRootEntry, &MOAIProfilerReportBase::Visitor_RecycleEntries );
	// ...before deleting them
	_DeletePool ();

	// Delete the root nodes
	if ( this->mMetaRootEntry ) {
		delete mMetaRootEntry;
		mMetaRootEntry = 0;
	}

	if ( this->mRootEntry ) {
		delete mRootEntry;
		mRootEntry = 0;
	}
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::_RecylceEntry ( MOAIProfilerEntryBase* entry ) {

	if ( mEntryPoolSize >= mMaxEntryPoolSize ) {
		
		delete entry;
	}
	else {

		// Reset the hierarchy
		entry->mParent = 0;
		entry->mFirstChild = 0;
		entry->mLastChild = 0;

		// Add the entry to the pool
		entry->mNext = mEntryPool;
		mEntryPool = entry;

		mEntryPoolSize++;
	}
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::_TraverseEntries ( MOAIProfilerEntryBase* root,  MOAIProfilerReportBase::EntryVisitorPtr entryVisitor, bool depthFirst ) {
	
	if ( !root ) return;
	
	MOAIProfilerEntryBase* entry = root->mFirstChild;
	MOAIProfilerEntryBase* prevEntry = 0;
	while ( entry ) {

		MOAIProfilerEntryBase* nextEntry = entry->mNext;

		if ( depthFirst ) {
			// Depth-first traversal
			_TraverseEntries ( entry, entryVisitor );
		}

		// Call visitor callback
		u32 result = ( this->*entryVisitor )( entry );
		if ( result == TRAVERSAL_STOP ) {

			break;
		}
		else if ( result == TRAVERSAL_DELETEENTRY ) {

			// Remove entry and update iterator
			if ( entry == root->mFirstChild ) {
				if ( entry == root->mLastChild ) {
					root->mFirstChild = root->mLastChild = 0;
				}
				else {
					root->mFirstChild = entry->mNext;
				}
			}
			else {

				assert ( prevEntry->mNext == entry );
				prevEntry->mNext = entry->mNext;
				
				if ( entry == root->mLastChild ) {
					root->mLastChild = prevEntry;
				}
			}

			// One less child node
			root->mNumChildren--;

			// Recylce the sub-tree
			_TraverseEntries ( entry, &MOAIProfilerReportBase::Visitor_RecycleEntries );

			// Recycle the current entry, so that we can use it again later
			MOAIProfilerEntryBase* oldEntry = entry;
			entry = entry->mNext;
			_RecylceEntry ( oldEntry );
		}
		else {
		
			if ( !depthFirst ) {
				// Breadth-first traversal
				_TraverseEntries ( entry, entryVisitor, false );
			}

			prevEntry = entry;
			entry = nextEntry;
		}
	}
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::_TraverseEntries ( MOAIProfilerEntryBase* root, EntryReportCallbackPtr entryVisitor, bool depthFirst ) {

	mCurrentReportCallback = entryVisitor;

	_TraverseEntries ( root, &MOAIProfilerReportBase::Visitor_ReportCallback, depthFirst );

	mCurrentReportCallback = 0;
}

//----------------------------------------------------------------//
void MOAIProfilerReportBase::TraverseProfileEntries ( EntryCallbackPtr callback, void* userData ) {

	// Traverse the tree and invoke the given callback for each item
	mCurrentUserCallback = callback;
	mCurrentUserData = userData;

	_TraverseEntries ( mRootEntry, &MOAIProfilerReportBase::Visitor_UserCallback, false );
	
	mCurrentUserCallback = 0;
	mCurrentUserData = 0;
}

//----------------------------------------------------------------//
u32 MOAIProfilerReportBase::Visitor_RecycleEntries ( MOAIProfilerEntryBase* entry ) {

	_RecylceEntry ( entry );

	return TRAVERSAL_CONTINUE;
}

//----------------------------------------------------------------//
u32 MOAIProfilerReportBase::Visitor_ReportCallback ( MOAIProfilerEntryBase* entry ) {

	return mCurrentReportCallback ( entry, this );
}

//----------------------------------------------------------------//
u32 MOAIProfilerReportBase::Visitor_UserCallback ( MOAIProfilerEntryBase* entry ) {

	return mCurrentUserCallback ( entry, mCurrentUserData );
}

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif
