// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBASE64READER_H
#define ZLBASE64READER_H

#include <zl-util/ZLBase64Encoder.h>
#include <zl-util/ZLStreamReader.h>

//================================================================//
// ZLBase64Reader
//================================================================//
class ZLBase64Reader :
	public ZLStreamReader {
private:

	ZLStream*			mInputStream;			// compressed input stream
	size_t				mInputBase;				// base of compressed stream (when opened)
	
	size_t				mCursor;				// cursor in the output stream
	size_t				mSize;					// size (if known)
	size_t				mLength;

	ZLBase64Encoder		mEncoder;
	u8					mPlainBlock [ ZLBase64Encoder::PLAIN_BLOCK_SIZE ];
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
	bool				Open					( ZLStream& stream );
	bool				Open					( ZLStream& stream, size_t size );
	size_t				ReadBytes				( void* buffer, size_t size );
	int					SetCursor				( long offset );
						ZLBase64Reader			();
						~ZLBase64Reader			();
};

#endif