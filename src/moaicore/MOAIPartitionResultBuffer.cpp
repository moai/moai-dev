// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// IsoSortItem
//================================================================//
class IsoSortItem {
public:
	MOAIPartitionResult*	mResult;
	IsoSortItem*			mNext;
};

//================================================================//
// IsoSortList
//================================================================//
class IsoSortList {
public:

	IsoSortItem*	mHead;
	IsoSortItem*	mTail;
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mHead = 0;
		this->mTail = 0;
	}
	
	//----------------------------------------------------------------//
	IsoSortList () :
		mHead ( 0 ),
		mTail ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	inline IsoSortItem* PopFront () {
		IsoSortItem* item = this->mHead;
		if ( item ) {
			if ( item->mNext ) {
				this->mHead = item->mNext;
			}
			else {
				this->mHead = 0;
				this->mTail = 0;
			}
		}
		return item;
	}
	
	//----------------------------------------------------------------//
	inline void PushBack ( IsoSortItem& item, MOAIPartitionResult* result ) {
	
		item.mResult = result;
		item.mNext = 0;
	
		if ( this->mHead ) {
			this->mTail->mNext = &item;
			this->mTail = &item;
		}
		else {
			this->mHead = &item;
			this->mTail = &item;
		}
	}
	
	//----------------------------------------------------------------//
	inline void PushBack ( IsoSortList& list ) {
	
		if ( list.mHead ) {
			
			if ( this->mHead ) {
				this->mTail->mNext = list.mHead;
				this->mTail = list.mTail;
			}
			else {
				this->mHead = list.mHead;
				this->mTail = list.mTail;
			}
		}
	}
};

//================================================================//
// MOAIPartitionResultBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Clear () {

	this->mMainBuffer.Clear ();
	this->mSwapBuffer.Clear ();
	
	this->mResults = 0;
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
MOAIProp* MOAIPartitionResultBuffer::FindBest () {

	if ( !this->mTotalResults ) return 0;
	
	MOAIPartitionResult* best = &this->mMainBuffer [ 0 ];
	
	u32 bestKey = best->mKey;
	
	for ( u32 i = 1; i < this->mTotalResults; ++i ) {
		MOAIPartitionResult* compare = &this->mMainBuffer [ i ];
		
		u32 compKey = compare->mKey;
		if ( bestKey < compKey ) {
			best = compare;
			bestKey = compKey;
		}
	}
	return best->mProp;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::GenerateKeys ( u32 mode, float xScale, float yScale, float zScale, float priority ) {

	float floatSign = mode & SORT_FLAG_DESCENDING ? -1.0f : 1.0f;
	s32 intSign = ( int )floatSign;

	switch ( mode & SORT_MODE_MASK ) {
		
		case SORT_KEY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				this->mMainBuffer [ i ].mKey = this->mMainBuffer [ i ].mKey * intSign;
			}
			break;
		case SORT_PRIORITY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				s32 p = this->mMainBuffer [ i ].mPriority * intSign;
				this->mMainBuffer [ i ].mKey = ( u32 )(( p ^ 0x80000000 ) | ( p & 0x7fffffff ));
			}
			break;

		case SORT_X_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float x = this->mMainBuffer [ i ].mLoc.mX;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( x * floatSign );
			}
			break;
		
		case SORT_Y_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float y = this->mMainBuffer [ i ].mLoc.mY;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( y * floatSign );
			}
			break;
		
		case SORT_Z_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float z = this->mMainBuffer [ i ].mLoc.mZ;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( z * floatSign );
			}
			break;
		
		case SORT_VECTOR_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				MOAIPartitionResult& result = this->mMainBuffer [ i ];
				float axis = ( result.mLoc.mX * xScale ) + ( result.mLoc.mY * yScale ) + ( result.mLoc.mZ * zScale ) + (( float )result.mPriority * priority );
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( axis * floatSign );
			}
			break;
		
		case SORT_NONE:
		default:
			return;
	}
}

//----------------------------------------------------------------//
MOAIPartitionResultBuffer::MOAIPartitionResultBuffer () :
	mResults ( 0 ),
	mTotalResults ( 0 ) {
}

//----------------------------------------------------------------//
MOAIPartitionResultBuffer::~MOAIPartitionResultBuffer () {
}

//----------------------------------------------------------------//
MOAIPartitionResult* MOAIPartitionResultBuffer::PopResult () {

	if ( this->mTotalResults ) {
		return &this->mResults [ --this->mTotalResults ];
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushProps ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 total = this->mTotalResults;
	
	for ( u32 i = 0; i < total; ++i ) {
		this->mResults [ i ].mProp->PushLuaUserdata ( state );
   }
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushResult ( MOAIProp& prop, u32 key, int subPrimID, s32 priority, const USVec3D& loc, const USBox& bounds ) {

	u32 idx = this->mTotalResults++;
	
	if ( idx >= this->mMainBuffer.Size ()) {
		this->mMainBuffer.Grow ( idx + 1, BLOCK_SIZE );
	}
	
	MOAIPartitionResult& result = this->mMainBuffer [ idx ] ;
	
	result.mKey = key;
	
	result.mProp = &prop;
	result.mSubPrimID = subPrimID;
	result.mPriority = priority;
	
	result.mLoc = loc;
	result.mBounds = bounds;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Reset () {

	this->mResults = 0;
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::Sort ( u32 mode ) {

	this->mResults = this->mMainBuffer;

	if ( mode == SORT_NONE ) {
		return this->mTotalResults;
	}
	else if ( mode == SORT_ISO ) {
		return this->SortResultsIso ();
	}
	return this->SortResultsLinear ();
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::SortResultsIso () {

	IsoSortItem* sortBuffer = ( IsoSortItem* )alloca ( this->mTotalResults * sizeof ( IsoSortItem ));
	
	IsoSortList frontList;
	IsoSortList backList;
	IsoSortList dontCareList;
	IsoSortList list;
	
	// sort by priority
	for ( u32 i = 0; i < this->mTotalResults; ++i ) {
		
		frontList.Clear ();
		backList.Clear ();
		dontCareList.Clear ();
		
		// get the next prop to add
		MOAIPartitionResult* result0 = &this->mMainBuffer [ i ];
		const USBox& bounds0 = result0->mBounds;
		
		// check incoming prop against all others
		IsoSortItem* cursor = list.PopFront ();
		while ( cursor ) {
			IsoSortItem* item = cursor;
			cursor = list.PopFront ();
			
			MOAIPartitionResult* result1 = item->mResult;
			const USBox& bounds1 = result1->mBounds;
			
			// front flags
			bool f0 =(( bounds1.mMax.mX < bounds0.mMin.mX ) || ( bounds1.mMax.mY < bounds0.mMin.mY ) || ( bounds1.mMax.mZ < bounds0.mMin.mZ ));
			bool f1 =(( bounds0.mMax.mX < bounds1.mMin.mX ) || ( bounds0.mMax.mY < bounds1.mMin.mY ) || ( bounds0.mMax.mZ < bounds1.mMin.mZ ));
			
			if ( f1 == f0 ) {
				// if ambiguous, add to the don't care list
				dontCareList.PushBack ( *item, result1 );
			}
			else if ( f0 ) {
				// if prop0 is *clearly* in front of prop1, add prop1 to back list
				backList.PushBack ( dontCareList );
				backList.PushBack ( *item, result1 );
				dontCareList.Clear ();
			}
			else {
				// if prop0 is *clearly* behind prop1, add prop1 to front list
				frontList.PushBack ( dontCareList );
				frontList.PushBack ( *item, result1 );
				dontCareList.Clear ();
			}
		}
		
		list.Clear ();
		list.PushBack ( backList );
		list.PushBack ( sortBuffer [ i ], result0 );
		list.PushBack ( frontList );
		list.PushBack ( dontCareList );
	}
	
	// affirm the swap buffer
	if ( this->mSwapBuffer.Size () < this->mMainBuffer.Size ()) {
		this->mSwapBuffer.Init ( this->mMainBuffer.Size ());
	}
	
	IsoSortItem* cursor = list.mHead;
	for ( u32 i = 0; cursor; cursor = cursor->mNext, ++i ) {
		
		MOAIPartitionResult* result = cursor->mResult;
		
		this->mSwapBuffer [ i ] = *result;
		this->mSwapBuffer [ i ].mKey = i;
	}
	
	this->mResults = this->mSwapBuffer;
	return this->mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::SortResultsLinear () {

	this->mResults = this->mMainBuffer;

	// affirm the swap buffer
	if ( this->mSwapBuffer.Size () < this->mMainBuffer.Size ()) {
		this->mSwapBuffer.Init ( this->mMainBuffer.Size ());
	}
	
	// sort
	this->mResults = RadixSort32 < MOAIPartitionResult >( this->mMainBuffer, this->mSwapBuffer, this->mTotalResults );
	
	return this->mTotalResults;
}
