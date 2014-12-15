// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef STLSORTPAIR_H
#define	STLSORTPAIR_H

using namespace std;

//================================================================//
// STLSortPair
//================================================================//
template < typename KEY_TYPE, typename VAL_TYPE >
class STLSortPair {
public:

	KEY_TYPE	mKey;
	VAL_TYPE	mValue;

	//----------------------------------------------------------------//
	inline bool operator> ( const STLSortPair < KEY_TYPE, VAL_TYPE >& rhs ) const {
		return greater ( *this, rhs );
	}

	//----------------------------------------------------------------//
	inline bool operator< ( const STLSortPair < KEY_TYPE, VAL_TYPE >& rhs ) const {
		return lesser ( *this, rhs );
	}
	
	//----------------------------------------------------------------//
	inline static bool greater ( const STLSortPair < KEY_TYPE, VAL_TYPE >& lhs, const STLSortPair < KEY_TYPE, VAL_TYPE >& rhs ) {
		return lhs.mKey > rhs.mKey;
	}
	
	//----------------------------------------------------------------//
	inline static bool lesser ( const STLSortPair < KEY_TYPE, VAL_TYPE >& lhs, const STLSortPair < KEY_TYPE, VAL_TYPE >& rhs ) {
		return lhs.mKey < rhs.mKey;
	}
};

#endif
