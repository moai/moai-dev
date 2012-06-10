// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlcore/zl_util.h>
#include <zlcore/ZLZipStream.h>

using namespace std;

//================================================================//
// ZLZipStream
//================================================================//

//----------------------------------------------------------------//
void ZLZipStream::AffirmBlock () {

	int blockID = ( int )( this->mUncompressedCursor / ZIP_STREAM_BLOCK_SIZE );
	int sign = blockID & 1 ? 1 : 0;
	
	ZLCacheBlock* block = &this->mBlock [ sign ];
	
	// if the block is already cached, we're good
	if ( block->mBlockID == blockID ) return;
	
	// initialize the block
	block->mBlockID = blockID;
	block->mBase = blockID * ZIP_STREAM_BLOCK_SIZE;
	block->mSize = 0;
	
	if ( this->mCompression ) {
		
		// if the new block is behind the last block we loaded, reset
		if ( blockID < this->mPrevBlockID ) {
			this->ResetZipStream ();
		}
		
		// decompress blocks until we're caught up
		for ( ; this->mPrevBlockID < blockID; ++this->mPrevBlockID ) {
			block->mSize = this->Inflate ( block->mCache, ZIP_STREAM_BLOCK_SIZE, this->mFileBuffer, this->mFileBufferSize );
		}
	}
	else {
		fseek ( this->mFile, this->mBaseAddr + block->mBase, SEEK_SET );
		block->mSize = fread ( block->mCache, 1, ZIP_STREAM_BLOCK_SIZE, this->mFile );
	}
}

//----------------------------------------------------------------//
void ZLZipStream::Close () {

	if ( this->mFile ) {
		fclose ( this->mFile );
		this->mFile = 0;
	}
	
	if ( this->mCompression ) {
		inflateEnd ( &this->mStream );
		this->mCompression = 0;
		memset ( &this->mStream, 0, sizeof ( z_stream ));
	}
	
	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
	}
}

//----------------------------------------------------------------//
int ZLZipStream::FullyCache () {

	int result;

	this->mBufferSize = this->mEntry->mUncompressedSize;
	this->mBuffer = malloc ( this->mBufferSize );
	
	this->mFileBufferSize = this->mBufferSize;
	this->mFileBuffer = this->mBuffer;
	
	if ( this->mEntry->mCompression ) {
	
		char buffer [ ZIP_STREAM_FILEBUFFER_MAX ];
	
		result = inflateInit2 ( &this->mStream, -MAX_WBITS );
		if ( result != Z_OK ) return -1;
	
		this->Inflate ( this->mFileBuffer, this->mFileBufferSize, buffer, ZIP_STREAM_FILEBUFFER_MAX );
		inflateEnd ( &this->mStream );
	}
	else {
	
		fseek ( this->mFile, this->mBaseAddr, SEEK_SET );
		fread ( this->mFileBuffer, 1, this->mFileBufferSize, this->mFile );
	}
	
	fclose ( this->mFile );
	this->mFile = 0;
	this->mFullyCached = 1;
	this->mCompression = 0;
	
	return 0;
}

//----------------------------------------------------------------//
size_t ZLZipStream::Inflate ( void* dest, size_t size, void* buffer, size_t bufferSize ) {

	int result;
	FILE* file = this->mFile;
	z_stream* stream = &this->mStream;
    ZLZipFileEntry* entry = this->mEntry;
    size_t totalRead = 0;
    size_t totalOut = 0;

	if ( !file )	return 0;
	if ( !stream )	return 0;
	if ( !size )	return 0;
    
	stream->next_out = ( Bytef* )dest;
	stream->avail_out = size;

    while ( totalRead < size ) {
		
		if ( stream->avail_in == 0 ) {
			
			size_t cacheSize = entry->mCompressedSize - this->mCompressedCursor;
			
			if ( cacheSize > 0 ) {
				if ( cacheSize > bufferSize ) {
					cacheSize = bufferSize;
				}

				cacheSize = fread ( buffer, 1, cacheSize, file );
				if ( cacheSize <= 0 ) break;

				this->mCompressedCursor += cacheSize;
				stream->next_in = ( Bytef* )buffer;
				stream->avail_in = cacheSize;
			}
		}

		totalOut = stream->total_out;
		result = inflate ( stream, Z_SYNC_FLUSH );
		totalRead += ( stream->total_out - totalOut );
		
		if ( result != Z_OK ) break;
	}
	
    return totalRead;
}

//----------------------------------------------------------------//
int ZLZipStream::InitBuffers () {

	int result;
	ZLZipFileEntry* entry = this->mEntry;

	// compute the file buffer size
	this->mFileBufferSize = 0;
	
	this->mCompression = entry->mCompression;
	if ( this->mCompression ) {
		result = inflateInit2 ( &this->mStream, -MAX_WBITS );
		if ( result != Z_OK ) return -1;
		this->mFileBufferSize = entry->mCompressedSize < ZIP_STREAM_FILEBUFFER_MAX ? entry->mCompressedSize : ZIP_STREAM_FILEBUFFER_MAX;
	}
	
	// allocate the master buffer
	this->mBufferSize = this->mFileBufferSize + ZIP_STREAM_CACHE_SIZE;
	this->mBuffer = malloc ( this->mBufferSize );
	
	// assign all the pointers to their positions in the master buffer
	this->mFileBuffer = this->mBuffer;
	this->mCache = ( void* )(( size_t )this->mFileBuffer + this->mFileBufferSize );
	
	this->mBlock [ 0 ].mCache = this->mCache;
	this->mBlock [ 1 ].mCache = ( void* )(( size_t )this->mCache + ZIP_STREAM_BLOCK_SIZE );
	
	// initialize the block IDs
	this->mBlock [ 0 ].mBlockID = -1;
	this->mBlock [ 1 ].mBlockID = -1;
	
	this->mPrevBlockID = -1;

	return 0;
}

//----------------------------------------------------------------//
int ZLZipStream::IsAtEnd () {

	return this->mUncompressedCursor >= this->mEntry->mUncompressedSize ? 1 : 0;	
}

//----------------------------------------------------------------//
ZLZipStream* ZLZipStream::Open ( ZLZipArchive* archive, const char* entryname ) {

	int result;
	FILE* file = 0;
	ZLZipFileEntry* entry;
	ZLZipFileHeader fileHeader;
	ZLZipStream* self = 0;
	
	entry = archive->FindEntry ( entryname );
	if ( !entry ) goto error;

	file = fopen ( archive->mFilename.c_str (), "rb" );
	if ( !file ) goto error;

	self = new ZLZipStream ();

	self->mFile = file;
	self->mEntry = entry;
	// finfo->entry = (( entry->symlink != NULL ) ? entry->symlink : entry );

	// seek to the base of the zip file header
	result = fseek ( file, entry->mFileHeaderAddr, SEEK_SET );
	if ( result ) goto error;

	// read local header
	result = fileHeader.Read ( file );
	if ( result ) goto error;

	// skip the extra field, etc.
	result = fseek ( file, fileHeader.mNameLength + fileHeader.mExtraFieldLength, SEEK_CUR );
	if ( result ) goto error;

	// this is the base address of the compressed file data
	self->mBaseAddr = ftell ( file );
    
    // looks like all systems are go, so time to set up the buffers (or not)
    result = ( entry->mUncompressedSize <= ZIP_STREAM_BUFFER_MAX ) ? self->FullyCache () : self->InitBuffers ();
	if ( result ) goto error;
    
	return self;

error:

	if ( self ) {
		delete self;
	}
	return 0;
}

//----------------------------------------------------------------//
size_t ZLZipStream::Read ( void* buffer, size_t size ) {

	ZLCacheBlock* block;
	size_t remaining = size;
	size_t available;
	size_t read;

	while ( this->mUngetStackTop && remaining ) {
		
		*( char* )buffer = this->mUngetStack [ --this->mUngetStackTop ];
		buffer = ( void* )(( size_t )buffer + 1 );
		this->mUncompressedCursor++;
		remaining--;
	}

	if ( !remaining ) return size;

	if ( this->mFullyCached ) {
		
		void* src = ( void* )(( size_t )this->mFileBuffer + this->mUncompressedCursor );
		
		available = this->mEntry->mUncompressedSize - this->mUncompressedCursor;
		read = available < remaining ? available : remaining;
		
		memcpy ( buffer, src, read );
		
		this->mUncompressedCursor += read;
		remaining -= read;
		return size - remaining;
	}
	
	while ( remaining ) {
		
		size_t cursor;
		void* src;
		size_t blockID = ( size_t )( this->mUncompressedCursor / ZIP_STREAM_BLOCK_SIZE );
		int sign = blockID & 1 ? 1 : 0;
		
		this->AffirmBlock ();
		
		block = &this->mBlock [ sign ];
		cursor = this->mUncompressedCursor - block->mBase;
		available = block->mSize - cursor;
		read = available < remaining ? available : remaining;
		
		if ( !read ) break;
		
		src = ( void* )(( size_t )block->mCache + cursor );
		memcpy ( buffer, src, read );
		
		this->mUncompressedCursor += read;
		remaining -= read;
		buffer = ( void* )(( size_t )buffer + read );
	}
	
    return size - remaining;
}

//----------------------------------------------------------------//
int ZLZipStream::ResetZipStream () {

	int result;
	FILE* file = this->mFile;
	z_stream* stream = &this->mStream;

	z_stream newStream;
	memset ( &newStream, 0, sizeof ( z_stream ));
	
	result = fseek ( file, this->mBaseAddr, SEEK_SET );
	if ( result ) return -1;
	
	result = inflateInit2 ( &newStream, -MAX_WBITS );
	if ( result != Z_OK ) return -1;

	inflateEnd ( stream );
	( *stream ) = newStream;

	this->mCompressedCursor = 0;
	this->mPrevBlockID = -1;
	
	return 0;
}

//----------------------------------------------------------------//
int ZLZipStream::Seek ( long int offset, int origin ) {

	size_t absOffset = 0;
	
	switch ( origin ) {
		case SEEK_CUR: {
			absOffset = this->mUncompressedCursor + offset;
			break;
		}
		case SEEK_END: {
			absOffset = this->mEntry->mUncompressedSize + offset;
			break;
		}
		case SEEK_SET: {
			absOffset = offset;
			break;
		}
	}

	if ( absOffset > this->mEntry->mUncompressedSize ) return -1;
	
	this->mUncompressedCursor = absOffset;
	this->mUngetStackTop = 0;
    return 0;
}

//----------------------------------------------------------------//
size_t ZLZipStream::Tell () {

	return this->mUncompressedCursor;
}

//----------------------------------------------------------------//
int ZLZipStream::UnGetChar ( char c ) {

	if ( this->mUncompressedCursor && ( this->mUngetStackTop < ZIP_STREAM_UNGET_STACK_SIZE )) {
	
		this->mUncompressedCursor--;
		this->mUngetStack [ this->mUngetStackTop++ ] = c;
		
		return 0;
	}
	return -1;
}

//----------------------------------------------------------------//
ZLZipStream::ZLZipStream () :
	mFile ( 0 ),
	mEntry ( 0 ),
	mBaseAddr ( 0 ),
	mCompressedCursor ( 0 ),
	mUncompressedCursor ( 0 ),
	mCompression ( 0 ),
	mFullyCached ( 0 ),
	mBuffer ( 0 ),
	mBufferSize ( 0 ),
	mFileBuffer ( 0 ),
	mFileBufferSize ( 0 ),
	mCache ( 0 ),
	mPrevBlockID ( 0 ),
	mUngetStackTop ( 0 ) {
	
	memset ( &this->mStream, 0, sizeof ( z_stream ));
}

//----------------------------------------------------------------//
ZLZipStream::~ZLZipStream () {

	this->Close ();
}