// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBYTESTREAM_H
#define ZLBYTESTREAM_H

#include <zl-util/ZLStream.h>

//================================================================//
// ZLByteStream
//================================================================//
class ZLByteStream :
	public virtual ZLStream {
private:

	const void*		mReadBuffer;
	void*			mWriteBuffer;
	size_t			mCursor;
	size_t			mLength;
	size_t			mCapacity;

	//----------------------------------------------------------------//
	ZLResultCode	SetCursor			( size_t offset );

public:

	//----------------------------------------------------------------//
	void			Clear				();
	size_t			GetCapacity			();
	u32				GetCaps				();
	size_t			GetCursor			();
	size_t			GetLength			();
	const void*		GetReadBuffer		();
	void*			GetWriteBuffer		();
	ZLSizeResult	ReadBytes			( void* buffer, size_t size );
	void			SetBuffer			( void* buffer, size_t size, size_t length = 0 );
	void			SetBuffer			( const void* buffer, size_t size, size_t length = 0 );
	ZLSizeResult	SetLength			( size_t length );
	ZLSizeResult	WriteBytes			( const void* buffer, size_t size );
	void			WriteBytesUnsafe	( const void* buffer, size_t size );
					ZLByteStream		();
					~ZLByteStream		();
		
	//----------------------------------------------------------------//
	template < typename TYPE >
	void WriteUnsafe ( TYPE value ) {
		this->WriteBytesUnsafe ( &value, sizeof ( TYPE ));
	}
};

#endif
