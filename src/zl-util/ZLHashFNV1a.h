// Copyright (c) 2017-2018 Cryptogogue, Inc. All Rights Reserved.
// http://cryptogogue.com

#ifndef ZLHASHFNV1A_H
#define ZLHASHFNV1A_H

//================================================================//
// ZLHashFNV1a
//================================================================//
class ZLHashFNV1a {
public:

	static const u32  FNV1A_VAL_32		= 0x811c9dc5;
	static const u32  FNV1A_PRIME_32	= 0x1000193;
	static const u64  FNV1A_VAL_64		= 0xcbf29ce484222325;
	static const u64  FNV1A_PRIME_64	= 0x100000001b3;

	//----------------------------------------------------------------//
	static inline u32 Hash32 ( cc8* str ) {

		u32 hash = FNV1A_VAL_32;
		for ( ; *str; ++str ) {
			hash = ( hash ^ ( uint32_t )*str ) * FNV1A_PRIME_32;
		}
		return hash;
	}

	//----------------------------------------------------------------//
	static inline u64 Hash64 ( cc8* str ) {

		u64 hash = FNV1A_VAL_64;
		for ( ; *str; ++str ) {
			hash = ( hash ^ ( uint64_t )*str ) * FNV1A_PRIME_64;
		}
		return hash;
	}
};

#endif
