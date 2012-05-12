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

	//----------------------------------------------------------------//
	void			Close				();
	void			Flush				();
	u32				GetCursor			();
	ZLFILE*			GetFile				();
	u32				GetLength			();
	bool			OpenRead			( cc8* filename );
	bool			OpenWrite			( cc8* filename, bool affirmPath = true );
	u32				ReadBytes			( void* buffer, u32 size );
	void			Seek				( long offset, int origin );
					USFileStream		();
					~USFileStream		();
	u32				WriteBytes			( const void* buffer, u32 size );
};

#endif
