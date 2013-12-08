// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBINARYSEARCH_H
#define	ZLBINARYSEARCH_H

//================================================================//
// ZLBinarySearch
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
u32 ZLBinarySearch ( const TYPE* buffer, const TYPE& key, u32 total ) {

	static const u32 NO_MATCH = 0xffffffff;

	u32 i = 0;
	u32 j = total - 1;
	u32 s = j;
	
	if ( key < buffer [ i ]) return NO_MATCH;
	if ( buffer [ j ] < key ) return NO_MATCH;
	
	while ( s ) {
		
		u32 c = i + ( s >> 1 );
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
	
	if ( !(( key < buffer [ i ]) || ( buffer [ i ] < key )))  return i;
	
	return NO_MATCH;
}

//----------------------------------------------------------------//
template < typename TYPE >
u32 USBinarySearchNearest ( const TYPE* buffer, const TYPE& key, u32 total ) {
	
	static const u32 NO_MATCH = 0xffffffff;
	
	u32 i = 0;
	u32 j = total - 1;
	u32 s = j;
	
	if ( key < buffer [ i ]) return NO_MATCH;
	if ( buffer [ j ] < key ) return NO_MATCH;
	
	while ( s > 1 ) {
		
		u32 c = i + ( s >> 1 );
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
	
	if ( key < buffer [ j ] ) {
		return i;
	}
	else {
		return j;
	}
}

#endif
