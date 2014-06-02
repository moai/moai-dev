// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLEANARRAY_H
#define	ZLLEANARRAY_H

//================================================================//
// ZLLeanArray
//================================================================//
template < typename TYPE >
class ZLLeanArray {
protected:

	size_t	mSize;
	TYPE*	mData;

	//----------------------------------------------------------------//
	virtual void Alloc ( size_t size ) {

		this->mData = new TYPE [ size ];
	}

	//----------------------------------------------------------------//
	virtual void Free () {

		delete [] this->mData;
	}

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
	virtual void Clear () {

		if ( this->mSize && this->mData ) {
			this->Free ();
		}

		this->mData = 0;
		this->mSize = 0;
	}

	//----------------------------------------------------------------//
	void CloneFrom ( const ZLLeanArray < TYPE >& src ) {

		this->Init ( src.mSize );
		this->CopyFrom ( src );
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
	void Grow ( size_t size ) {
	
		if ( size > this->mSize ) {
			this->Resize ( size );
		}
	}
	
	//----------------------------------------------------------------//
	void Grow ( size_t size, size_t chunkSize ) {
		
		size_t chunks = ( size / chunkSize ) + 1;
		this->Grow ( chunks * chunkSize );
	}

	//----------------------------------------------------------------//
	void Grow ( size_t size, size_t chunkSize, const TYPE& value ) {
		
		size_t chunks = ( size / chunkSize ) + 1;
		size_t newSize = chunks * chunkSize;
		
		if ( newSize > this->mSize ) {
			this->Resize ( newSize, value );
		}
	}

	//----------------------------------------------------------------//
	bool Init ( size_t size ) {

		this->Clear ();

		if ( !size ) return true;

		this->Alloc ( size );
		if ( !this->mData ) return false;

		this->mSize = size;
		return true;
	}

	//----------------------------------------------------------------//
	void Nullify () {
		mData = 0;
		mSize = 0;
	}

	//----------------------------------------------------------------//
	void Resize ( size_t size ) {

		size_t oldSize = this->mSize;
		TYPE* oldArray = this->mData;

		this->mSize = 0;
		this->mData = 0;

		if ( size ) {
			this->Alloc ( size );
			this->mSize = size;
			
			for ( size_t i = 0; (( i < size ) && ( i < oldSize )); ++i ) {
				this->mData [ i ] = oldArray [ i ];
			}
		}
		
		if ( oldArray ) {
			delete [] oldArray;
		}
	}

	//----------------------------------------------------------------//
	void Resize ( size_t size, const TYPE& value ) {

		size_t oldSize = this->mSize;
		
		this->Resize ( size );
		
		if ( size >= oldSize ) {
			for ( size_t i = oldSize; i < size; ++i ) {
				this->mData [ i ] = value;
			}
		}
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
