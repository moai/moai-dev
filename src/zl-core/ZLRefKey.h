// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREFKEY_H
#define ZLREFKEY_H

//================================================================//
// ZLRefKey
//================================================================//
template < typename TYPE >
class ZLRefKey {
public:

	const TYPE& mSignature;

	//----------------------------------------------------------------//
	bool operator < ( const ZLRefKey& other ) const {
	
		if ( &this->mSignature == &other.mSignature ) return false;
	
		return ( memcmp ( &this->mSignature, &other.mSignature, sizeof ( TYPE )) < 0 );
	}

	//----------------------------------------------------------------//
	ZLRefKey ( const TYPE& signature ) :
		mSignature ( signature ) {
	}
};

#endif
