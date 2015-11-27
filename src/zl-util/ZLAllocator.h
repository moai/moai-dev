// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLALLOCATOR_H
#define ZLALLOCATOR_H

#include <zl-util/STLMap.h>
#include <zl-util/STLSet.h>
#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLAllocator
//================================================================//
class ZLAllocator {
protected:

	//----------------------------------------------------------------//
	static size_t		Pad			( size_t size, u32 paddingScheme, size_t paddedChunkSize = 0 );


public:

	enum {
		NO_PADDING,
		PAD_TO_POW2,
		PAD_TO_CHUNK,
	};

	//----------------------------------------------------------------//
	virtual void*		Alloc		( size_t size ) = 0;
	virtual void		Free		( void* mem ) = 0;

	//----------------------------------------------------------------//
	ZLAllocator () {
	}

	//----------------------------------------------------------------//
	virtual ~ZLAllocator () {
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void Delete ( TYPE* type ) {
		
		if ( type ) {
			type->TYPE::~TYPE ();
			this->Free ( type );
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* New () {
		
		TYPE* type = ( TYPE* )this->Alloc ( sizeof ( TYPE ));
		return type ? new ( type ) TYPE () : 0;
	}
};

//================================================================//
// ZLSimpleAllocator
//================================================================//
class ZLSimpleAllocator :
	public ZLAllocator {
public:

	//----------------------------------------------------------------//
	void* Alloc ( size_t size ) {
	
		return malloc ( size );
	}
	
	//----------------------------------------------------------------//
	void Free ( void* mem ) {
	
		if ( mem ) {
			free ( mem );
		}
	}

	//----------------------------------------------------------------//
	ZLSimpleAllocator () {
	}

	//----------------------------------------------------------------//
	virtual ~ZLSimpleAllocator () {
	}
};

//================================================================//
// ZLTemporalAllocator
//================================================================//
class ZLTemporalAllocator :
	public ZLAllocator {
private:

	//================================================================//
	// EntrySet
	//================================================================//
	class EntrySet {
	public:
	
		u32						mAge;
		STLSet < void* >		mBuffers;
	};

	typedef STLMap < size_t, EntrySet >::iterator	EntrySetsIteratorIt;
	STLMap < size_t, EntrySet >						mEntrySetsBySize;
	
	STLMap < void*, size_t >	mSizesByAddress;

	u32		mPaddingScheme;
	u32		mPaddedChunkSize;
	u32		mMaximumAge;

public:
	
	GET_SET ( u32, PaddingScheme, mPaddingScheme )
	GET_SET ( u32, PaddedChunkSize, mPaddedChunkSize )
	GET_SET ( u32, MaximumAge, mMaximumAge )

	//----------------------------------------------------------------//
	void			Age							();
	void*			Alloc						( size_t size );
	void			Free						( void* mem );
					ZLTemporalAllocator			();
					~ZLTemporalAllocator		();
};

//================================================================//
// ZLLinearAllocator
//================================================================//
class ZLLinearAllocator :
	public ZLAllocator {
private:

	static const int DEFAULT_RESIZE_DELAY = 60; // 60 frames


	STLMap < void*, size_t >	mSizesByAddress;
	
	size_t		mAllocated;
	size_t		mMaxAllocated;
	
	size_t		mBufferSize;
	size_t		mBufferAllocated;
	size_t		mBufferTop;
	void*		mBuffer;

	int		mUpdateCounter;
	int		mResizeDelay;

public:

	//----------------------------------------------------------------//
	void*			Alloc						( size_t size );
	void			Free						( void* mem );
	void			Update						( u32 paddingScheme = PAD_TO_POW2, u32 paddedChunkSize = 0 );
					ZLLinearAllocator			();
					~ZLLinearAllocator			();
};

#endif
