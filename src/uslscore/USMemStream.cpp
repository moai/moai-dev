// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFilename.h>
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
		for ( u32 i = 0; i < this->mTotalChunks; ++i ) {
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
u32 USMemStream::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
u32 USMemStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
u32 USMemStream::ReadBytes ( void* buffer, u32 size ) {

	if ( size == 0 ) return 0;

	u32 cursor0 = this->mCursor;
	u32 cursor1 = cursor0 + size;

	if ( cursor1 > this->mLength ) {
		size = this->mLength - this->mCursor;
		cursor1 = this->mLength;
	}

	u32 chunk0 = ( u32 )( cursor0 / this->mChunkSize );
	u32 chunk1 = ( u32 )( cursor1 / this->mChunkSize );

	u32 offset0 = cursor0 - ( chunk0 * this->mChunkSize );
	u32 offset1 = cursor1 - ( chunk1 * this->mChunkSize );

	void* src = ( void* )(( uintptr )this->mChunks [ chunk0 ] + offset0 );
	void* dest = buffer;

	if ( chunk0 == chunk1 ) {
		
		memcpy ( dest, src, offset1 - offset0 );
	}
	else {
		
		memcpy ( dest, src, this->mChunkSize - offset0 );
		dest = ( void* )(( uintptr )dest + this->mChunkSize - offset0 );
		
		for ( u32 i = ( chunk0 + 1 ); i < chunk1; ++i ) {
			memcpy ( dest, this->mChunks [ i ], this->mChunkSize );
			dest = ( void* )(( uintptr )dest + this->mChunkSize );
		}
		
		memcpy ( dest, this->mChunks [ chunk1 ], offset1 );
	}

	this->mCursor = cursor1;
	return size;
}

//----------------------------------------------------------------//
void USMemStream::Reserve ( u32 length ) {

	if ( length <= this->mLength ) return; 

	u32 totalChunks = ( u32 )ceil (( float )length / ( float )this->mChunkSize );
	if ( totalChunks <= this->mTotalChunks ) return;
		
	void** chunks = ( void** )malloc ( totalChunks * sizeof ( void* ));
	
	if ( this->mChunks ) {
		memcpy ( chunks, this->mChunks, this->mTotalChunks * sizeof ( void* ));
		free ( this->mChunks );
	}
	
	for ( u32 i = this->mTotalChunks; i < totalChunks; ++i ) {
		chunks [ i ] = malloc ( this->mChunkSize );
	}
	
	this->mTotalChunks = totalChunks;
	this->mChunks = chunks;
}

//----------------------------------------------------------------//
void USMemStream::Seek ( long offset, int origin ) {

	switch ( origin ) {
		case SEEK_CUR: {
			this->mCursor = this->mCursor + offset;
			break;
		}
		case SEEK_END: {
			this->mCursor = this->mLength;
			break;
		}
		case SEEK_SET: {
			this->mCursor = offset;
			break;
		}
	}
	
	if ( this->mCursor > this->mLength ) {
		this->mCursor = this->mLength;
	}
}

//----------------------------------------------------------------//
void USMemStream::SetChunkSize ( u32 chunkSize ) {

	assert ( chunkSize );
	this->Clear ();
	this->mChunkSize = chunkSize;
}

//----------------------------------------------------------------//
STLString USMemStream::ToString ( u32 size ) {

	if ( size == 0 ) return 0;

	if (( this->mCursor + size ) > this->mLength ) {
		size = this->mLength - this->mCursor;
	}
	
	STLString str;
	char buffer [ DEFAULT_CHUNK_SIZE + 1 ];
	u32 readSize = DEFAULT_CHUNK_SIZE;
	
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
u32 USMemStream::WriteBytes ( const void* buffer, u32 size ) {

	u32 cursor0 = this->mCursor;
	u32 cursor1 = cursor0 + size;

	this->Reserve ( cursor1 );

	u32 chunk0 = ( u32 )( cursor0 / this->mChunkSize );
	u32 chunk1 = ( u32 )( cursor1 / this->mChunkSize );

	u32 offset0 = cursor0 - ( chunk0 * this->mChunkSize );
	u32 offset1 = cursor1 - ( chunk1 * this->mChunkSize );

	void* dest = ( void* )(( uintptr )this->mChunks [ chunk0 ] + offset0 );
	const void* src = buffer;

	if ( chunk0 == chunk1 ) {
	
		memcpy ( dest, src, offset1 - offset0 );
	}
	else {
		
		memcpy ( dest, src, this->mChunkSize - offset0 );
		src = ( void* )(( uintptr )src + this->mChunkSize - offset0 );
		
		for ( u32 i = ( chunk0 + 1 ); i < chunk1; ++i ) {
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
