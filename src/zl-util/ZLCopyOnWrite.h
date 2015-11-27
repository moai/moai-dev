// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCOPYONWRITE_H
#define ZLCOPYONWRITE_H

#include <zl-util/ZLRefCountedObject.h>
#include <zl-util/ZLStream.h>

class ZLAllocator;

//================================================================//
// ZLCopyOnWriteBuffer
//================================================================//
class ZLCopyOnWriteBuffer :
	public ZLSharedMutableBuffer {
private:

	friend class ZLCopyOnWrite;

	void*				mBuffer;
	size_t				mSize;
	size_t				mLength;

public:

	GET_CONST ( void*, ConstData, mBuffer )
	GET ( void*, MutableData, mBuffer )
	GET ( size_t, Size, mLength )
	
	//----------------------------------------------------------------//
						ZLCopyOnWriteBuffer		();
						~ZLCopyOnWriteBuffer	();
};

//================================================================//
// ZLCopyOnWrite
//================================================================//
class ZLCopyOnWrite :
	public virtual ZLStream {
private:

	ZLCopyOnWriteBuffer*	mInternal;
	size_t					mCursor;
	
	ZLAllocator*			mAllocator;

	//----------------------------------------------------------------//
	void				AffirmInternal			();
	ZLAllocator&		DefaultAllocator		();

public:

	GET ( ZLSharedConstBuffer*, SharedConstBuffer, mInternal )
	GET_SET ( ZLAllocator*, Allocator, mAllocator )

	//----------------------------------------------------------------//
	void*				Alloc					( size_t size );
	void*				Alloc					( size_t size, u8 fill );
	void*				Alloc					( size_t size, const void* fill );
	void				Assign					( const ZLCopyOnWrite& assign );
	void				Free					();
	const void*			GetBuffer				() const;
	u32					GetCaps					();
	size_t				GetCursor				();
	size_t				GetLength				();
	size_t				GetSize					() const;
	void*				Invalidate				();
	size_t				ReadBytes				( void* buffer, size_t size );
	void*				Reserve					( size_t size );
	int					SetCursor				( long offset );
	size_t				SetLength				( size_t length );
	size_t				WriteBytes				( const void* buffer, size_t size );
						ZLCopyOnWrite			();
						ZLCopyOnWrite			( size_t size );
						ZLCopyOnWrite			( size_t size, u8 fill );
						ZLCopyOnWrite			( size_t size, const void* fill );
						ZLCopyOnWrite			( const ZLCopyOnWrite& assign );
						~ZLCopyOnWrite			();
	
	//----------------------------------------------------------------//
	inline ZLCopyOnWrite& operator= ( const ZLCopyOnWrite& rhs ) {
		this->Assign ( rhs );
		return *this;
	}
};

#endif
