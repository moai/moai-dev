// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINDEX_H
#define	ZLINDEX_H

template < typename TYPE > class ZLLeanArray;

//================================================================//
// ZLMetaIndex
//================================================================//
template < typename TYPE >
class ZLMetaIndex {
public:

	static const ZLMetaIndex < TYPE >	INVALID;
	static const ZLMetaIndex < TYPE >	ONE;
	static const ZLMetaIndex < TYPE >	ZERO;

	static const TYPE INVALID_KEY	= (( uint )-1 );
	static const TYPE LIMIT			= (( uint )-1 );

	TYPE		mKey;
	TYPE		mLimit;

	//----------------------------------------------------------------//
	operator TYPE () const {
		return this->mKey;
	}

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator + ( ZLSize v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey + v, LIMIT );
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator + ( const ZLMetaIndex < TYPE >& v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey + v.mKey, LIMIT );
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator - ( ZLSize v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey - v, LIMIT );
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator - ( const ZLMetaIndex < TYPE >& v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey - v.mKey, LIMIT );
	}

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator * ( ZLSize v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey * v, LIMIT );
	}

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator * ( const ZLMetaIndex < TYPE >& v ) const {
		return ZLMetaIndex < TYPE > ( this->mKey * v.mKey, LIMIT );
	}

	//----------------------------------------------------------------//
	bool operator == ( const ZLMetaIndex < TYPE >& v ) const {
		return ( this->mKey == v.mKey );
	}

	//----------------------------------------------------------------//
	bool operator != ( const ZLMetaIndex < TYPE >& v ) const {
		return ( this->mKey != v.mKey );
	}

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE >& operator ++ () {
	  this->mKey += 1;
	  return *this;
	}

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator ++ ( int ) {
	  ZLMetaIndex < TYPE > tmp ( *this );
	  operator ++ ();
	  return tmp;
	}

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE >& operator -- () {
	  this->mKey -= 1;
	  return *this;
	}

	//----------------------------------------------------------------//
	ZLMetaIndex < TYPE > operator -- ( int ) {
	  ZLMetaIndex < TYPE > tmp ( *this );
	  operator -- ();
	  return tmp;
	}

	//----------------------------------------------------------------//
	static ZLMetaIndex < TYPE > AddAndWrap ( const ZLMetaIndex < TYPE >& v, TYPE amount, TYPE size ) {
		return ZLMetaIndex < TYPE >(( v.mKey + amount ) % size, LIMIT );
	}

	//----------------------------------------------------------------//
	int ToInt () {
		return ( int )this->mKey;
	}

	//----------------------------------------------------------------//
	static ZLMetaIndex < TYPE > SubtractAndWrap ( const ZLMetaIndex < TYPE >& v, TYPE amount, TYPE size ) {
	
		ZLSize w = v.mKey < amount ? ( size - ( amount - v.mKey )) : ( v.mKey - amount );
		return ZLMetaIndex < TYPE >( w, LIMIT );
	}
	
	//----------------------------------------------------------------//
	static ZLMetaIndex < TYPE > Wrap ( const ZLMetaIndex < TYPE >& v, TYPE size ) {
		return ZLMetaIndex < TYPE >( v.mKey % size, LIMIT );
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex () :
		mKey ( INVALID_KEY ) {
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex ( TYPE key, TYPE limit ) :
		mKey ( key ),
		mLimit ( limit ) {
	}
	
	//----------------------------------------------------------------//
	ZLMetaIndex ( const ZLMetaIndex < TYPE >& index ) :
		mKey ( index.mKey ) {
	}
};

template < typename TYPE > const ZLMetaIndex < TYPE > ZLMetaIndex < TYPE >::INVALID;
template < typename TYPE > const ZLMetaIndex < TYPE > ZLMetaIndex < TYPE >::ONE ( 1, ZLMetaIndex < TYPE >::LIMIT );
template < typename TYPE > const ZLMetaIndex < TYPE > ZLMetaIndex < TYPE >::ZERO ( 0, ZLMetaIndex < TYPE >::LIMIT );

typedef ZLMetaIndex < ZLSize > ZLIndex;

#endif
