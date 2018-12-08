// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSEMANTICNUMERIC_H
#define	ZLSEMANTICNUMERIC_H

template < typename TYPE > class ZLSemanticNumericCast;

//================================================================//
// ZLSemanticNumeric
//================================================================//
template < typename TYPE >
class ZLSemanticNumeric {
private:

	friend class ZLSemanticNumericCast < TYPE >;

	static const TYPE INVALID_KEY = (( TYPE )-1 );

	TYPE mKey;

	//----------------------------------------------------------------//
	static ZLSemanticNumeric < TYPE > Make ( TYPE key ) {
		ZLSemanticNumeric < TYPE > index;
		index.mKey = key;
		return index;
	}

public:

	//----------------------------------------------------------------//
	operator TYPE () const {
		return this->mKey;
	}

	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator + ( ZLSize v ) const {
		return ZLSemanticNumeric < TYPE >::Make ( this->mKey + v );
	}
	
	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator + ( const ZLSemanticNumeric < TYPE >& v ) const {
		return ZLSemanticNumeric < TYPE >::Make ( this->mKey + v.mKey );
	}
	
	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator - ( ZLSize v ) const {
		return ZLSemanticNumeric < TYPE >::Make ( this->mKey - v );
	}
	
	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator - ( const ZLSemanticNumeric < TYPE >& v ) const {
		return ZLSemanticNumeric < TYPE >::Make ( this->mKey - v.mKey );
	}

	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator * ( ZLSize v ) const {
		return ZLSemanticNumeric < TYPE >::Make ( this->mKey * v );
	}

	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator * ( const ZLSemanticNumeric < TYPE >& v ) const {
		return ZLSemanticNumeric < TYPE >::Make ( this->mKey * v.mKey );
	}

	//----------------------------------------------------------------//
	bool operator == ( const ZLSemanticNumeric < TYPE >& v ) const {
		return ( this->mKey == v.mKey );
	}

	//----------------------------------------------------------------//
	bool operator != ( const ZLSemanticNumeric < TYPE >& v ) const {
		return ( this->mKey != v.mKey );
	}

	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE >& operator ++ () {
	  this->mKey += 1;
	  return *this;
	}

	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator ++ ( int ) {
	  ZLSemanticNumeric < TYPE > tmp ( *this );
	  operator ++ ();
	  return tmp;
	}

	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE >& operator -- () {
	  this->mKey -= 1;
	  return *this;
	}

	//----------------------------------------------------------------//
	ZLSemanticNumeric < TYPE > operator -- ( int ) {
	  ZLSemanticNumeric < TYPE > tmp ( *this );
	  operator -- ();
	  return tmp;
	}
	
	//----------------------------------------------------------------//
	ZLSemanticNumeric () :
		mKey ( INVALID_KEY ) {
	}
	
	//----------------------------------------------------------------//
	ZLSemanticNumeric ( const ZLSemanticNumeric < TYPE >& index ) :
		mKey ( index.mKey ) {
	}
};

//================================================================//
// ZLSemanticNumericCast
//================================================================//
template < typename TYPE >
class ZLSemanticNumericCast {
private:

	ZLSemanticNumeric < TYPE >	mIndex;

public:

	//----------------------------------------------------------------//
	operator ZLSemanticNumeric < TYPE >() const {
		return this->mIndex;
	}

	//----------------------------------------------------------------//
	ZLSemanticNumericCast ( TYPE key ) {
		this->mIndex.mKey = key;
	}
};

#endif
