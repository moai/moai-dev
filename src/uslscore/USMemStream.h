// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MEMSTREAM_H
#define MEMSTREAM_H

#include <uslscore/STLString.h>
#include <uslscore/USStream.h>

//================================================================//
// USMemStream
//================================================================//
class USMemStream :
	public USStream {
private:

	static const u32 DEFAULT_CHUNK_SIZE = 2048;

	u32			mChunkSize;
	u32			mTotalChunks;
	void**		mChunks;

	u32			mCursor;
	u32			mLength;

public:

	//----------------------------------------------------------------//
	void		Clear			();
	u32			GetCursor		();
	u32			GetLength		();
	u32			ReadBytes		( void* buffer, u32 size );
	void		Reserve			( u32 length );
	void		Seek			( long offset, int origin );
	void		SetChunkSize	( u32 chunkSize );
	STLString	ToString		( u32 size );
				USMemStream		();
				~USMemStream	();
	u32			WriteBytes		( const void* buffer, u32 size );
};

#endif
