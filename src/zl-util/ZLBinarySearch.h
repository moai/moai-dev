// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBINARYSEARCH_H
#define	ZLBINARYSEARCH_H

//================================================================//
// ZLBinarySearch
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
ZLSize ZLBinarySearch ( const TYPE* buffer, const TYPE& key, ZLSize total ) {

	ZLSize i = 0;
	ZLSize j = total - 1;
	ZLSize s = j;
	
	if ( key < buffer [ i ]) return ZLIndexOp::INVALID_KEY;
	if ( buffer [ j ] < key ) return ZLIndexOp::INVALID_KEY;
	
	while ( s ) {
		
		ZLSize c = i + ( s >> 1 );
		if ( i == c ) c = j;
		const TYPE& test = buffer [ c ];
		
		if ( test < key ) {
			i = c + 1;
		}
		else if ( key < test ) {
			j = c - 1;
		}
		else {
			return c;
		}
		s = j - i;
	}
	
	return ( !(( key < buffer [ i ]) || ( buffer [ i ] < key ))) ? i : ZLIndexOp::INVALID_KEY;
}

//----------------------------------------------------------------//
template < typename TYPE >
ZLSize ZLBinarySearchNearest ( const TYPE* buffer, const TYPE& key, ZLSize total ) {
	
	ZLSize i = 0;
	ZLSize j = total - 1;
	ZLSize s = j;
	
	if ( key < buffer [ i ]) return ZLIndexOp::INVALID_KEY;
	if ( buffer [ j ] < key ) return ZLIndexOp::INVALID_KEY;
	
	while ( s > 1 ) {
		
		ZLSize c = i + ( s >> 1 );
		const TYPE& test = buffer [ c ];
		
		if ( test < key ) {
			i = c;
		}
		else if ( key < test ) {
			j = c;
		}
		else {
			return c;
		}
		s = j - i;
	}
	
	return ( key < buffer [ j ]) ? i : j;
}

#endif
