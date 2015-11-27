// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLAllocator.h>

//================================================================//
// ZLAllocator
//================================================================//

//----------------------------------------------------------------//
size_t ZLAllocator::Pad ( size_t size, u32 paddingScheme, size_t paddedChunkSize ) {

	switch ( paddingScheme ) {
	
		case NO_PADDING:
			break;
		
		case PAD_TO_POW2: {
			
			size_t pow2 = 1;
			while ( pow2 < size ) pow2 = pow2 << 0x01;
			size = pow2;
			
			break;
		}
		case PAD_TO_CHUNK:
			
			assert ( paddedChunkSize > 0 );
			size = (( size / paddedChunkSize ) + 1 ) * paddedChunkSize;
			break;
	}
	return size;
}

//================================================================//
// ZLTemporalAllocator
//================================================================//

//---------------------------------------------------------------
void ZLTemporalAllocator::Age () {

	EntrySetsIteratorIt entrySetIt = this->mEntrySetsBySize.begin ();
	while ( entrySetIt != this->mEntrySetsBySize.end ()) {
	
		size_t size = entrySetIt->first;
		EntrySet& entrySet = entrySetIt->second;
		++entrySetIt;
		
		entrySet.mAge++;
		
		if ( entrySet.mAge > this->mMaximumAge ) {
		
			STLSet < void* >::iterator bufferIt = entrySet.mBuffers.begin ();
			for ( ; bufferIt != entrySet.mBuffers.end (); ++bufferIt ) {
			
				void* buffer = *bufferIt;
				this->mSizesByAddress.erase ( buffer );
				free ( buffer );
			}
			this->mEntrySetsBySize.erase ( size );
		}
	}
}

//----------------------------------------------------------------//
void* ZLTemporalAllocator::Alloc ( size_t size ) {

	if ( !size ) return 0;

	void* buffer = 0;

	size = ZLAllocator::Pad ( size, this->mPaddingScheme, this->mPaddedChunkSize );
	
	if ( this->mEntrySetsBySize.contains ( size )) {
	
		EntrySet& entrySet = this->mEntrySetsBySize [ size ];
		
		if ( entrySet.mBuffers.size ()) {
		
			STLSet < void* >::iterator bufferIt = entrySet.mBuffers.begin ();
		
			buffer = *bufferIt;
			entrySet.mBuffers.erase ( bufferIt );
		}
	}
	
	if ( buffer ) {
	
		buffer = malloc ( size );
		this->mSizesByAddress [ buffer ] = size;
	}
	
	return buffer;
}

//----------------------------------------------------------------//
void ZLTemporalAllocator::Free ( void* mem ) {

	assert ( this->mSizesByAddress.contains ( mem ));

	size_t size = this->mSizesByAddress [ mem ];
	
	EntrySet& entrySet = this->mEntrySetsBySize [ size ];
	entrySet.mBuffers.insert ( mem );
	entrySet.mAge = 0;
}

//----------------------------------------------------------------//
ZLTemporalAllocator::ZLTemporalAllocator () :
	mPaddingScheme ( PAD_TO_POW2 ),
	mPaddedChunkSize ( 0 ),
	mMaximumAge ( -1 ) {
}

//----------------------------------------------------------------//
ZLTemporalAllocator::~ZLTemporalAllocator () {
}

//================================================================//
// ZLLinearAllocator
//================================================================//

//----------------------------------------------------------------//
void* ZLLinearAllocator::Alloc ( size_t size ) {

	void* mem = 0;

	if (( this->mBufferTop + size ) < this->mBufferSize ) {
	
		mem = ( void* )(( size_t )this->mBuffer + this->mBufferTop );
		this->mBufferTop += size;
		this->mBufferAllocated += size;
	}
	else {
		mem = malloc ( size );
	}
	
	if ( mem ) {
		this->mSizesByAddress [ mem ] = size;
		this->mAllocated += size;
		
		if ( this->mAllocated > this->mMaxAllocated ) {
			this->mMaxAllocated = this->mAllocated;
		}
	}
	return mem;
}

//----------------------------------------------------------------//
void ZLLinearAllocator::Free ( void* mem ) {

	size_t base = ( size_t )this->mBuffer;
	size_t top = base + this->mBufferTop;

	assert ( this->mSizesByAddress.contains ( mem ));
	size_t size = this->mSizesByAddress [ mem ];

	if ((( size_t )mem >= base ) && (( size_t )mem < top )) {
	
		this->mBufferAllocated -= size;
		
		if ( this->mBufferAllocated == 0 ) {
			this->mBufferTop = 0;
		}
	}
	else {
		free ( mem );
	}
	
	this->mAllocated -= size;
	this->mSizesByAddress.erase ( mem );
}

//----------------------------------------------------------------//
void ZLLinearAllocator::Update ( u32 paddingScheme, u32 paddedChunkSize ) {

	// we can only change the buffer size when the buffer is empty
	if ( this->mBufferTop == 0 ) {
	
		// if the buffer top is better, there had better damn well be no allocations
		assert ( this->mBufferAllocated == 0 );
	
		// suggested size for the buffer
		size_t bufferSize = ZLAllocator::Pad ( this->mMaxAllocated, paddingScheme, paddedChunkSize );
		
		// grow buffer if the suggested is greater than the current size OR shrink the buffer if it's been at least mResizeDelay updates since the last resize
		bool resize = (( this->mBufferSize < bufferSize ) || (( bufferSize < this->mBufferSize  ) && ( this->mUpdateCounter >= this->mResizeDelay )));
	
		if ( resize ) {
	
			if ( this->mBuffer ) {
				free ( this->mBuffer );
				this->mBuffer = 0;
			}
		
			this->mBufferSize = bufferSize;
			
			if ( bufferSize ) {
				this->mBuffer = malloc ( bufferSize );
			}
			
			// reset the update counter and max allocated
			this->mUpdateCounter = -1;
			this->mMaxAllocated = this->mAllocated;
		}
	}
	
	if ( this->mUpdateCounter <= this->mResizeDelay ) {
		this->mUpdateCounter++;
	}
}

//----------------------------------------------------------------//
ZLLinearAllocator::ZLLinearAllocator () :
	mAllocated ( 0 ),
	mMaxAllocated ( 0 ),
	mBufferSize ( 0 ),
	mBufferAllocated ( 0 ),
	mBufferTop ( 0 ),
	mBuffer ( 0 ),
	mUpdateCounter ( 0 ),
	mResizeDelay ( DEFAULT_RESIZE_DELAY ) {
}

//----------------------------------------------------------------//
ZLLinearAllocator::~ZLLinearAllocator () {
}
