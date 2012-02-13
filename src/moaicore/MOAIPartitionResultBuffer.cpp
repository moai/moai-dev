// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// MOAIPartitionResultBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Clear () {

	this->mMainBuffer.Clear ();
	this->mSwapBuffer.Clear ();
	this->mProps.Clear ();
	
	this->mResults = 0;
	this->mTotalResults = 0;
	this->mTotalProps = 0;
}

//----------------------------------------------------------------//
MOAIProp* MOAIPartitionResultBuffer::FindBest ( u32 mode, float xScale, float yScale, float zScale, float priority ) {

	if ( !this->mTotalResults ) return 0;
	
	MOAIPartitionResult* best = &this->mResults [ 0 ];
	
	float floatSign = mode & SORT_FLAG_DESCENDING ? -1.0f : 1.0f;
	s32 intSign = ( int )floatSign;

	float bestFloat;
	int bestInt;

	switch ( mode & SORT_MODE_MASK ) {
  
		case SORT_PRIORITY_ASCENDING:
		
			bestInt = best->mPriority * intSign;
		
			for ( u32 i = 1; i < this->mTotalProps; ++i ) {
				MOAIPartitionResult* compare = &this->mResults [ i ];
				
				int compInt = compare->mPriority * intSign;
				if ( compInt > bestInt ) {
					best = compare;
					bestInt = compInt;
				}
			}
			break;
		
		case SORT_X_ASCENDING:
		
			bestFloat = best->mX * floatSign;
		
			for ( u32 i = 1; i < this->mTotalProps; ++i ) {
				MOAIPartitionResult* compare = &this->mResults [ i ];
				
				float compFloat = compare->mX * floatSign;
				if ( compFloat > bestFloat ) {
					best = compare;
					bestFloat = compFloat;
				}
			}
			break;
		
		case SORT_Y_ASCENDING:
		
			bestFloat = best->mY * floatSign;
		
			for ( u32 i = 1; i < this->mTotalProps; ++i ) {
				MOAIPartitionResult* compare = &this->mResults [ i ];
				
				float compFloat = compare->mY * floatSign;
				if ( compFloat > bestFloat ) {
					best = compare;
					bestFloat = compFloat;
				}
			}
			break;
		
		case SORT_Z_ASCENDING:
		
			bestFloat = best->mZ * floatSign;
		
			for ( u32 i = 1; i < this->mTotalProps; ++i ) {
				MOAIPartitionResult* compare = &this->mResults [ i ];

				float compFloat = compare->mZ * floatSign;
				if ( compFloat > bestFloat ) {
					best = compare;
					bestFloat = compFloat;
				}
			}
			break;

		case SORT_VECTOR_ASCENDING:
		
			bestFloat = (( best->mX * xScale ) + ( best->mY * yScale ) + ( best->mZ * zScale ) + (( float )best->mPriority * priority )) * floatSign;
		
			for ( u32 i = 1; i < this->mTotalProps; ++i ) {
				MOAIPartitionResult* compare = &this->mResults [ i ];
				
				float compFloat = (( compare->mX * xScale ) + ( compare->mY * yScale ) + ( compare->mZ * zScale ) + (( float )compare->mPriority * priority )) * floatSign;
				if ( compFloat > bestFloat ) {
					best = compare;
					bestFloat = compFloat;
				}
			}
			break;
		
		case SORT_NONE:
		default:
			break;
	}
	
	return best->mProp;
}

//----------------------------------------------------------------//
MOAIPartitionResultBuffer::MOAIPartitionResultBuffer () :
	mResults ( 0 ),
	mTotalResults ( 0 ),
	mTotalProps ( 0 ) {
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
u32 MOAIPartitionResultBuffer::PrepareResults ( u32 mode ) {

	return this->PrepareResults ( mode, false, 0.0f, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------//
u32 MOAIPartitionResultBuffer::PrepareResults ( u32 mode, bool expand, float xScale, float yScale, float zScale, float priority ) {
	UNUSED ( zScale );

	// make sure the main results buffer is at least as big as the props buffer
	if ( this->mMainBuffer.Size () < this->mProps.Size ()) {
		this->mMainBuffer.Init ( this->mProps.Size ());
	}

	if ( expand ) {
		for ( u32 i = 0; i < this->mTotalProps; ++i ) {
			this->mProps [ i ]->ExpandForSort ( *this );
		}
	}
	else {
		for ( u32 i = 0; i < this->mTotalProps; ++i ) {
			this->mProps [ i ]->MOAIProp::ExpandForSort ( *this );
		}
	}

	this->mResults = this->mMainBuffer;

	float floatSign = mode & SORT_FLAG_DESCENDING ? -1.0f : 1.0f;
	s32 intSign = ( int )floatSign;

	switch ( mode & SORT_MODE_MASK ) {
  
		case SORT_PRIORITY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				s32 p = this->mMainBuffer [ i ].mPriority * intSign;
				this->mMainBuffer [ i ].mKey = ( u32 )(( p ^ 0x80000000 ) | ( p & 0x7fffffff ));
			}
			break;

		case SORT_X_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float x = this->mMainBuffer [ i ].mX;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( x * floatSign );
			}
			break;
		
		case SORT_Y_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float y = this->mMainBuffer [ i ].mY;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( y * floatSign );
			}
			break;
		
		case SORT_Z_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float z = this->mMainBuffer [ i ].mZ;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( z * floatSign );
			}
			break;
		
		case SORT_VECTOR_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				MOAIPartitionResult& result = this->mMainBuffer [ i ];
				float axis = ( result.mX * xScale ) + ( result.mY * yScale ) + ( result.mZ * zScale ) + (( float )result.mPriority * priority );
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( axis * floatSign );
			}
			break;
		
		case SORT_NONE:
		default:
			return this->mTotalResults;
	}
	
	// affirm the swap buffer
	if ( this->mSwapBuffer.Size () < this->mMainBuffer.Size ()) {
		this->mSwapBuffer.Init ( this->mMainBuffer.Size ());
	}
	
	// sort
	this->mResults = RadixSort32 < MOAIPartitionResult >( this->mMainBuffer, this->mSwapBuffer, this->mTotalResults );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushProp ( MOAIProp& result ) {

	u32 idx = this->mTotalProps++;
	
	if ( idx >= this->mProps.Size ()) {
		this->mProps.Grow ( idx + 1, BLOCK_SIZE );
	}
	this->mProps [ idx ] = &result;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushResult ( MOAIProp& prop, int subPrimID, s32 priority, float x, float y, float z ) {

	u32 idx = this->mTotalResults++;
	
	if ( idx >= this->mMainBuffer.Size ()) {
		this->mMainBuffer.Grow ( idx + 1, BLOCK_SIZE );
	}
	
	MOAIPartitionResult& result = this->mMainBuffer [ idx ] ;
	
	result.mProp = &prop;
	result.mSubPrimID = subPrimID;
	result.mPriority = priority;
	
	result.mX = x;
	result.mY = y;
	result.mZ = z;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushResultProps ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 total = this->mTotalResults;
	
	for ( u32 i = 0; i < total; ++i ) {
		this->mResults [ i ].mProp->PushLuaUserdata ( state );
   }
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Reset () {

	this->mResults = 0;
	this->mTotalResults = 0;

	this->mTotalProps = 0;
}

