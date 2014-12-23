// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHashedString.h>
#include <zl-util/STLMap.h>

//================================================================//
// String hashing
//================================================================//

//uint MOAIPathDictionary::GetHash ( const char path[], size_t* pathLength ) {
//
//	// Using djb2 (see http://www.cse.yorku.ca/~oz/hash.html)
//	
//	size_t i = 0;
//	int c;
//	unsigned long hash = 5381;
//	
//	for (; ( c = path [ i ]); ++i ) {
//		hash = (( hash << 5 ) + hash) + c; // hash * 33 + c
//	}
//	*pathLength = i;
//	return ( uint )hash;
//}

#define FNV_seed 2166136261u
#define FNV_prime 16777619u

//----------------------------------------------------------------//
static u32 HashString ( cc8* szKey, const u32 nLength, const u32 seed = FNV_seed ) {

    u32 hash = seed;

    for ( u32 i = 0; i < nLength; ++i ) {
        hash = ( hash ^ szKey [ i ]) * FNV_prime;
    }

    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
    return ( hash );
}

//================================================================//
// ZLHashedString::ZLHashedStringEntry
//================================================================//

//----------------------------------------------------------------//
ZLHashedString::ZLHashedStringEntry::ZLHashedStringEntry ( cc8* name, const u32 nameLength, const u32 hash ) :
	mName ( 0 ),
	mHash ( hash ) {

	mName = new char [ nameLength + 1 ];
	memcpy ( mName, name, nameLength );
	mName [ nameLength ] = 0;
}

//----------------------------------------------------------------//
ZLHashedString::ZLHashedStringEntry::~ZLHashedStringEntry () {

	delete [] mName;
	mName = 0;
}

//================================================================//
// ZLHashedString
//================================================================//

const ZLHashedString ZLHashedString::Empty ( 0 );

//----------------------------------------------------------------//
const ZLHashedString::ZLHashedStringEntry& ZLHashedString::GetHashedStringEntry ( cc8* name ) {

	static STLMap < u32, ZLHashedStringEntry* > gStringDictonary;

	size_t nameLength = name ? strlen ( name ) : 0;
	u32 nameHash = GetStringHash ( name, nameLength );

	ZLHashedStringEntry* entry = gStringDictonary.value_for_key ( nameHash );
	if ( entry == 0 ) {

		// Create a new hashed string entry
		entry = new ZLHashedStringEntry ( name, nameLength, nameHash );
		gStringDictonary [ nameHash ] = entry;
	}

	return *entry;
}

//----------------------------------------------------------------//
u32 ZLHashedString::GetStringHash ( cc8* str, u32 strLength ) {

	u32 hash = 0;

	if ( str != 0 && strLength > 0 ) {
		hash = HashString ( str, strLength );
	}

	return hash;
}

//----------------------------------------------------------------//
ZLHashedString::ZLHashedString () :
	mHashedString ( &GetHashedStringEntry ( 0 )) {
}

//----------------------------------------------------------------//
ZLHashedString::ZLHashedString ( cc8* name ) :
	mHashedString ( &GetHashedStringEntry ( name )) {
}

//----------------------------------------------------------------//
ZLHashedString::ZLHashedString ( const ZLHashedString& other ) : 
	mHashedString ( other.mHashedString ) {
}
