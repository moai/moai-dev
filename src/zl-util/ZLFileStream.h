// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLFILESTREAM_H
#define ZLFILESTREAM_H

#include <zl-util/ZLStream.h>

//================================================================//
// ZLFileStream
//================================================================//
class ZLFileStream :
	public virtual ZLStream {
protected:

	ZLFILE*			mFile;
	size_t			mLength;
	u32				mCaps;

	//----------------------------------------------------------------//
	int				SetCursor			( long offset );

public:

	enum {
		APPEND,
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
	u32				GetCaps				();
	size_t			GetCursor			();
	ZLFILE*			GetFile				();
	size_t			GetLength			();
	bool			IsOpen				();
	bool			Open				( cc8* filename, u32 mode );
	bool			OpenRead			( cc8* filename );
	bool			OpenWrite			( cc8* filename );
	size_t			ReadBytes			( void* buffer, size_t size );
	size_t			WriteBytes			( const void* buffer, size_t size );
					ZLFileStream		();
					~ZLFileStream		();
};

#endif
