// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIProfileReportBase.h>

//================================================================//
// MOAIProfileReportBase
//================================================================//

#if USE_MOAI_PROFILER

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

//----------------------------------------------------------------//
MOAIProfileReportBase::MOAIProfileReportBase ( const USHashedString& name, const u32 maxEntryPoolSize ) :
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
MOAIProfileReportBase::~MOAIProfileReportBase () {

	// Return all entries to the pool...
	_TraverseEntries ( mRootEntry, &MOAIProfileReportBase::Visitor_RecycleEntries );
	// ...before deleting them
	_DeletePool ();

	// Delete the root nodes
	delete mMetaRootEntry;
	mMetaRootEntry = 0;

	delete mRootEntry;
	mRootEntry = 0;
}

//----------------------------------------------------------------//
void MOAIProfileReportBase::BeginUpdate () {

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
void MOAIProfileReportBase::EnterScope ( const USHashedString& name ) {

	MOAIProfileEntryBase* newRoot = 0;

	// Check if we had the same entry last frame too
	MOAIProfileEntryBase* entry = mCurrentEntry->mFirstChild;
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
void MOAIProfileReportBase::LeaveScope ( const USHashedString& name, const u32 durationMicroSec ) {
	
	// Sanity check
	assert ( mCurrentEntry->mName == name );

	_OnLeaveScope ( mCurrentEntry, durationMicroSec );

	// Leave the current scope
	mCurrentEntry = mCurrentEntry->mParent;
}

//----------------------------------------------------------------//
void MOAIProfileReportBase::EndUpdate () {

	// Sanity check
	assert ( mCurrentEntry == mRootEntry );
	mCurrentEntry = 0;
	
	_OnEndUpdate ();
}

//----------------------------------------------------------------//
void MOAIProfileReportBase::_TraverseEntries ( MOAIProfileEntryBase* root,  MOAIProfileReportBase::EntryVisitorPtr entryVisitor, bool depthFirst ) {
	
	MOAIProfileEntryBase* entry = root->mFirstChild;
	MOAIProfileEntryBase* prevEntry = 0;
	while ( entry ) {

		MOAIProfileEntryBase* nextEntry = entry->mNext;

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
			_TraverseEntries ( entry, &MOAIProfileReportBase::Visitor_RecycleEntries );

			// Recycle the current entry, so that we can use it again later
			MOAIProfileEntryBase* oldEntry = entry;
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
void MOAIProfileReportBase::_TraverseEntries ( MOAIProfileEntryBase* root, EntryReportCallbackPtr entryVisitor, bool depthFirst ) {

	mCurrentReportCallback = entryVisitor;

	_TraverseEntries ( root, &MOAIProfileReportBase::Visitor_ReportCallback, depthFirst );

	mCurrentReportCallback = 0;
}

//----------------------------------------------------------------//
void MOAIProfileReportBase::TraverseProfileEntries ( EntryCallbackPtr callback, void* userData ) {

	// Traverse the tree and invoke the given callback for each item
	mCurrentUserCallback = callback;
	mCurrentUserData = userData;

	_TraverseEntries ( mRootEntry, &MOAIProfileReportBase::Visitor_UserCallback, false );
	
	mCurrentUserCallback = 0;
	mCurrentUserData = 0;
}

//----------------------------------------------------------------//
u32 MOAIProfileReportBase::Visitor_RecycleEntries ( MOAIProfileEntryBase* entry ) {

	_RecylceEntry ( entry );

	return TRAVERSAL_CONTINUE;
}

//----------------------------------------------------------------//
u32 MOAIProfileReportBase::Visitor_ReportCallback ( MOAIProfileEntryBase* entry ) {

	return mCurrentReportCallback ( entry, this );
}

//----------------------------------------------------------------//
u32 MOAIProfileReportBase::Visitor_UserCallback ( MOAIProfileEntryBase* entry ) {

	return mCurrentUserCallback ( entry, mCurrentUserData );
}

//----------------------------------------------------------------//
MOAIProfileEntryBase* MOAIProfileReportBase::_CreateEntry ( MOAIProfileEntryBase* parent, const USHashedString& name ) {
	
	MOAIProfileEntryBase* result = 0;	

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
void MOAIProfileReportBase::_RecylceEntry ( MOAIProfileEntryBase* entry ) {

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
void MOAIProfileReportBase::_DeletePool () {

	while ( mEntryPool ) {

		MOAIProfileEntryBase* entry = mEntryPool;
		mEntryPool = mEntryPool->mNext;
		delete entry;
	}

	mEntryPoolSize = 0;
}

//================================================================//
// MOAIProfileEntryBase
//================================================================//

MOAIProfileEntryBase::MOAIProfileEntryBase ( MOAIProfileEntryBase* parent, const USHashedString& name ) : 
	mNext ( 0 ),
	mName ( name ),
	mDepth ( parent ? parent->mDepth + 1 : 0 ),
	mParent ( parent ),
	mFirstChild ( 0 ),
	mLastChild ( 0 ),
	mNumChildren ( 0 ) {
}
	
//----------------------------------------------------------------//
void MOAIProfileEntryBase::Reset () {

	mNext = 0;

	mName = USHashedString::Empty;

	mDepth = 0;

	mParent = 0;

	mFirstChild = 0;
	mLastChild = 0;
	mNumChildren = 0;
}

//----------------------------------------------------------------//
void MOAIProfileEntryBase::CopyFrom ( const MOAIProfileEntryBase& other ) {

	mName = other.mName;

	mDepth = other.mDepth;

	mParent = other.mParent;

	mFirstChild = other.mFirstChild;
	mLastChild = other.mLastChild;
	mNumChildren = other.mNumChildren;
}

//----------------------------------------------------------------//
void MOAIProfileEntryBase::AddChildEntry ( MOAIProfileEntryBase* child ) {

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
void MOAIProfileEntryBase::SortChildren () {

	if ( mNumChildren > 1 ) {
		
		if ( mNumChildren > 2 ) {

			// Bubble sort is acceptable in this case, because there will be almost no swaps after
			// a few frames, so the algorithm basically just runs over the child nodes once.
			bool swapped = true;
			while ( swapped ) {

				swapped = false;
				MOAIProfileEntryBase* child = mFirstChild;
				while ( child->mNext ) {
					
					MOAIProfileEntryBase* left = child;
					MOAIProfileEntryBase* right = child->mNext;

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

				MOAIProfileEntryBase* temp = mLastChild;
				mLastChild = mFirstChild;
				mFirstChild = temp;

				mFirstChild->mNext = mLastChild;
				mLastChild->mNext = 0;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIProfileEntryBase::DebugChildren ( std::string& childNames ) const {

	childNames = "";

	u32 numChildren = 0;
	
	MOAIProfileEntryBase* prevChild = 0;
	MOAIProfileEntryBase* child = mFirstChild;
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
void MOAIProfileEntryBase::SwapChildren ( MOAIProfileEntryBase& left, MOAIProfileEntryBase& right ) {

	// Patch the children, so they point to the correct parent
	MOAIProfileEntryBase* subChild = left.mFirstChild;
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