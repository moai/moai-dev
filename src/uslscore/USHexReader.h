// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHEXREADER_H
#define USHEXREADER_H

#include <uslscore/USStreamReader.h>

//================================================================//
// USHexReader
//================================================================//
class USHexReader :
	public USStreamReader {
private:

	USStream*			mInputStream;
	size_t				mCursor;				// cursor in the output stream

public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( USStream* stream );
	size_t				ReadBytes				( void* buffer, size_t size );
						USHexReader				();
						~USHexReader			();
};

#endif