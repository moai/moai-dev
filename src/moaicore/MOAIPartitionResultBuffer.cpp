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

	float sign = mode & SORT_FLAG_DESCENDING ? -1.0f : 1.0f;
	s32 intSign = ( int )sign;

	switch ( mode & SORT_MODE_MASK ) {
  
		case SORT_PRIORITY_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				s32 priority = this->mMainBuffer [ i ].mPriority * intSign;
				this->mMainBuffer [ i ].mKey = ( u32 )(( priority ^ 0x80000000 ) | ( priority & 0x7fffffff ));
			}
			break;

		case SORT_X_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float x = this->mMainBuffer [ i ].mX;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( x * sign );
			}
			break;
		
		case SORT_Y_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				float y = this->mMainBuffer [ i ].mY;
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( y * sign );
			}
			break;
		
		case SORT_VECTOR_ASCENDING:
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				MOAIPartitionResult& result = this->mMainBuffer [ i ];
				float axis = ( result.mX * xScale ) + ( result.mY * yScale ) + (( float )result.mPriority * priority );
				this->mMainBuffer [ i ].mKey = USFloat::FloatToIntKey ( axis * sign );
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
	
	lua_createtable ( state, total, 0 );
	
	for ( u32 i = 1; i <= total; ++i ) {
		lua_pushnumber ( state, i );
		this->mResults [ i ].mProp->PushLuaUserdata ( state );
		lua_settable ( state, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Reset () {

	this->mResults = 0;
	this->mTotalResults = 0;

	this->mTotalProps = 0;
}

