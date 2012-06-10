// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLZIPSTREAM_H
#define ZLZIPSTREAM_H

#include <zlib.h>
#include <zlcore/ZLZipArchive.h>

#define ZIP_STREAM_FILEBUFFER_MAX 4096
#define ZIP_STREAM_CACHE_SIZE 4096
#define ZIP_STREAM_BLOCK_SIZE ( ZIP_STREAM_CACHE_SIZE >> 1 )
#define ZIP_STREAM_BUFFER_MAX ( ZIP_STREAM_FILEBUFFER_MAX + ZIP_STREAM_CACHE_SIZE )
#define ZIP_STREAM_UNGET_STACK_SIZE 32

//================================================================//
// ZLCacheBlock
//================================================================//
class ZLCacheBlock {
public:

	int					mBlockID;		// ID of currently cached blocks
	size_t				mBase;
	size_t				mSize;
	void*				mCache;			// pointer into cache
};

//================================================================//
// ZLZipStream
//================================================================//
class ZLZipStream {
private:

	FILE*				mFile;					// archive file
	ZLZipFileEntry*		mEntry;					// address of the zip entry in the archive
	size_t				mBaseAddr;				// address of the zip data in the archive
	
	size_t				mCompressedCursor;		// cursor in the main stream
	size_t				mUncompressedCursor;	// cursor in the zip stream
	z_stream			mStream;				// underlying zip stream state
	
	int					mCompression;			// compression mode
	int					mFullyCached;			// nonzero if entire file has been uncompressed
	
	void*				mBuffer;
	size_t				mBufferSize;
	
	void*				mFileBuffer;			// buffer of compressed file data
	size_t				mFileBufferSize;		// size of buffer
	
	// we use a pair of cache blocks to avoid 'thrashing' when seeking and reading back and
	// forth across a block boundary. in other words the user may seek back and forth within
	// the scope of a single cache block and only pay (at most) one reload of one block.
	
	void*				mCache;					// decompressed data cache
	ZLCacheBlock		mBlock [ 2 ];			// structure to hold block info

	int					mPrevBlockID;			// ID of the previous block decoded
	
	char				mUngetStack [ ZIP_STREAM_UNGET_STACK_SIZE ];
	int					mUngetStackTop;

	//----------------------------------------------------------------//
	void					AffirmBlock			();
	int						FullyCache			();
	size_t					Inflate				( void* dest, size_t size, void* buffer, size_t bufferSize );
	int						InitBuffers			();
	int						ResetZipStream		();

public:

	//----------------------------------------------------------------//
	void					Close				();
	int						IsAtEnd				();
	static ZLZipStream*		Open				( ZLZipArchive* archive, const char* entryname );
	size_t					Read				( void* buffer, size_t size );
	int						Seek				( long int offset, int origin );
	size_t					Tell				();
	int						UnGetChar			( char c );
							ZLZipStream			();
							~ZLZipStream		();
};

#endif