// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANSTACK_H
#define	ZLLEANSTACK_H

#include <zl-core/ZLLeanArray.h>
#include <zl-core/ZLType.h>

//================================================================//
// ZLLeanStack
//================================================================//
template < typename TYPE, ZLSize CHUNKSIZE = 64 >
class ZLLeanStack :
	public ZLLeanArray < TYPE > {
protected:

	ZLSize mTop;

public:
	
	//----------------------------------------------------------------//
	ZLSize GetTop () const {
		
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
		return ZLType::Dummy < TYPE >();
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
		return ZLType::Dummy < TYPE >();
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

		if (( this->GrowChunked ( this->mTop + 1, CHUNKSIZE ) != ZL_OK ) || ( !this->mData )) {
			assert ( false );
			return ZLType::DummyRef < TYPE >();
		}
		
		new ( &this->mData [ this->mTop ]) TYPE (); // placement new
		return this->mData [ this->mTop++ ]; // Note the post-increment
	}
	
	//----------------------------------------------------------------//
	TYPE& Push ( ZLResultCode& result ) {
	
		if (( this->GrowChunked ( this->mTop + 1, CHUNKSIZE ) != ZL_OK ) || ( !this->mData )) {
			result = ZL_ALLOCATION_ERROR;
			return ZLType::DummyRef < TYPE >();
		}
		result = ZL_OK;
		
		new ( &this->mData [ this->mTop ]) TYPE (); // placement new
		return this->mData [ this->mTop++ ]; // Note the post-increment
	}
	
	//----------------------------------------------------------------//
	TYPE& Push ( const TYPE& type ) {
		
		if (( this->GrowChunked ( this->mTop + 1, CHUNKSIZE ) != ZL_OK ) || ( !this->mData )) {
			assert ( false );
			return ZLType::DummyRef < TYPE >();
		}
		
		new ( &this->mData [ this->mTop ]) TYPE ( type ); // placement copy constructor
		return this->mData [ this->mTop++ ]; // Note the post-increment
	}
	
	//----------------------------------------------------------------//
	TYPE& Push ( const TYPE& type, ZLResultCode& result ) {
		
		if (( this->GrowChunked ( this->mTop + 1, CHUNKSIZE ) != ZL_OK ) || ( !this->mData )) {
			result = ZL_ALLOCATION_ERROR;
			return ZLType::DummyRef < TYPE >();
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
	ZLResultCode Resize ( ZLSize size ) {

		if ( this->mSize != size ) {

			TYPE* data = 0;
			ZLSize top = 0;
			
			if ( size ) {
			
				data = ( TYPE* )malloc ( size * sizeof ( TYPE ));
				if ( !data ) return ZL_ALLOCATION_ERROR;

				top = ( this->mTop < size ) ? this->mTop : size;

				for ( ZLSize i = 0; i < top; ++i ) {
					new ( &data [ i ]) TYPE ( this->mData [ i ]); // placement new
				}
			}

			if ( this->mData ) {
				for ( ZLSize i = this->mTop; i > 0; --i ) {
					this->mData [ i - 1 ].~TYPE ();
				}
				free ( this->mData );
			}

			this->mData = data;
			this->mSize = size;
			this->mTop = top;
		}
		return ZL_OK;
	}
	
	//----------------------------------------------------------------//
	ZLResultCode SetTop ( ZLSize top ) {
	
		if ( this->GrowChunked ( top, CHUNKSIZE ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
		this->mTop = top;
		return ZL_OK;
	}
	
	//----------------------------------------------------------------//
	ZLResultCode SetTop ( ZLSize top, const TYPE& value ) {
	
		if ( this->GrowChunked ( top, CHUNKSIZE, value ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
		this->mTop = top;
		
		for ( ZLSize i = 0; i < this->mTop; ++i ) {
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
			return ZLType::DummyRef < TYPE >();
		}
		return this->mData [ this->mTop - 1 ];
	}
	
	//----------------------------------------------------------------//
	ZLLeanStack () :
		mTop ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLLeanStack () {
		this->Resize ( 0 );
	}
};

#endif
