// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANSTACK_H
#define	ZLLEANSTACK_H

#include <zl-util/ZLLeanArray.h>

//================================================================//
// ZLLeanStack
//================================================================//
template < typename TYPE, u32 CHUNKSIZE >
class ZLLeanStack :
	public ZLLeanArray < TYPE > {
private:

	u32 mTop;

public:
	
	//----------------------------------------------------------------//
	u32 GetTop () const {
		
		return this->mTop;
	}
	
	//----------------------------------------------------------------//
	void Pop () {
	
		if ( this->mTop ) {
			--this->mTop;
		}
	}
	
	//----------------------------------------------------------------//
	void Push ( const TYPE& type ) {
	
		this->Grow ( this->mTop + 1, CHUNKSIZE );
		this->mData [ this->mTop++ ] = type;
	}
	
	//----------------------------------------------------------------//
	void Reset () {
		this->mTop = 0;
	}
	
	//----------------------------------------------------------------//
	void SetTop ( u32 top ) {
	
		this->Grow ( top, CHUNKSIZE );
		this->mTop = top;
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
