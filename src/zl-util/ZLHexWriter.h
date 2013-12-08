// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHEXWRITER_H
#define ZLHEXWRITER_H

#include <zl-util/ZLStreamWriter.h>

//================================================================//
// ZLHexWriter
//================================================================//
class ZLHexWriter :
	public ZLStreamWriter {
private:

	ZLStream*			mOutputStream;
	size_t				mCursor;				// cursor in the input stream

public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( ZLStream& stream );
						ZLHexWriter				();
						~ZLHexWriter			();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
