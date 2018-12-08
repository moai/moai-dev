// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLINDEX_H
#define	ZLINDEX_H

#include <zl-util/ZLSemanticNumeric.h>

#define MOAI_USE_STRICT_SEMANTIC_NUMERICS

#ifdef MOAI_USE_STRICT_SEMANTIC_NUMERICS

	typedef ZLSemanticNumeric < ZLSize > ZLIndex;
	typedef ZLSemanticNumericCast < ZLSize > ZLIndexCast;

#else

	typedef ZLSize ZLIndex;
	typedef ZLSize ZLIndexCast;
#endif

//================================================================//
// ZLMetaIndexOp
//================================================================//
template < typename TYPE, typename INDEX_TYPE, typename INDEX_CAST >
class ZLMetaIndexOp {
public:

	static const INDEX_TYPE		INVALID;
	static const INDEX_TYPE		ONE;
	static const INDEX_TYPE		ZERO;

	static const TYPE INVALID_KEY	= ( TYPE )-1;

	//----------------------------------------------------------------//
	static INDEX_TYPE AddAndWrap ( TYPE index, TYPE amount, TYPE size ) {
		return INDEX_CAST (( index + amount ) % size );
	}

	//----------------------------------------------------------------//
	static int ToInt ( INDEX_TYPE& index ) {
		return ( int )(( TYPE )index );
	}

	//----------------------------------------------------------------//
	static INDEX_TYPE SubtractAndWrap ( TYPE index, TYPE amount, TYPE size ) {
		return INDEX_CAST ( index < amount ? ( size - ( amount - index )) : ( index - amount ));
	}
	
	//----------------------------------------------------------------//
	static INDEX_TYPE Wrap ( TYPE index, TYPE size ) {
		return INDEX_CAST ( index % size );
	}
};

template < typename TYPE, typename INDEX_TYPE, typename INDEX_CAST > const INDEX_TYPE ZLMetaIndexOp < TYPE, INDEX_TYPE, INDEX_CAST >::INVALID	= INDEX_CAST (( TYPE )-1 );
template < typename TYPE, typename INDEX_TYPE, typename INDEX_CAST > const INDEX_TYPE ZLMetaIndexOp < TYPE, INDEX_TYPE, INDEX_CAST >::ONE		= INDEX_CAST ( 1 );
template < typename TYPE, typename INDEX_TYPE, typename INDEX_CAST > const INDEX_TYPE ZLMetaIndexOp < TYPE, INDEX_TYPE, INDEX_CAST >::ZERO		= INDEX_CAST ( 0 );

typedef ZLMetaIndexOp < ZLSize, ZLIndex, ZLIndexCast > ZLIndexOp;

#endif
