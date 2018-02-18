// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINDEX_H
#define	ZLINDEX_H

//================================================================//
// ZLMetaIndex
//================================================================//
template < typename TYPE >
class ZLMetaIndex {
	public:

	static const ZLMetaIndex < TYPE >	INVALID;
	static const ZLMetaIndex < TYPE >	ZERO;

	static const TYPE INVALID_KEY	= (( uint )-1 );

	TYPE		mKey;

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator + ( const ZLMetaIndex < TYPE >& v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey + v.mKey );
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator - ( const ZLMetaIndex < TYPE >& v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey - v.mKey );
	}

	//----------------------------------------------------------------//
	void Add ( TYPE amount, TYPE size ) {
	
		this->mKey = ( this->mKey + amount ) % size;
	}

	//----------------------------------------------------------------//
	void Sub ( TYPE amount, TYPE size ) {
	
		this->mKey = ( this->mKey - amount ) % size;
	}
	
	//----------------------------------------------------------------//
	void Wrap ( TYPE size ) {
	
		this->mKey = this->mKey % size;
	}

	//----------------------------------------------------------------//
	ZLMetaIndex () :
		mKey ( INVALID_KEY ) {
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex ( TYPE key ) :
		mKey ( key ) {
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex ( const ZLMetaIndex < TYPE >& index ) :
		mKey ( index.mKey ) {
	}
};

template < typename TYPE > const ZLMetaIndex < TYPE > ZLMetaIndex < TYPE >::INVALID;
template < typename TYPE > const ZLMetaIndex < TYPE > ZLMetaIndex < TYPE >::ZERO ( 0 );

typedef ZLMetaIndex < ZLSize > ZLIndex;

#endif
