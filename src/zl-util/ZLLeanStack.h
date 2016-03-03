// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANSTACK_H
#define	ZLLEANSTACK_H

#include <zl-util/ZLLeanArray.h>
#include <zl-util/ZLTypeID.h>

//================================================================//
// ZLLeanStack
//================================================================//
template < typename TYPE, size_t CHUNKSIZE = 64 >
class ZLLeanStack :
	public ZLLeanArray < TYPE > {
private:

	size_t mTop;

	//----------------------------------------------------------------//
	virtual ZLResultCode Alloc ( size_t size ) {

		this->mData = ( TYPE* )malloc ( size * sizeof ( TYPE ));
		return this->mData ? ZL_OK : ZL_ALLOCATION_ERROR;
	}

	//----------------------------------------------------------------//
	virtual void Free () {

		free ( this->mData );
	}

public:
	
	//----------------------------------------------------------------//
	void Clear () {

		this->ZLLeanArray < TYPE >::Clear ();
		this->mTop = 0;
	}
	
	//----------------------------------------------------------------//
	size_t GetTop () const {
		
		return this->mTop;
	}
	
	//----------------------------------------------------------------//
	TYPE Pop () {
		
		if ( this->mTop ) {
			TYPE value = this->mData [ --this->mTop ]; // Note the pre-decrement
			this->mData [ this->mTop ].~TYPE ();
			return value;
		}
		assert ( false );
		return ZLTypeID < TYPE >::Dummy ();
	}
	
	//----------------------------------------------------------------//
	// may cause copy constructor to be called twice
	TYPE Pop ( ZLResultCode& result ) {
	
		if ( this->mTop ) {
			result = ZL_OK;
			TYPE value = this->mData [ --this->mTop ]; // Note the pre-decrement
			this->mData [ this->mTop ].~TYPE ();
			return value;
		}
		result = ZL_ERROR;
		return ZLTypeID < TYPE >::Dummy ();
	}
	
	//----------------------------------------------------------------//
	// guarantee no more than one call to copy constructor
	ZLResultCode Pop ( TYPE* value ) {
	
		if ( this->mTop ) {
			if ( value ) {
				*value = this->mData [ this->mTop - 1 ];
			}
			this->mData [ --this->mTop ].~TYPE (); // Note the pre-decrement
			return ZL_OK;
		}
		return ZL_ERROR;
	}
	
	//----------------------------------------------------------------//
	TYPE& Push () {
		
		ZLResultCode result = this->Grow ( this->mTop + 1, CHUNKSIZE );
		assert ( result == ZL_OK );
		
		new ( &this->mData [ this->mTop ]) TYPE (); // placement new
		return this->mData [ this->mTop++ ]; // Note the post-increment
	}
	
	//----------------------------------------------------------------//
	TYPE& Push ( ZLResultCode& result ) {
	
		if ( this->Grow ( this->mTop + 1, CHUNKSIZE ) != ZL_OK ) {
			result = ZL_ALLOCATION_ERROR;
			return ZLTypeID < TYPE >::DummyRef ();
		}
		result = ZL_OK;
		
		new ( &this->mData [ this->mTop ]) TYPE (); // placement new
		return this->mData [ this->mTop++ ]; // Note the post-increment
	}
	
	//----------------------------------------------------------------//
	TYPE& Push ( const TYPE& type ) {
		
		ZLResultCode result = this->Grow ( this->mTop + 1, CHUNKSIZE );
		assert ( result == ZL_OK );
		new ( &this->mData [ this->mTop ]) TYPE ( type ); // placement copy constructor
		return this->mData [ this->mTop++ ]; // Note the post-increment
	}
	
	//----------------------------------------------------------------//
	TYPE& Push ( const TYPE& type, ZLResultCode& result ) {
		
		if ( this->Grow ( this->mTop + 1, CHUNKSIZE ) != ZL_OK ) {
			result = ZL_ALLOCATION_ERROR;
			return ZLTypeID < TYPE >::DummyRef ();
		}
		result = ZL_OK;
		new ( &this->mData [ this->mTop ]) TYPE ( type ); // placement copy constructor
		return this->mData [ this->mTop++ ]; // Note the post-increment
	}
	
	//----------------------------------------------------------------//
	void Reset () {
		this->mTop = 0;
	}
	
	//----------------------------------------------------------------//
	ZLResultCode SetTop ( size_t top ) {
	
		if ( this->Grow ( top, CHUNKSIZE ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
		this->mTop = top;
		return ZL_OK;
	}
	
	//----------------------------------------------------------------//
	ZLResultCode SetTop ( size_t top, const TYPE& value ) {
	
		if ( this->Grow ( top, CHUNKSIZE, value ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
		this->mTop = top;
		
		for ( size_t i = 0; i < this->mTop; ++i ) {
			new ( &this->mData [ i ]) TYPE ();
		}
		return ZL_OK;
	}
	
	//----------------------------------------------------------------//
	TYPE& Top () {
		assert ( this->mTop > 0 );
		return this->mData [ this->mTop - 1 ];
	}
	
	//----------------------------------------------------------------//
	TYPE& Top ( ZLResultCode& result ) {
		if ( this->mTop == 0 ) {
			result = ZL_ERROR;
			return ZLTypeID < TYPE >::DummyRef ();
		}
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
