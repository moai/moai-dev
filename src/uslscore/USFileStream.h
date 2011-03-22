// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef FILESTREAM_H
#define FILESTREAM_H

/** @addtogroup StreamLib */

#include <uslscore/USStream.h>

//================================================================//
// USFileStream
//================================================================//
/**	@brief File-based stream.
	@ingroup StreamLib
*/
class USFileStream :
	public USStream {
private:

	FILE*		mFile;
	u32			mLength;

public:

	//----------------------------------------------------------------//
	void		Close				();
	void		Flush				();
	u32			GetCursor			();
	FILE*		GetFile				();
	u32			GetLength			();
	bool		OpenRead			( cc8* filename );
	bool		OpenWrite			( cc8* filename, bool affirmPath = true );
	u32			ReadBytes			( void* buffer, u32 size );
	void		Seek				( long offset, int origin );
				USFileStream		();
				~USFileStream		();
	u32			WriteBytes			( const void* buffer, u32 size );
};

#endif
