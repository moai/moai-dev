// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREVBUFFER_H
#define ZLREVBUFFER_H

// "There are only two hard things in Computer Science: cache invalidation and naming things."
// -- Phil Karlton

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLLeanStack.h>

class ZLRevBuffer;

//================================================================//
// ZLRevBufferHandle
//================================================================//
class ZLRevBufferHandle {
private:

	friend class ZLRevBufferEdition;
	friend class ZLRevBuffer;

	u32				mRefCount;
	ZLRevBuffer*	mBuffer;
	
	//----------------------------------------------------------------//
	void			Release			();
	void			Retain			();
};

//================================================================//
// ZLRevBufferEdition
//================================================================//
class ZLRevBufferEdition {
private:

	friend class ZLRevBuffer;

	u32					mRefCount;
	void*				mData;
	size_t				mSize;

	ZLRevBufferHandle*	mHandle;

	GET ( void*, MutableData, mData );

	//----------------------------------------------------------------//
	void				Init						( ZLRevBufferHandle& handle, size_t size );
	void				Retain						();
						ZLRevBufferEdition			();
						~ZLRevBufferEdition			();

public:

	GET ( const void*, ImmutableData, mData );

	//----------------------------------------------------------------//
	void				Release						();
};

//================================================================//
// ZLRevBuffer
//================================================================//
class ZLRevBuffer {
protected:

	friend class ZLRevBufferEdition;

	size_t		mSize;

private:
	
	ZLRevBufferHandle*		mHandle;
	
	bool					mLocked;
	ZLRevBufferEdition*		mCurrentEdition;

	ZLLeanStack < ZLRevBufferEdition* > mCache;

	//----------------------------------------------------------------//
	void					Reuse					( ZLRevBufferEdition* edition );

public:

	GET ( size_t, Size, mSize )

	//----------------------------------------------------------------//
	void					Clear					();
	ZLRevBufferEdition*		GetCurrentEdition		();
	const void*				GetImmutableData		();
	void*					GetMutableData			();
	void					Init					( size_t size );
							ZLRevBuffer				();
							~ZLRevBuffer			();
};

#endif
