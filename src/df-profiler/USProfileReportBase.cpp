// Copyright (c) 2012 - 2013 Double Fine Productions, Inc. All Rights Reserved.

#include "pch.h"
#include <uslscore/USProfileReportBase.h>

//================================================================//
// USProfileReportBase
//================================================================//

#if PROFILE_ENABLED

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

//----------------------------------------------------------------//
USProfileReportBase::USProfileReportBase ( const USHashedString& name, const u32 maxEntryPoolSize ) :
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
USProfileReportBase::~USProfileReportBase () {

	// Return all entries to the pool...
	_TraverseEntries ( mRootEntry, &USProfileReportBase::Visitor_RecycleEntries );
	// ...before deleting them
	_DeletePool ();

	// Delete the root nodes
	delete mMetaRootEntry;
	mMetaRootEntry = 0;

	delete mRootEntry;
	mRootEntry = 0;
}

//----------------------------------------------------------------//
void USProfileReportBase::BeginUpdate () {

	if ( mRootEntry == 0 ) {
	
		// Setup the root nodes
		mRootEntry = _CreateEntry ( 0, USHashedString ( "_root" ) );
		mMetaRootEntry = _CreateEntry ( 0, USHashedString ( "_meta_root" ) );
		mMetaRootEntry->AddChildEntry ( mRootEntry );
	}

	_OnBeginUpdate ();

	// 'Bind' the root scope
	mCurrentEntry = mRootEntry;
}

//----------------------------------------------------------------//
void USProfileReportBase::EnterScope ( const USHashedString& name ) {

	USProfileEntryBase* newRoot = 0;

	// Check if we had the same entry last frame too
	USProfileEntryBase* entry = mCurrentEntry->mFirstChild;
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
void USProfileReportBase::LeaveScope ( const USHashedString& name, const u64 startTimeMicroSec, const u32 durationMicroSec ) {
	
	// Sanity check
	assert ( mCurrentEntry->mName == name );

	_OnLeaveScope ( mCurrentEntry, startTimeMicroSec, durationMicroSec );

	// Leave the current scope
	mCurrentEntry = mCurrentEntry->mParent;
}

//----------------------------------------------------------------//
void USProfileReportBase::EndUpdate () {

	// Sanity check
	assert ( mCurrentEntry == mRootEntry );
	mCurrentEntry = 0;
	
	_OnEndUpdate ();
}

//----------------------------------------------------------------//
void USProfileReportBase::_TraverseEntries ( USProfileEntryBase* root,  USProfileReportBase::EntryVisitorPtr entryVisitor, bool depthFirst ) {
	
	USProfileEntryBase* entry = root->mFirstChild;
	USProfileEntryBase* prevEntry = 0;
	while ( entry ) {

		USProfileEntryBase* nextEntry = entry->mNext;

		if ( depthFirst ) {
			// Depth-first traversal
			_TraverseEntries ( entry, entryVisitor );
		}

		// Call visitor callback
		u32 result = (this->*entryVisitor) ( entry );
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
			_TraverseEntries ( entry, &USProfileReportBase::Visitor_RecycleEntries );

			// Recycle the current entry, so that we can use it again later
			USProfileEntryBase* oldEntry = entry;
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
void USProfileReportBase::_TraverseEntries ( USProfileEntryBase* root, EntryReportCallbackPtr entryVisitor, bool depthFirst ) {

	mCurrentReportCallback = entryVisitor;

	_TraverseEntries ( root, &USProfileReportBase::Visitor_ReportCallback, depthFirst );

	mCurrentReportCallback = 0;
}

//----------------------------------------------------------------//
void USProfileReportBase::TraverseProfileEntries ( EntryCallbackPtr callback, void* userData ) {

	// Traverse the tree and invoke the given callback for each item
	mCurrentUserCallback = callback;
	mCurrentUserData = userData;

	_TraverseEntries ( mRootEntry, &USProfileReportBase::Visitor_UserCallback, false );
	
	mCurrentUserCallback = 0;
	mCurrentUserData = 0;
}

//----------------------------------------------------------------//
u32 USProfileReportBase::Visitor_RecycleEntries ( USProfileEntryBase* entry ) {

	_RecylceEntry ( entry );

	return TRAVERSAL_CONTINUE;
}

//----------------------------------------------------------------//
u32 USProfileReportBase::Visitor_ReportCallback ( USProfileEntryBase* entry ) {

	return mCurrentReportCallback ( entry, this );
}

//----------------------------------------------------------------//
u32 USProfileReportBase::Visitor_UserCallback ( USProfileEntryBase* entry ) {

	return mCurrentUserCallback ( entry, mCurrentUserData );
}

//----------------------------------------------------------------//
USProfileEntryBase* USProfileReportBase::_CreateEntry ( USProfileEntryBase* parent, const USHashedString& name ) {
	
	USProfileEntryBase* result = 0;	

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
void USProfileReportBase::_RecylceEntry ( USProfileEntryBase* entry ) {

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
void USProfileReportBase::_DeletePool () {

	while ( mEntryPool ) {

		USProfileEntryBase* entry = mEntryPool;
		mEntryPool = mEntryPool->mNext;
		delete entry;
	}

	mEntryPoolSize = 0;
}

//================================================================//
// USProfileEntryBase
//================================================================//

USProfileEntryBase::USProfileEntryBase ( USProfileEntryBase* parent, const USHashedString& name ) : 
	mNext ( 0 ),
	mName ( name ),
	mDepth ( parent ? parent->mDepth + 1 : 0 ),
	mParent ( parent ),
	mFirstChild ( 0 ),
	mLastChild ( 0 ),
	mNumChildren ( 0 ) {
}
	
//----------------------------------------------------------------//
void USProfileEntryBase::Reset () {

	mNext = 0;

	mName = USHashedString::Empty;

	mDepth = 0;

	mParent = 0;

	mFirstChild = 0;
	mLastChild = 0;
	mNumChildren = 0;
}

//----------------------------------------------------------------//
void USProfileEntryBase::CopyFrom ( const USProfileEntryBase& other ) {

	mName = other.mName;

	mDepth = other.mDepth;

	mParent = other.mParent;

	mFirstChild = other.mFirstChild;
	mLastChild = other.mLastChild;
	mNumChildren = other.mNumChildren;
}

//----------------------------------------------------------------//
void USProfileEntryBase::AddChildEntry ( USProfileEntryBase* child ) {

	if ( mLastChild ) {
		mLastChild->mNext = child;
	}
	else {
		mFirstChild = child;
	}

	mLastChild = child;
	
	assert ( mFirstChild != 0 );
	assert ( mLastChild->mNext == 0 );

	mNumChildren++;
}

//----------------------------------------------------------------//
void USProfileEntryBase::SortChildren () {

	if ( mNumChildren > 1 ) {
		
		if ( mNumChildren > 2 ) {

			// Bubble sort is acceptable in this case, because there will be almost no swaps after
			// a few frames, so the algorithm basically just runs over the child nodes once.
			bool swapped = true;
			while ( swapped ) {

				swapped = false;
				USProfileEntryBase* child = mFirstChild;
				while ( child->mNext ) {
					
					USProfileEntryBase* left = child;
					USProfileEntryBase* right = child->mNext;

					child = child->mNext;

					if ( left->IsLessThan( *right ) ) {

						left->SwapWith ( *right );
						SwapChildren ( *left, *right );
						swapped = true;
					}
				}
			}
		}
		else {

			if ( mFirstChild->IsLessThan ( *mLastChild ) ) {

				USProfileEntryBase* temp = mLastChild;
				mLastChild = mFirstChild;
				mFirstChild = temp;

				mFirstChild->mNext = mLastChild;
				mLastChild->mNext = 0;
			}
		}
	}
}

//----------------------------------------------------------------//
void USProfileEntryBase::DebugChildren ( std::string& childNames ) const {

	childNames = "";

	u32 numChildren = 0;
	
	USProfileEntryBase* prevChild = 0;
	USProfileEntryBase* child = mFirstChild;
	while ( child ) {

		if ( child != mFirstChild ) {
			childNames += " -> ";
		}
		childNames += child->mName.GetString();

		numChildren++;

		prevChild = child;
		child = child->mNext;
	}

	assert ( mNumChildren == numChildren );
	assert ( mLastChild == prevChild );
}

//----------------------------------------------------------------//
void USProfileEntryBase::SwapChildren ( USProfileEntryBase& left, USProfileEntryBase& right ) {

	// Patch the children, so they point to the correct parent
	USProfileEntryBase* subChild = left.mFirstChild;
	while ( subChild ) {
		subChild->mParent = &left;
		subChild = subChild->mNext;
	}

	subChild = right.mFirstChild;
	while ( subChild ) {
		subChild->mParent = &right;
		subChild = subChild->mNext;
	}
}

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif

#endif