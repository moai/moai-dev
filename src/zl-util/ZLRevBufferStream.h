// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREVBUFFERSTREAM_H
#define ZLREVBUFFERSTREAM_H

// "There are only two hard things in Computer Science: cache invalidation and naming things."
// -- Phil Karlton

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLLeanStack.h>
#include <zl-util/ZLStream.h>

class ZLRevBufferStream;

//================================================================//
// ZLRevBufferAllocator
//================================================================//
class ZLRevBufferAllocator {
private:

public:

	//----------------------------------------------------------------//
	virtual void*		Alloc						( size_t ) = 0;
	virtual void		Free						( void* buffer ) = 0;
						ZLRevBufferAllocator		();
	virtual				~ZLRevBufferAllocator		();
};

//================================================================//
// ZLRevBufferEdition
//================================================================//
class ZLRevBufferEdition {
private:

	friend class ZLRevBufferStream;

	u32					mRefCount;
	void*				mData;
	size_t				mSize;
	
	ZLRevBufferAllocator*	mAllocator;

	//----------------------------------------------------------------//
						ZLRevBufferEdition			();
						~ZLRevBufferEdition			();

public:

	GET ( const void*, Data, mData );

	//----------------------------------------------------------------//
	void				Release						();
	void				Retain						();
};

//================================================================//
// ZLRevBufferStream
//================================================================//
class ZLRevBufferStream :
	public virtual ZLStream {
private:

	friend class ZLRevBufferEdition;

	size_t					mSize;
	size_t					mLength;
	size_t					mEditionLength;

	size_t					mCursor;

	ZLRevBufferEdition*		mEdition;

public:

	GET ( size_t, Size, mSize )

	//----------------------------------------------------------------//
	void					Clear					();
	size_t					GetCapacity				();
	u32						GetCaps					();
	size_t					GetCursor				();
	void*					GetData					();
	ZLRevBufferEdition*		GetEdition				( ZLRevBufferAllocator* allocator = 0 );
	size_t					GetLength				();
	void					MakeDirty				();
	size_t					ReadBytes				( void* buffer, size_t size );
	void					Reserve					( size_t size );
	void					Reset					();
	int						SetCursor				( long offset );
	size_t					SetLength				( size_t length );
	size_t					WriteBytes				( const void* buffer, size_t size );
							ZLRevBufferStream		();
							~ZLRevBufferStream		();
};

#endif
