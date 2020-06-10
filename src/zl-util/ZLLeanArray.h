// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANARRAY_H
#define	ZLLEANARRAY_H

#include <zl-util/ZLResult.h>

#define ZL_DECLARE_SCOPED_BUFFER(name,type,size,stackAllocMax)									\
	ZLLeanArray < u8 >			_##name##_array;												\
	type* name					= ( type* )( size <= stackAllocMax ? alloca ( size ) : 0 );		\
	if ( !name ) {																				\
		_##name##_array.Init ( size );															\
		name = ( type* )_##name##_array.Data ();												\
	}

//================================================================//
// ZLLeanArray
//================================================================//
template < typename TYPE >
class ZLLeanArray {
protected:

	size_t	mSize;
	TYPE*	mData;

public:

	//----------------------------------------------------------------//
	ZLLeanArray < TYPE >& operator = ( const ZLLeanArray < TYPE >& assign ) {
		if ( &assign != this ) {
			this->CloneFrom ( assign );
		}
		return *this;
	};

	//----------------------------------------------------------------//
	inline operator TYPE* () const {
		return this->mData;
	};

	//----------------------------------------------------------------//
	size_t BufferSize () {
		return this->mSize * sizeof ( TYPE );
	}

	//----------------------------------------------------------------//
	void Clear () {

		this->Resize ( 0 );
	}

	//----------------------------------------------------------------//
	ZLResultCode CloneFrom ( const ZLLeanArray < TYPE >& src ) {

		if ( this->Init ( src.mSize ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
		this->CopyFrom ( src );
		return ZL_OK;
	}

	//----------------------------------------------------------------//
	void CopyFrom ( const ZLLeanArray < TYPE >& src ) {

		size_t total = ( this->mSize < src.mSize ) ? this->mSize : src.mSize;

		for ( size_t i = 0; i < total; ++i ) {
			this->mData [ i ] = src [ i ];
		}
	}

	//----------------------------------------------------------------//
	inline TYPE& Elem ( size_t idx ) {
		return this->mData [ idx ];
	}

	//----------------------------------------------------------------//
	inline TYPE* Data () {
		return this->mData;
	}

	//----------------------------------------------------------------//
	inline const TYPE* Data () const {
		return this->mData;
	}

	//----------------------------------------------------------------//
	void Fill ( const TYPE& value ) {
		for ( size_t i = 0; i < this->mSize; ++i ) {
			this->mData [ i ] = value;
		}
	}

	//----------------------------------------------------------------//
	ZLResultCode Grow ( size_t size ) {
	
		if ( size > this->mSize ) {
			return this->Resize ( size );
		}
		return ZL_OK;
	}
	
	//----------------------------------------------------------------//
	ZLResultCode Grow ( size_t size, const TYPE& value ) {
	
		if ( size > this->mSize ) {
			return this->Resize ( size, value );
		}
		return ZL_OK;
	}
	
	//----------------------------------------------------------------//
	ZLResultCode GrowChunked ( size_t size, size_t chunkSize ) {
		
		size_t chunks = ( size / chunkSize ) + (( size % chunkSize ) ? 1 : 0 );
		return this->Grow ( chunks * chunkSize );
	}

	//----------------------------------------------------------------//
	ZLResultCode GrowChunked ( size_t size, size_t chunkSize, const TYPE& value ) {
		
		size_t chunks = ( size / chunkSize ) + (( size % chunkSize ) ? 1 : 0 );
		return this->Resize ( chunks * chunkSize, value );
	}

	//----------------------------------------------------------------//
	ZLResultCode Init ( size_t size ) {

		return this->Resize ( size );
	}

	//----------------------------------------------------------------//
	bool IsIdentical ( const ZLLeanArray < TYPE >& array ) {
	
		if ( this->mSize == array.mSize ) {
			return ( this->mSize > 0 ) ? ( memcmp ( this->mData, array.mData, this->mSize ) == 0 ) : true;
		}
		return false;
	}

	//----------------------------------------------------------------//
	void Nullify () {
	
		mData = 0;
		mSize = 0;
	}

	//----------------------------------------------------------------//
	virtual ZLResultCode Resize ( size_t size ) {

		if ( this->mSize != size ) {

			TYPE* data = 0;

			if ( size ) {
			
				data = new TYPE [ size ];
				if ( !data ) return ZL_ALLOCATION_ERROR;

				size_t total = ( this->mSize < size ) ? this->mSize : size;
				
				for ( size_t i = 0; i < total; ++i ) {
					data [ i ] = this->mData [ i ];
				}
			}
			
			if ( this->mData ) {
				delete [] this->mData;
			}
			
			this->mData = data;
			this->mSize = size;
		}
		return ZL_OK;
	}

	//----------------------------------------------------------------//
	ZLResultCode Resize ( size_t size, const TYPE& value ) {

		size_t oldSize = this->mSize;
		
		if ( this->Resize ( size ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
		
		if ( size >= oldSize ) {
			for ( size_t i = oldSize; i < size; ++i ) {
				this->mData [ i ] = value;
			}
		}
		return ZL_OK;
	}

	//----------------------------------------------------------------//
	void RotateLeft ( size_t spaces ) {
	
		spaces = spaces % this->mSize;
		if ( spaces ) {
			
			size_t size = this->mSize * sizeof ( TYPE );
			size_t leftSize = spaces * sizeof ( TYPE );
			size_t rightSize = size - leftSize;
			
			void* lower = ( void* )(( size_t )this->mData + leftSize );
			void* upper = ( void* )(( size_t )this->mData + rightSize );
			
			void* temp = alloca ( leftSize );
		
			memcpy ( temp, this->mData, leftSize );
			memmove ( this->mData, lower, rightSize );
			memcpy ( upper, temp, leftSize );
		}
	}
	
	//----------------------------------------------------------------//
	void RotateRight ( size_t spaces ) {
	
		spaces = spaces % this->mSize;
		if ( spaces ) {
			
			size_t size = this->mSize * sizeof ( TYPE );
			size_t leftSize = spaces * sizeof ( TYPE );
			size_t rightSize = size - leftSize;
			
			void* lower = ( void* )(( size_t )this->mData + leftSize );
			void* upper = ( void* )(( size_t )this->mData + rightSize );
			
			void* temp = alloca ( rightSize );
		
			memcpy ( temp, upper, rightSize );
			memmove ( lower, this->mData, leftSize );
			memcpy ( this->mData, temp, rightSize );
		}
	}

	//----------------------------------------------------------------//
	inline size_t Size () const {
		return this->mSize;
	}

	//----------------------------------------------------------------//
	virtual void Take ( ZLLeanArray < TYPE >& src ) {
	
		if ( this->mData != src.mData ) {
			
			if ( this->mData ) {
				this->Clear ();
			}
			
			this->mSize = src.mSize;
			this->mData = src.mData;
			src.mSize = 0;
			src.mData = 0;
		}
	}

	//----------------------------------------------------------------//
	ZLLeanArray () :
		mSize ( 0 ),
		mData ( 0 ) {
	}

	//----------------------------------------------------------------//
	virtual ~ZLLeanArray () {
		this->Clear ();
	}
};

#endif
