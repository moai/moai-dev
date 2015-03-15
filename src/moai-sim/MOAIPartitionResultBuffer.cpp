// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIProp.h>

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
MOAIPartitionResult* MOAIPartitionResultBuffer::AffirmSwapBuffer () {

	this->mSwapBuffer->Grow ( this->mTotalResults, BLOCK_SIZE );
	return *this->mSwapBuffer;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Clear () {

	this->mBufferA.Clear ();
	this->mBufferB.Clear ();
	
	this->mResults = 0;
	
	this->mMainBuffer = 0;
	this->mSwapBuffer = 0;
	
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
MOAIProp* MOAIPartitionResultBuffer::FindBest () {

	if ( !this->mTotalResults ) return 0;
	
	MOAIPartitionResult* best = &this->mResults [ 0 ];
	
	u32 bestKey = best->mKey;
	
	for ( u32 i = 1; i < this->mTotalResults; ++i ) {
		MOAIPartitionResult* compare = &this->mResults [ i ];
		
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

	float floatSign = ( mode & SORT_FLAG_DESCENDING ) ? -1.0f : 1.0f;
	s32 intSign = ( int )floatSign;

	switch ( mode & SORT_MODE_MASK ) {
		
		case SORT_DIST_SQUARED_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				MOAIPartitionResult& result = this->mResults [ i ];
				
				float dist =
					(( result.mLoc.mX * result.mLoc.mX ) - ( xScale * xScale )) +
					(( result.mLoc.mY * result.mLoc.mY ) - ( yScale * yScale )) +
					(( result.mLoc.mZ * result.mLoc.mZ ) - ( zScale * zScale ));
				
				result.mKey = ZLFloat::FloatToIntKey ( dist * floatSign );
			}
			break;
		
		case SORT_KEY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				this->mResults [ i ].mKey = this->mResults [ i ].mKey * intSign;
			}
			break;
		
		case SORT_PRIORITY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				s32 p = this->mResults [ i ].mPriority * intSign;
				this->mResults [ i ].mKey = ( u32 )(( p ^ 0x80000000 ) | ( p & 0x7fffffff ));
			}
			break;

		case SORT_X_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float x = this->mResults [ i ].mLoc.mX;
				this->mResults [ i ].mKey = ZLFloat::FloatToIntKey ( x * floatSign );
			}
			break;
		
		case SORT_Y_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float y = this->mResults [ i ].mLoc.mY;
				this->mResults [ i ].mKey = ZLFloat::FloatToIntKey ( y * floatSign );
			}
			break;
		
		case SORT_Z_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float z = this->mResults [ i ].mLoc.mZ;
				this->mResults [ i ].mKey = ZLFloat::FloatToIntKey ( z * floatSign );
			}
			break;
		
		case SORT_VECTOR_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				MOAIPartitionResult& result = this->mResults [ i ];
				float axis = ( result.mLoc.mX * xScale ) + ( result.mLoc.mY * yScale ) + ( result.mLoc.mZ * zScale ) + (( float )result.mPriority * priority );
				result.mKey = ZLFloat::FloatToIntKey ( axis * floatSign );
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
	mMainBuffer ( 0 ),
	mSwapBuffer ( 0 ),
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
void MOAIPartitionResultBuffer::Project ( const ZLMatrix4x4& mtx ) {

	for ( u32 i = 0; i < this->mTotalResults; ++i ) {
		mtx.Project ( this->mResults [ i ].mLoc );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushProps ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 total = this->mTotalResults;

	// make sure there is enough stack space to push all props
	// the +1 is needed because pushing a prop requires an
	// additional value to be pushed onto the stack temporarily
	lua_checkstack ( L, total + 1 );

	for ( u32 i = 0; i < total; ++i ) {
		this->mResults [ i ].mProp->PushLuaUserdata ( state );
   }
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushResult ( MOAIProp& prop, u32 key, int subPrimID, s32 priority, const ZLVec3D& loc, const ZLBox& bounds ) {

	u32 idx = this->mTotalResults++;
	
	if ( idx >= this->mMainBuffer->Size ()) {
		this->mMainBuffer->Grow ( idx + 1, BLOCK_SIZE );
		this->mResults = this->mMainBuffer->Data ();
	}
	
	MOAIPartitionResult& result = this->mResults [ idx ] ;
	
	result.mKey = key;
	
	result.mProp = &prop;
	result.mSubPrimID = subPrimID;
	result.mPriority = priority;
	
	result.mLoc = loc;
	result.mBounds = bounds;
	
	ZLVec3D piv = prop.GetPiv ();
	result.mLoc.Add ( piv );
	result.mBounds.Offset ( piv );
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::SetResultsBuffer ( MOAIPartitionResult* buffer ) {

	assert (( buffer == this->mBufferA ) || ( buffer == this->mBufferB ));
	
	this->mResults = buffer;
	
	if ( buffer == this->mBufferA ) {
		this->mMainBuffer = &this->mBufferA;
		this->mSwapBuffer = &this->mBufferB;
	}
	else {
		this->mMainBuffer = &this->mBufferB;
		this->mSwapBuffer = &this->mBufferA;
	}
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Reset () {

	this->mBufferA.Grow ( 1, BLOCK_SIZE );
	this->SetResultsBuffer ( this->mBufferA );
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::Sort ( u32 mode ) {

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

	MOAIPartitionResult* mainBuffer = this->mResults;
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
		MOAIPartitionResult* result0 = &mainBuffer [ i ];
		const ZLBox& bounds0 = result0->mBounds;
		
		// check incoming prop against all others
		IsoSortItem* cursor = list.PopFront ();
		while ( cursor ) {
			IsoSortItem* item = cursor;
			cursor = list.PopFront ();
			
			MOAIPartitionResult* result1 = item->mResult;
			const ZLBox& bounds1 = result1->mBounds;
			
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
	MOAIPartitionResult* swapBuffer = this->AffirmSwapBuffer ();
	
	IsoSortItem* cursor = list.mHead;
	for ( u32 i = 0; cursor; cursor = cursor->mNext, ++i ) {
		MOAIPartitionResult* result = cursor->mResult;
		swapBuffer [ i ] = *result;
		swapBuffer [ i ].mKey = i;
	}
	
	this->SetResultsBuffer ( swapBuffer );
	return this->mTotalResults;
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::SortResultsLinear () {

	MOAIPartitionResult* swapBuffer = this->AffirmSwapBuffer ();
	MOAIPartitionResult* results = RadixSort32 < MOAIPartitionResult >( this->mResults, swapBuffer, this->mTotalResults );
	this->SetResultsBuffer ( results );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Transform ( const ZLMatrix4x4& mtx, bool transformBounds ) {

	if ( transformBounds ) {
		for ( u32 i = 0; i < this->mTotalResults; ++i ) {
			MOAIPartitionResult& result = this->mResults [ i ];
			mtx.Transform ( result.mLoc );
			result.mBounds.Transform ( mtx );
		}
	}
	else {
		for ( u32 i = 0; i < this->mTotalResults; ++i ) {
			mtx.Transform ( this->mResults [ i ].mLoc );
		}
	}
}
