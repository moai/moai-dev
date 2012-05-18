// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <uslscore/USStream.h>

//================================================================//
// USFileStream
//================================================================//
class USFileStream :
	public USStream {
protected:

	ZLFILE*			mFile;
	u32				mLength;

public:

	enum {
		READ,				// existing file just for reading (rb)
		READ_WRITE,			// existing file for reading and writing (rb+)
		READ_WRITE_AFFIRM,	// new or existing file for reading and writing (rb+, fallback on wb+)
		READ_WRITE_NEW,		// new file for reading and writing (wb+)
		WRITE,				// new file just for writing (wb)
	};
	
	static const u32 DEFAULT_FILE_MODE = READ;

	//----------------------------------------------------------------//
	void			Close				();
	void			Flush				();
	u32				GetCursor			();
	ZLFILE*			GetFile				();
	u32				GetLength			();
	bool			Open				( cc8* filename, u32 mode );
	bool			OpenRead			( cc8* filename );
	bool			OpenWrite			( cc8* filename );
	u32				ReadBytes			( void* buffer, u32 size );
	void			Seek				( long offset, int origin );
					USFileStream		();
					~USFileStream		();
	u32				WriteBytes			( const void* buffer, u32 size );
};

#endif
