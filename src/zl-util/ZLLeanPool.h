// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANPOOL_H
#define	ZLLEANPOOL_H

#include <zl-util/ZLLeanArray.h>
#include <zl-util/ZLLeanStack.h>

//================================================================//
// ZLLeanPool
//================================================================//
template < typename TYPE, ZLSize CHUNKSIZE = 64, ZLSize MAXCHUNKS = 0 >
class ZLLeanPool {
private:

	ZLSize								mMaxChunks;
	ZLLeanStack < TYPE*, 1 >			mChunks;
	ZLLeanStack < TYPE*, CHUNKSIZE >	mFree;

	//----------------------------------------------------------------//
	void More () {
	
		if (( this->mMaxChunks == 0 ) || ( this->mChunks.Size () < this->mMaxChunks )) {
	
			TYPE* chunk = ( TYPE* )malloc ( CHUNKSIZE * sizeof ( TYPE ));
			this->mChunks.Push ( chunk );
			
			for ( ZLIndex i = ZLIndex::ZERO; i < CHUNKSIZE; ++i ) {
				TYPE* elem = &chunk [ i ];
				this->mFree.Push ( elem );
			}
		}
	}

public:

	//----------------------------------------------------------------//
	TYPE* Alloc () {
		
		ZLSize top = this->mFree.GetTop ();
		
		if ( !top ) {
			this->More ();
		}
		
		top = this->mFree.GetTop ();
		if ( top ) {
		
			TYPE* type = this->mFree.Pop ();
			new ( type ) TYPE ();
			return type;
		}
		return 0;
	}

	//----------------------------------------------------------------//
	void Clear () {
		
		ZLSize top = this->mChunks.GetTop ();
		for ( ZLIndex i = ZLIndex::ZERO; i < top; ++i ) {
			free ( this->mChunks [ i ]);
		}
		
		this->mChunks.Clear ();
		this->mFree.Clear ();
	}

	//----------------------------------------------------------------//
	void Free ( TYPE* type ) {
		
		if ( type ) {
			type->~TYPE ();
			this->mFree.Push ( type );
		}
	}
	
	//----------------------------------------------------------------//
	ZLLeanPool () :
		mMaxChunks ( MAXCHUNKS ) {
	}
	
	//----------------------------------------------------------------//
	~ZLLeanPool () {
		
		this->Clear ();
	}
};

#endif
