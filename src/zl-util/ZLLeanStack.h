// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANSTACK_H
#define	ZLLEANSTACK_H

#include <zl-util/ZLLeanArray.h>

//================================================================//
// ZLLeanStack
//================================================================//
template < typename TYPE, size_t CHUNKSIZE = 64 >
class ZLLeanStack :
	public ZLLeanArray < TYPE > {
private:

	size_t mTop;

public:
	
	//----------------------------------------------------------------//
	size_t GetTop () const {
		
		return this->mTop;
	}
	
	//----------------------------------------------------------------//
	TYPE& Pop () {
	
		if ( this->mTop ) {
			return this->mData [ --this->mTop ];
		}
		assert ( false );
		return this->mData [ 0 ];
	}
	
	//----------------------------------------------------------------//
	TYPE& Push () {
	
		this->Grow ( ++this->mTop, CHUNKSIZE );
		return this->mData [ this->mTop - 1 ];
	}
	
	//----------------------------------------------------------------//
	TYPE& Push ( const TYPE& type ) {
		
		size_t idx = this->mTop;
		this->Grow ( ++this->mTop, CHUNKSIZE );
		this->mData [ idx ] = type;
		return this->mData [ idx ];
	}
	
	//----------------------------------------------------------------//
	void Reset () {
		this->mTop = 0;
	}
	
	//----------------------------------------------------------------//
	void SetTop ( size_t top ) {
	
		this->Grow ( top, CHUNKSIZE );
		this->mTop = top;
	}
	
	//----------------------------------------------------------------//
	TYPE& Top () {
		assert ( this->mTop > 0 );
		return this->mData [ this->mTop - 1 ];
	}
	
	//----------------------------------------------------------------//
	ZLLeanStack () :
		mTop ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLLeanStack () {
	}
};

#endif
