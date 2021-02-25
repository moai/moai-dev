// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINDEXOP_H
#define	ZLINDEXOP_H

//================================================================//
// ZLMetaIndexOp
//================================================================//
template < typename TYPE >
class ZLMetaIndexOp {
public:

	static const TYPE INVALID	= ( TYPE )-1;

	//----------------------------------------------------------------//
	static TYPE AddAndWrap ( TYPE index, TYPE amount, TYPE size ) {
		return (( index + amount ) % size );
	}

	//----------------------------------------------------------------//
	static TYPE SubtractAndWrap ( TYPE index, TYPE amount, TYPE size ) {
		return index < amount ? ( size - ( amount - index )) : ( index - amount );
	}
	
	//----------------------------------------------------------------//
	static TYPE Wrap ( TYPE index, TYPE size ) {
		return ( index % size );
	}
};

typedef ZLMetaIndexOp < ZLIndex > ZLIndexOp;

#endif
