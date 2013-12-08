// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHEXREADER_H
#define ZLHEXREADER_H

#include <zl-util/ZLStreamReader.h>

//================================================================//
// ZLHexReader
//================================================================//
class ZLHexReader :
	public ZLStreamReader {
private:

	ZLStream*			mInputStream;
	size_t				mCursor;				// cursor in the output stream

public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( ZLStream& stream );
	size_t				ReadBytes				( void* buffer, size_t size );
						ZLHexReader				();
						~ZLHexReader			();
};

#endif