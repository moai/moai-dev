// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartitionResultMgr.h>

//================================================================//
// MOAIScopedPartitionResultBufferHandle
//================================================================//

//----------------------------------------------------------------//
MOAIPartitionResultBuffer& MOAIScopedPartitionResultBufferHandle::Buffer () {

	return *this->mBuffer;
}

//----------------------------------------------------------------//
MOAIScopedPartitionResultBufferHandle::MOAIScopedPartitionResultBufferHandle ( MOAIPartitionResultMgr& mgr ) :
	mMgr ( mgr ) {
}

//----------------------------------------------------------------//
MOAIScopedPartitionResultBufferHandle::~MOAIScopedPartitionResultBufferHandle () {

	if ( this->mBuffer ) {
		this->mMgr.ReleaseBuffer ( this->mBuffer );
	}
}

//================================================================//
// MOAIPartitionResultMgr
//================================================================//

//----------------------------------------------------------------//
MOAIScopedPartitionResultBufferHandle MOAIPartitionResultMgr::GetBufferHandle () {

	MOAIScopedPartitionResultBufferHandle handle ( *this );

	if ( this->mAvailable.GetTop () == 0 ) {

		handle.mBuffer = new MOAIPartitionResultBuffer ();

		size_t index = this->mBuffers.Size ();
		this->mBuffers.Grow ( index + 1, 1, 0 );
		this->mBuffers [ index ] = handle.mBuffer;
		
		return handle;
	}
	
	handle.mBuffer = this->mAvailable.Pop ();
	assert ( handle.mBuffer );
	return handle;
}

//----------------------------------------------------------------//
MOAIPartitionResultMgr::MOAIPartitionResultMgr () {
}

//----------------------------------------------------------------//
MOAIPartitionResultMgr::~MOAIPartitionResultMgr () {

	for ( size_t i = 0; i < this->mBuffers.Size (); ++i ) {
		delete this->mBuffers [ i ];
	}
}

//----------------------------------------------------------------//
void MOAIPartitionResultMgr::ReleaseBuffer ( MOAIPartitionResultBuffer* buffer ) {

	if ( buffer ) {
		this->mAvailable.Push ( buffer );
	}
}
