// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USBASE64READER_H
#define USBASE64READER_H

#include <uslscore/USBase64Encoder.h>
#include <uslscore/USStreamReader.h>

//================================================================//
// USBase64Reader
//================================================================//
class USBase64Reader :
	public USStreamReader {
private:

	USStream*			mInputStream;			// compressed input stream
	size_t				mInputBase;				// base of compressed stream (when opened)
	
	size_t				mCursor;				// cursor in the output stream
	size_t				mSize;					// size (if known)
	size_t				mLength;

	USBase64Encoder		mEncoder;
	u8					mPlainBlock [ USBase64Encoder::PLAIN_BLOCK_SIZE ];
	size_t				mBlockID;
	u32					mBlockTop;

	//----------------------------------------------------------------//
	void				SyncBlock				();

public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				IsAtEnd					();
	bool				Open					( USStream* stream );
	bool				Open					( USStream* stream, size_t size );
	size_t				ReadBytes				( void* buffer, size_t size );
	int					SetCursor				( long offset );
						USBase64Reader			();
						~USBase64Reader			();
};

#endif