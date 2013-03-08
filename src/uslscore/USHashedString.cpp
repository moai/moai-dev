// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHashedString.h>
#include <uslscore/STLMap.h>

//================================================================//
// String hashing
//================================================================//

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
// USHashedString::USHashedStringEntry
//================================================================//

//----------------------------------------------------------------//
USHashedString::USHashedStringEntry::USHashedStringEntry ( cc8* name, const u32 nameLength, const u32 hash ) :
	mName ( 0 ),
	mHash ( hash ) {

	mName = new char [ nameLength + 1 ];
	memcpy ( mName, name, nameLength );
	mName [ nameLength ] = 0;
}

//----------------------------------------------------------------//
USHashedString::USHashedStringEntry::~USHashedStringEntry () {

	delete [] mName;
	mName = 0;
}

//================================================================//
// USHashedString
//================================================================//

const USHashedString USHashedString::Empty ( 0 );

//----------------------------------------------------------------//
const USHashedString::USHashedStringEntry& USHashedString::GetHashedStringEntry ( cc8* name ) {

	static STLMap < u32, USHashedStringEntry* > gStringDictonary;

	size_t nameLength = name ? strlen ( name ) : 0;
	u32 nameHash = GetStringHash ( name, nameLength );

	USHashedStringEntry* entry = gStringDictonary.value_for_key ( nameHash );
	if ( entry == 0 ) {

		// Create a new hashed string entry
		entry = new USHashedStringEntry ( name, nameLength, nameHash );
		gStringDictonary [ nameHash ] = entry;
	}

	return *entry;
}

//----------------------------------------------------------------//
u32 USHashedString::GetStringHash ( cc8* str, u32 strLength ) {

	u32 hash = 0;

	if ( str != 0 && strLength > 0 ) {
		hash = HashString ( str, strLength );
	}

	return hash;
}

//----------------------------------------------------------------//
USHashedString::USHashedString () :
	mHashedString ( &GetHashedStringEntry ( 0 )) {
}

//----------------------------------------------------------------//
USHashedString::USHashedString ( cc8* name ) :
	mHashedString ( &GetHashedStringEntry ( name )) {
}

//----------------------------------------------------------------//
USHashedString::USHashedString ( const USHashedString& other ) : 
	mHashedString ( other.mHashedString ) {
}
