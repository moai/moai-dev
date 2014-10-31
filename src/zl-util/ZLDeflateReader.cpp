// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLDeflateReader.h>
#include <zl-util/ZLDeflateWriter.h>
#include <zl-util/ZLZip.h>

//================================================================//
// ZLDeflateReader
//================================================================//

//----------------------------------------------------------------//
int ZLDeflateReader::AffirmChunk ( int chunkID ) {

	int sign = chunkID & 1 ? 1 : 0;
	ZLStreamChunk& chunk = this->mChunk [ sign ];
	
	// if the chunk is already cached, we're good
	if ( chunkID == chunk.mChunkID ) return 0;
	
	int prevChunkID = chunk.mChunkID;
	
	// initialize the chunk
	chunk.mChunkID = chunkID;
	chunk.mBase = chunkID * ZL_DEFLATE_READER_CHUNK_SIZE;
	chunk.mSize = 0;

	if ( chunkID < prevChunkID ) {
		this->ResetZipStream ();
		
		for ( int i = 0; i <= chunkID; ++i ) {
			this->InflateChunk ( chunk );
		}
	}
	else {
		
		ZLStreamChunk& otherChunk = this->mChunk [ ~sign & 1 ];
		prevChunkID = chunkID - 1;
		
		if ( otherChunk.mChunkID < prevChunkID ) {
		
			this->ResetZipStream ();
			
			otherChunk.mChunkID = prevChunkID;
			otherChunk.mBase = prevChunkID * ZL_DEFLATE_READER_CHUNK_SIZE;
			otherChunk.mSize = 0;
			
			for ( int i = 0; i <= prevChunkID; ++i ) {
				this->InflateChunk ( otherChunk );
			}
		}
		this->InflateChunk ( chunk );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 ZLDeflateReader::GetCaps () {

	return this->mProxiedStream ? CAN_READ | CAN_SEEK : 0;
}

//----------------------------------------------------------------//
size_t ZLDeflateReader::Inflate ( void* dest, size_t size ) {
    
    z_stream* stream = &this->mZStream;
	stream->next_out = ( Bytef* )dest;
	stream->avail_out = size;

	size_t totalRead = 0;
    while ( totalRead < size ) {
		
		if ( stream->avail_in == 0 ) {
			
			size_t available = this->mProxiedStream->ReadBytes ( this->mInputChunk, ZL_DEFLATE_READER_CHUNK_SIZE );
			if ( available <= 0 ) break;
			
			stream->next_in = ( Bytef* )this->mInputChunk;
			stream->avail_in = available;
		}

		size_t totalOut = stream->total_out;
		int result = inflate ( stream, Z_SYNC_FLUSH );
		totalRead += ( stream->total_out - totalOut );
		
		if ( result != Z_OK ) {
			if ( result != Z_STREAM_END ) {
				printf ( "%s\n", ZLZip::GetErrMsg ( result ));
			}
			break;
		}
	}
    return totalRead;
}

//----------------------------------------------------------------//
void ZLDeflateReader::InflateChunk ( ZLStreamChunk& chunk ) {

	chunk.mSize = this->Inflate ( chunk.mCache, ZL_DEFLATE_READER_CHUNK_SIZE );
	size_t length = chunk.mBase + chunk.mSize;
	if ( this->mLength <= length ) {
		this->mLength = length;
	}
}

//----------------------------------------------------------------//
void ZLDeflateReader::OnClose () {
	
	if ( this->mProxiedStream ) {
		inflateEnd ( &this->mZStream );
		memset ( &this->mZStream, 0, sizeof ( z_stream ));
	}
	
	if ( this->mInputChunk ) {
		free ( this->mInputChunk );
	}
	
	if ( this->mCache ) {
		free ( this->mCache );
	}
	
	this->mCache = 0;
	this->mCacheSize = 0;

	this->mInputChunk = 0;
}

//----------------------------------------------------------------//
bool ZLDeflateReader::OnOpen () {

	memset ( &this->mZStream, 0, sizeof ( z_stream ));
	int result = inflateInit2 ( &this->mZStream, mWindowBits );
	if ( result != Z_OK ) return false;

	this->mInputChunk = malloc ( ZL_DEFLATE_READER_CHUNK_SIZE );

	this->mCacheSize = ZL_DEFLATE_READER_CHUNK_SIZE * 2;
	this->mCache = malloc ( this->mCacheSize );
	
	this->mChunk [ 0 ].mCache = this->mCache;
	this->mChunk [ 0 ].mChunkID = -1;
	
	this->mChunk [ 1 ].mCache = ( void* )(( size_t )this->mCache + ZL_DEFLATE_READER_CHUNK_SIZE );
	this->mChunk [ 1 ].mChunkID = -1;
	
	return true;
}

//----------------------------------------------------------------//
size_t ZLDeflateReader::ReadBytes ( void* buffer, size_t size ) {

	ZLStreamChunk* chunk;
	size_t remaining = size;

	if ( !remaining ) return size;
	
	while ( remaining ) {
		
		size_t cursor;
		void* src;
		size_t chunkID = ( size_t )( this->mCursor / ZL_DEFLATE_READER_CHUNK_SIZE );
		int sign = chunkID & 1 ? 1 : 0;
		
		this->AffirmChunk ( chunkID );
		
		if ( this->mLength && ( this->mCursor >= this->mLength )) {
			this->mCursor = this->mLength;
			break;
		}
		
		chunk = &this->mChunk [ sign ];
		cursor = this->mCursor - chunk->mBase;
		size_t available = chunk->mSize - cursor;
		size_t read = available < remaining ? available : remaining;
		
		if ( !read ) break;
		
		src = ( void* )(( size_t )chunk->mCache + cursor );
		memcpy ( buffer, src, read );
		
		this->mCursor += read;
		remaining -= read;
		buffer = ( void* )(( size_t )buffer + read );
	}
    return size - remaining;
}

//----------------------------------------------------------------//
int ZLDeflateReader::ResetZipStream () {

	z_stream newStream;
	memset ( &newStream, 0, sizeof ( z_stream ));
	
	int result = inflateInit2 ( &newStream, this->mWindowBits );
	if ( result != Z_OK ) return -1;

	inflateEnd ( &this->mZStream );
	this->mZStream = newStream;

	this->mProxiedStream->Seek ( this->mBase, SEEK_SET );
	
	return 0;
}

//----------------------------------------------------------------//
int ZLDeflateReader::SetCursor ( long offset ) {

	this->mCursor = offset;
	return 0;
}

//----------------------------------------------------------------//
ZLDeflateReader::ZLDeflateReader () :
	mWindowBits ( ZLDeflateWriter::DEFAULT_WBITS ),
	mInputChunk ( 0 ),
	mCache ( 0 ),
	mCacheSize ( 0 ) {
	
	memset ( &this->mZStream, 0, sizeof ( z_stream ));
}

//----------------------------------------------------------------//
ZLDeflateReader::~ZLDeflateReader () {

	this->Close ();
}