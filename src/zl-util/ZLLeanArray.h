// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANARRAY_H
#define	ZLLEANARRAY_H

#include <zl-util/ZLIndex.h>
#include <zl-util/ZLResult.h>

#define ZL_DECLARE_SCOPED_BUFFER(name,type,size,stackAllocMax)									\
	ZLLeanArray < u8 >			_##name##_array;												\
	type* name					= ( type* )( size <= stackAllocMax ? alloca ( size ) : 0 );		\
	if ( !name ) {																				\
		_##name##_array.Init ( size );															\
		name = ( type* )_##name##_array.GetBuffer ();											\
	}

//================================================================//
// ZLLeanArray
//================================================================//
template < typename TYPE >
class ZLLeanArray {
protected:

	ZLSize	mSize;
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
	inline TYPE& operator [] ( const ZLIndex& idx ) {
		
		assert ( idx < this->mSize );
		return this->mData [( ZLSize )idx ];
	}
	
	//----------------------------------------------------------------//
	inline const TYPE& operator [] ( const ZLIndex& idx ) const {
		
		assert ( idx < this->mSize );
		return this->mData [( ZLSize )idx ];
	}

	//----------------------------------------------------------------//
	ZLSize BufferSize () {
		return this->mSize * sizeof ( TYPE );
	}

	//----------------------------------------------------------------//
	inline bool CheckIndex ( ZLIndex index ) const {
		return ( index < this->mSize );
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

		ZLSize total = ( this->mSize < src.mSize ) ? this->mSize : src.mSize;

		for ( ZLSize i = 0; i < total; ++i ) {
			this->mData [ i ] = src.mData [ i ];
		}
	}

	//----------------------------------------------------------------//
	inline TYPE& Elem ( const ZLIndex& idx ) {
		return this->mData [( ZLSize )idx ];
	}

	//----------------------------------------------------------------//
	void Fill ( const TYPE& value ) {
		for ( ZLSize i = 0; i < this->mSize; ++i ) {
			this->mData [ i ] = value;
		}
	}

	//----------------------------------------------------------------//
	inline TYPE* GetBuffer () {
		return this->mData;
	}

	//----------------------------------------------------------------//
	inline const TYPE* GetBuffer () const {
		return this->mData;
	}

	//----------------------------------------------------------------//
	ZLResultCode Grow ( ZLSize size ) {
	
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
	ZLResultCode Init ( ZLSize size ) {

		return this->Resize ( size );
	}

	//----------------------------------------------------------------//
	bool IsIdentical ( const ZLLeanArray < TYPE >& array ) {
	
		if ( this->mSize == array.mSize ) {
			return ( memcmp ( this->mData, array.mData, this->mSize ) == 0 );
		}
		return false;
	}

	//----------------------------------------------------------------//
	void Nullify () {
	
		mData = 0;
		mSize = 0;
	}

	//----------------------------------------------------------------//
	virtual ZLResultCode Resize ( ZLSize size ) {

		if ( this->mSize != size ) {

			TYPE* data = 0;

			if ( size ) {
				
				data = new TYPE [ size ];
				if ( !data ) return ZL_ALLOCATION_ERROR;

				ZLSize total = ( this->mSize < size ) ? this->mSize : size;
				
				for ( ZLSize i = 0; i < total; ++i ) {
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
	ZLResultCode Resize ( ZLSize size, const TYPE& value ) {

		ZLSize oldSize = this->mSize;
		
		if ( this->Resize ( size ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
		
		if ( size >= oldSize ) {
			for ( ZLSize i = oldSize; i < size; ++i ) {
				this->mData [ i ] = value;
			}
		}
		return ZL_OK;
	}

	//----------------------------------------------------------------//
	void RotateLeft ( ZLSize spaces ) {
	
		spaces = spaces % this->mSize;
		if ( spaces ) {
			
			ZLSize size = this->mSize * sizeof ( TYPE );
			ZLSize leftSize = spaces * sizeof ( TYPE );
			ZLSize rightSize = size - leftSize;
			
			void* lower = ( void* )(( ZLSize )this->mData + leftSize );
			void* upper = ( void* )(( ZLSize )this->mData + rightSize );
			
			void* temp = alloca ( leftSize );
		
			memcpy ( temp, this->mData, leftSize );
			memmove ( this->mData, lower, rightSize );
			memcpy ( upper, temp, leftSize );
		}
	}
	
	//----------------------------------------------------------------//
	void RotateRight ( ZLSize spaces ) {
	
		spaces = spaces % this->mSize;
		if ( spaces ) {
			
			ZLSize size = this->mSize * sizeof ( TYPE );
			ZLSize leftSize = spaces * sizeof ( TYPE );
			ZLSize rightSize = size - leftSize;
			
			void* lower = ( void* )(( ZLSize )this->mData + leftSize );
			void* upper = ( void* )(( ZLSize )this->mData + rightSize );
			
			void* temp = alloca ( rightSize );
		
			memcpy ( temp, upper, rightSize );
			memmove ( lower, this->mData, leftSize );
			memcpy ( this->mData, temp, rightSize );
		}
	}

	//----------------------------------------------------------------//
	inline ZLSize Size () const {
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
