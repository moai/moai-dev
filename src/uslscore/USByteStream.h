// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef BYTESTREAM_H
#define BYTESTREAM_H

/** @addtogroup StreamLib */

#include <uslscore/USStream.h>

//================================================================//
// USByteStream
//================================================================//
/**	@brief Memory stream.  Fixed size byte buffer must be allocated
	in advance; will not grow to accomodate writing to stream.
	@ingroup StreamLib
*/
class USByteStream :
	public USStream {
private:

	void*		mBuffer;
	u32			mCursor;
	u32			mLength;
	u32			mCapacity;

public:

	//----------------------------------------------------------------//
	u32			GetCapacity		();
	u32			GetCursor		();
	void*		GetBuffer		();
	u32			GetLength		();
	u32			ReadBytes		( void* buffer, u32 size );
	void		Seek			( long offset, int origin );
	void		SetBuffer		( void* buffer, u32 size );
				USByteStream	();
				~USByteStream	();
	u32			WriteBytes		( const void* buffer, u32 size );
};

#endif
