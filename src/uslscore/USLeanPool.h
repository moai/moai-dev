// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	LEANPOOL_H
#define	LEANPOOL_H

#include <uslscore/USLeanArray.h>

//================================================================//
// USLeanPool
//================================================================//
template < typename TYPE >
class USLeanPool {
private:

	USLeanArray < TYPE* > mStack;
	TYPE* mPool;

	u32 mTop;

public:

	//----------------------------------------------------------------//
	TYPE* Alloc () {
		
		if ( this->mTop ) {
			TYPE* type = this->mStack [ --this->mTop ];
			new ( type ) TYPE ();
			return type;
		}
		return 0;
	}

	//----------------------------------------------------------------//
	void Clear () {
		
		u32 max = this->mStack.Size ();
		for ( u32 i = this->mTop; i < max; ++i ) {
			TYPE* type = this->mStack [ i ];
			type->TYPE::~TYPE ();
		}
		this->mTop = max;
	}

	//----------------------------------------------------------------//
	void Free ( TYPE* type ) {
		
		if ( type ) {
			type->TYPE::~TYPE ();
			this->mStack [ this->mTop++ ] = type;
		}
	}

	//----------------------------------------------------------------//
	void Init ( u32 max ) {
	
		this->mTop = max;
		
		this->mPool = ( TYPE* )malloc ( max * sizeof ( TYPE ));
		this->mStack.Init ( max );
		
		for ( u32 i = 0; i < max; ++i ) {
			this->mStack [ i ] = &this->mPool [ i ];
		}	
	}
	
	//----------------------------------------------------------------//
	~USLeanPool () {
		
		this->Clear ();
		free ( this->mPool );
		this->mPool = 0;
		this->mTop = 0;
	}
};

#endif
