// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfilerEntryBase.h>

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerEntryBase
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerEntryBase::AddChildEntry ( MOAIProfilerEntryBase* child ) {

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
void MOAIProfilerEntryBase::CopyFrom ( const MOAIProfilerEntryBase& other ) {

	mName = other.mName;

	mDepth = other.mDepth;

	mParent = other.mParent;

	mFirstChild = other.mFirstChild;
	mLastChild = other.mLastChild;
	mNumChildren = other.mNumChildren;
}

//----------------------------------------------------------------//
void MOAIProfilerEntryBase::DebugChildren ( std::string& childNames ) const {

	childNames = "";

	u32 numChildren = 0;
	
	MOAIProfilerEntryBase* prevChild = 0;
	MOAIProfilerEntryBase* child = mFirstChild;
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
MOAIProfilerEntryBase::MOAIProfilerEntryBase ( MOAIProfilerEntryBase* parent, const ZLHashedString& name ) : 
	mNext ( 0 ),
	mName ( name ),
	mDepth ( parent ? parent->mDepth + 1 : 0 ),
	mParent ( parent ),
	mFirstChild ( 0 ),
	mLastChild ( 0 ),
	mNumChildren ( 0 ) {
}
	
//----------------------------------------------------------------//
void MOAIProfilerEntryBase::Reset () {

	mNext = 0;

	mName = ZLHashedString::Empty;

	mDepth = 0;

	mParent = 0;

	mFirstChild = 0;
	mLastChild = 0;
	mNumChildren = 0;
}

//----------------------------------------------------------------//
void MOAIProfilerEntryBase::SortChildren () {

	if ( mNumChildren > 1 ) {
		
		if ( mNumChildren > 2 ) {

			// Bubble sort is acceptable in this case, because there will be almost no swaps after
			// a few frames, so the algorithm basically just runs over the child nodes once.
			bool swapped = true;
			while ( swapped ) {

				swapped = false;
				MOAIProfilerEntryBase* child = mFirstChild;
				while ( child->mNext ) {
					
					MOAIProfilerEntryBase* left = child;
					MOAIProfilerEntryBase* right = child->mNext;

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

				MOAIProfilerEntryBase* temp = mLastChild;
				mLastChild = mFirstChild;
				mFirstChild = temp;

				mFirstChild->mNext = mLastChild;
				mLastChild->mNext = 0;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIProfilerEntryBase::SwapChildren ( MOAIProfilerEntryBase& left, MOAIProfilerEntryBase& right ) {

	// Patch the children, so they point to the correct parent
	MOAIProfilerEntryBase* subChild = left.mFirstChild;
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
	#pragma optimize ( "", off ) 
#endif
