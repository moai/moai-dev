// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLARRAYKEY_H
#define ZLARRAYKEY_H

//================================================================//
// ZLArrayKey
//================================================================//
template < typename TYPE >
class ZLArrayKey {
public:

	const ZLLeanArray < TYPE >&		mSignature;

	//----------------------------------------------------------------//
	bool operator < ( const ZLArrayKey& other ) const {
	
		ZLSize sigSize = this->mSignature.BufferSize ();
		ZLSize otherSigSize = other.mSignature.BufferSize ();
		if ( sigSize == otherSigSize ) {
			return ( memcmp ( this->mSignature.GetBuffer (), other.mSignature.GetBuffer (), sigSize ) < 0 );
		}
		return ( sigSize < otherSigSize ); // this should never happen, but handle it anyway
	}

	//----------------------------------------------------------------//
	ZLArrayKey ( const ZLLeanArray < TYPE >& signature ) :
		mSignature ( signature ) {
	}
};

#endif
