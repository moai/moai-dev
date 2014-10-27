// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLDEFLATEREADER_H
#define ZLDEFLATEREADER_H

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLStreamAdapter.h>
#include <zlib.h>

#define ZL_DEFLATE_READER_MAX_CACHE		4096
#define ZL_DEFLATE_READER_CHUNK_SIZE	2048

//================================================================//
// ZLStreamChunk
//================================================================//
class ZLStreamChunk {
public:

	int					mChunkID;		// ID of currently cached chunk
	size_t				mBase;
	size_t				mSize;
	void*				mCache;			// pointer into cache
};

//================================================================//
// ZLDeflateReader
//================================================================//
class ZLDeflateReader :
	public ZLStreamAdapter {
private:
	
	z_stream			mZStream;				// underlying zip stream state
	int					mWindowBits;
	
	void*				mInputChunk;			// compressed chunk from file
	
	// we use a pair of cache blocks to avoid 'thrashing' when seeking and reading back and
	// forth across a block boundary. in other words the user may seek back and forth within
	// the scope of a single cache block and only pay (at most) one reload of one block.
	
	void*				mCache;					// decompressed data cache
	size_t				mCacheSize;				// total size of cache
	ZLStreamChunk		mChunk [ 2 ];			// structure to hold chunk info

	//----------------------------------------------------------------//
	int					AffirmChunk				( int chunkID );
	size_t				Inflate					( void* dest, size_t size );
	void				InflateChunk			( ZLStreamChunk& chunk );
	void				OnClose					();
	bool				OnOpen					();
	size_t				ReadBytes				( void* buffer, size_t size );
	int					ResetZipStream			();

public:

	GET_SET ( int, WindowBits, mWindowBits )

	//----------------------------------------------------------------//
	u32					GetCaps					();
	int					SetCursor				( long offset );
						ZLDeflateReader			();
						~ZLDeflateReader		();
};

#endif