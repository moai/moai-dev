// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	LEANARRAY_H
#define	LEANARRAY_H

//================================================================//
// USLeanArray
//================================================================//
template < typename TYPE >
class USLeanArray {
protected:

	u32		mSize;
	TYPE*	mData;

	//----------------------------------------------------------------//
	virtual void Alloc ( u32 size ) {

		this->mData = new TYPE [ size ];
	}

	//----------------------------------------------------------------//
	virtual void Free () {

		delete [] this->mData;
	}

public:

	//----------------------------------------------------------------//
	USLeanArray < TYPE >& operator = ( const USLeanArray < TYPE >& assign ) {
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
	virtual void Clear () {

		if ( this->mSize && this->mData ) {
			this->Free ();
		}

		this->mData = 0;
		this->mSize = 0;
	}

	//----------------------------------------------------------------//
	void CloneFrom ( const USLeanArray < TYPE >& src ) {

		this->Init ( src.mSize );
		this->CopyFrom ( src );
	}

	//----------------------------------------------------------------//
	void CopyFrom ( const USLeanArray < TYPE >& src ) {

		u32 total = ( this->mSize < src.mSize ) ? this->mSize : src.mSize;

		for ( u32 i = 0; i < total; ++i ) {
			this->mData [ i ] = src [ i ];
		}
	}

	//----------------------------------------------------------------//
	inline TYPE& Elem ( u32 idx ) {
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
		for ( u32 i = 0; i < this->mSize; ++i ) {
			this->mData [ i ] = value;
		}
	}

	//----------------------------------------------------------------//
	void Grow ( u32 size ) {
	
		if ( size > this->mSize ) {
			this->Resize ( size );
		}
	}
	
	//----------------------------------------------------------------//
	void Grow ( u32 size, u32 chunkSize ) {
		
		u32 chunks = ( size / chunkSize ) + 1;
		this->Grow ( chunks * chunkSize );
	}

	//----------------------------------------------------------------//
	void Grow ( u32 size, u32 chunkSize, const TYPE& value ) {
		
		u32 chunks = ( size / chunkSize ) + 1;
		u32 newSize = chunks * chunkSize;
		
		if ( newSize > this->mSize ) {
			this->Resize ( newSize, value );
		}
	}

	//----------------------------------------------------------------//
	bool Init ( u32 size ) {

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
	void Resize ( u32 size ) {

		u32 oldSize = this->mSize;
		TYPE* oldArray = this->mData;

		this->mSize = 0;
		this->mData = 0;

		if ( size ) {
			this->Alloc ( size );
			this->mSize = size;
			
			for ( u32 i = 0; (( i < size ) && ( i < oldSize )); ++i ) {
				this->mData [ i ] = oldArray [ i ];
			}
		}
		
		if ( oldArray ) {
			delete [] oldArray;
		}
	}

	//----------------------------------------------------------------//
	void Resize ( u32 size, const TYPE& value ) {

		u32 oldSize = this->mSize;
		
		this->Resize ( size );
		
		if ( size >= oldSize ) {
			for ( u32 i = oldSize; i < size; ++i ) {
				this->mData [ i ] = value;
			}
		}
	}

	//----------------------------------------------------------------//
	inline u32 Size () const {
		return this->mSize;
	}

	//----------------------------------------------------------------//
	USLeanArray () :
		mSize ( 0 ),
		mData ( 0 ) {
	}

	//----------------------------------------------------------------//
	virtual ~USLeanArray () {
		this->Clear ();
	}
};

#endif
