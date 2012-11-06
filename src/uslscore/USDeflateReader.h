// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDEFLATEREADER_H
#define USDEFLATEREADER_H

#include <uslscore/USAccessors.h>
#include <uslscore/USStreamReader.h>
#include <zlib.h>

#define US_DEFLATE_READER_MAX_CACHE		4096
#define US_DEFLATE_READER_CHUNK_SIZE	2048

//================================================================//
// USStreamChunk
//================================================================//
class USStreamChunk {
public:

	int					mChunkID;		// ID of currently cached chunk
	size_t				mBase;
	size_t				mSize;
	void*				mCache;			// pointer into cache
};

//================================================================//
// USDeflateReader
//================================================================//
class USDeflateReader :
	public USStreamReader {
private:

	USStream*			mInputStream;			// compressed input stream
	size_t				mInputBase;				// base of compressed stream (when opened)
	
	size_t				mUncompressedCursor;	// cursor in the output stream
	size_t				mUncompressedSize;		// uncompressed size (if known)
	size_t				mLength;
	
	z_stream			mZStream;				// underlying zip stream state
	int					mWindowBits;
	
	void*				mInputChunk;			// compressed chunk from file
	
	// we use a pair of cache blocks to avoid 'thrashing' when seeking and reading back and
	// forth across a block boundary. in other words the user may seek back and forth within
	// the scope of a single cache block and only pay (at most) one reload of one block.
	
	void*				mCache;					// decompressed data cache
	size_t				mCacheSize;				// total size of cache
	USStreamChunk		mChunk [ 2 ];			// structure to hold chunk info

	//----------------------------------------------------------------//
	int					AffirmChunk				( int chunkID );
	size_t				Inflate					( void* dest, size_t size );
	void				InflateChunk			( USStreamChunk& chunk );
	int					ResetZipStream			();

public:

	static const int DEFAULT_WBITS = -15;

	GET_SET ( int, WindowBits, mWindowBits )

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				IsAtEnd					();
	bool				Open					( USStream* stream );
	size_t				ReadBytes				( void* buffer, size_t size );
	int					SetCursor				( long offset );
						USDeflateReader			();
						~USDeflateReader		();
};

#endif