// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USMemStream.h>

#include <math.h>

//================================================================//
// USMemStream
//================================================================//

//----------------------------------------------------------------//
void USMemStream::Clear () {

	if ( this->mChunks ) {
		for ( size_t i = 0; i < this->mTotalChunks; ++i ) {
			free ( this->mChunks [ i ]);
		}
		free ( this->mChunks );
		
		this->mLength = 0;
		this->mTotalChunks = 0;
		this->mChunks = 0;
	}
	
	this->mCursor = 0;
}

//----------------------------------------------------------------//
u32 USMemStream::GetCaps () {

	return CAN_READ | CAN_WRITE | CAN_SEEK;
}

//----------------------------------------------------------------//
size_t USMemStream::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t USMemStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
size_t USMemStream::ReadBytes ( void* buffer, size_t size ) {

	size_t cursor0 = this->mCursor;
	size_t cursor1 = cursor0 + size;

	if ( cursor1 > this->mLength ) {
		size = this->mLength - this->mCursor;
		cursor1 = this->mLength;
	}

	if ( size == 0 ) return 0;
	
	assert ( this->mChunks );

	size_t chunk0 = ( size_t )( cursor0 / this->mChunkSize );
	size_t chunk1 = ( size_t )( cursor1 / this->mChunkSize );

	size_t offset0 = cursor0 - ( chunk0 * this->mChunkSize );
	size_t offset1 = cursor1 - ( chunk1 * this->mChunkSize );

	void* src = ( void* )(( uintptr )this->mChunks [ chunk0 ] + offset0 );
	void* dest = buffer;

	if ( chunk0 == chunk1 ) {
		
		memcpy ( dest, src, offset1 - offset0 );
	}
	else {
		
		memcpy ( dest, src, this->mChunkSize - offset0 );
		dest = ( void* )(( uintptr )dest + this->mChunkSize - offset0 );
		
		for ( size_t i = ( chunk0 + 1 ); i < chunk1; ++i ) {
			memcpy ( dest, this->mChunks [ i ], this->mChunkSize );
			dest = ( void* )(( uintptr )dest + this->mChunkSize );
		}
		
		memcpy ( dest, this->mChunks [ chunk1 ], offset1 );
	}

	this->mCursor = cursor1;
	return size;
}

//----------------------------------------------------------------//
void USMemStream::Reserve ( size_t length ) {

	if ( length <= this->mLength ) return; 

	size_t totalChunks = ( length / this->mChunkSize ) + 1;
	if ( totalChunks <= this->mTotalChunks ) return;
		
	void** chunks = ( void** )malloc ( totalChunks * sizeof ( void* ));
	
	if ( this->mChunks ) {
		memcpy ( chunks, this->mChunks, this->mTotalChunks * sizeof ( void* ));
		free ( this->mChunks );
	}
	
	for ( size_t i = this->mTotalChunks; i < totalChunks; ++i ) {
		chunks [ i ] = malloc ( this->mChunkSize );
	}
	
	this->mTotalChunks = totalChunks;
	this->mChunks = chunks;
}

//----------------------------------------------------------------//
void USMemStream::SetChunkSize ( size_t chunkSize ) {

	assert ( chunkSize );
	this->Clear ();
	this->mChunkSize = chunkSize;
}

//----------------------------------------------------------------//
int USMemStream::SetCursor ( long offset ) {

	this->mCursor = offset;
	return 0;
}

//----------------------------------------------------------------//
STLString USMemStream::ToString ( size_t size ) {

	if ( size == 0 ) return 0;

	if (( this->mCursor + size ) > this->mLength ) {
		size = this->mLength - this->mCursor;
	}
	
	STLString str;
	char buffer [ DEFAULT_CHUNK_SIZE + 1 ];
	size_t readSize = DEFAULT_CHUNK_SIZE;
	
	while ( size > 0 ) {
		
		if ( size < readSize ) {
			readSize = size;
		}
		
		this->ReadBytes ( buffer, readSize );
		buffer [ readSize ] = 0;
		str.write ( "%s", buffer );
		size -= readSize;
	}
	
	return str;
}

//----------------------------------------------------------------//
USMemStream::USMemStream () :
	mChunkSize ( DEFAULT_CHUNK_SIZE ),
	mTotalChunks ( 0 ),
	mChunks ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ) {
}

//----------------------------------------------------------------//
USMemStream::~USMemStream () {
	this->Clear ();
}

//----------------------------------------------------------------//
size_t USMemStream::WriteBytes ( const void* buffer, size_t size ) {

	if ( !size ) return 0;

	size_t cursor0 = this->mCursor;
	size_t cursor1 = cursor0 + size;

	this->Reserve ( cursor1 );

	size_t chunk0 = ( size_t )( cursor0 / this->mChunkSize );
	size_t chunk1 = ( size_t )( cursor1 / this->mChunkSize );

	size_t offset0 = cursor0 - ( chunk0 * this->mChunkSize );
	size_t offset1 = cursor1 - ( chunk1 * this->mChunkSize );

	void* dest = ( void* )(( uintptr )this->mChunks [ chunk0 ] + offset0 );
	const void* src = buffer;

	if ( chunk0 == chunk1 ) {
	
		memcpy ( dest, src, offset1 - offset0 );
	}
	else {
		
		memcpy ( dest, src, this->mChunkSize - offset0 );
		src = ( void* )(( uintptr )src + this->mChunkSize - offset0 );
		
		for ( size_t i = ( chunk0 + 1 ); i < chunk1; ++i ) {
			memcpy ( this->mChunks [ i ], src, this->mChunkSize );
			src = ( void* )(( uintptr )src + this->mChunkSize );
		}
		
		memcpy ( this->mChunks [ chunk1 ], src, offset1 );
	}

	this->mCursor = cursor1;
	if ( this->mLength < this->mCursor ) {
		this->mLength = this->mCursor;
	}
	return size;
}
