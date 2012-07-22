// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHEXWRITER_H
#define USHEXWRITER_H

#include <uslscore/USStreamWriter.h>

//================================================================//
// USHexWriter
//================================================================//
class USHexWriter :
	public USStreamWriter {
private:

	USStream*			mOutputStream;
	size_t				mCursor;				// cursor in the input stream

public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( USStream* stream );
						USHexWriter				();
						~USHexWriter			();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
