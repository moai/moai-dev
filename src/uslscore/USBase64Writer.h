// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USBASE64WRITER_H
#define USBASE64WRITER_H

#include <uslscore/USBase64Encoder.h>
#include <uslscore/USStreamWriter.h>

//================================================================//
// USBase64Writer
//================================================================//
class USBase64Writer :
	public USStreamWriter {
private:

	USStream*			mOutputStream;			// compressed output stream
	size_t				mCursor;				// cursor in the input stream
	
	USBase64Encoder		mEncoder;
	u8					mPlainBlock [ USBase64Encoder::PLAIN_BLOCK_SIZE ];

public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( USStream* stream );
						USBase64Writer			();
						~USBase64Writer			();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
