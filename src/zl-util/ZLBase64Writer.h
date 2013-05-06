// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBASE64WRITER_H
#define ZLBASE64WRITER_H

#include <zl-util/ZLBase64Encoder.h>
#include <zl-util/ZLStreamWriter.h>

//================================================================//
// ZLBase64Writer
//================================================================//
class ZLBase64Writer :
	public ZLStreamWriter {
private:

	ZLStream*			mOutputStream;			// compressed output stream
	size_t				mCursor;				// cursor in the input stream
	
	ZLBase64Encoder		mEncoder;
	u8					mPlainBlock [ ZLBase64Encoder::PLAIN_BLOCK_SIZE ];

public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( ZLStream& stream );
						ZLBase64Writer			();
						~ZLBase64Writer			();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
