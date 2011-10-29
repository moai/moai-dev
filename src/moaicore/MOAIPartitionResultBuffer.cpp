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
	
	this->mResults = 0;
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::FinishQuery () {

	this->mResults = this->mMainBuffer;
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
MOAIProp* MOAIPartitionResultBuffer::PopResult () {

	if ( this->mTotalResults ) {
		return this->mResults [ --this->mTotalResults ].mData;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushResult ( MOAIProp& result ) {

	u32 idx = this->mTotalResults++;
	
	if ( idx >= this->mMainBuffer.Size ()) {
		this->mMainBuffer.Grow ( idx + 1, BLOCK_SIZE );
	}

	this->mMainBuffer [ idx ].mData = &result;
	this->mMainBuffer [ idx ].mKey = 0;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::PushResultsList ( lua_State* L ) {
	USLuaState state ( L );

	u32 total = this->mTotalResults;
	
	lua_createtable ( state, total, 0 );
	
	for ( u32 i = 1; i <= total; ++i ) {
		lua_pushnumber ( state, i );
		this->mResults [ i ].mData->PushLuaUserdata ( state );
		lua_settable ( state, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Reset () {

	this->mResults = 0;
	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
void MOAIPartitionResultBuffer::Sort ( u32 mode ) {

	switch ( mode ) {
		
		case SORT_PRIORITY:
		
			for ( u32 i = 0; i < this->mTotalResults; ++i ) {
				s32 priority = this->mMainBuffer [ i ].mData->GetPriority ();
				this->mMainBuffer [ i ].mKey = ( u32 )(( priority ^ 0x80000000 ) | ( priority & 0x7fffffff ));
			}
			break;
		
		// TODO
		case SORT_X_AXIS:
		case SORT_Y_AXIS:
		case SORT_NONE:
		default:
			return;
	}
	
	// affirm the swap buffer
	if ( this->mSwapBuffer.Size () < this->mMainBuffer.Size ()) {
		this->mSwapBuffer.Init ( this->mMainBuffer.Size ());
	}
	
	// sort
	this->mResults = RadixSort32 < QueryEntry >( this->mMainBuffer, this->mSwapBuffer, this->mTotalResults );
}
