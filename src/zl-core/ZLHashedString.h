// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLHASHEDSTRING_H
#define	ZLHASHEDSTRING_H

#include <zl-core/ZLAccessors.h>

#define DECLARE_NAME( name ) static ZLHashedString name ( #name )

//================================================================//
// ZLHashedString
//================================================================//
class ZLHashedString {
private:
	
	//================================================================//
	// ZLHashedStringEntry
	//================================================================//
	struct ZLHashedStringEntry {
		
		char*	mName;
		u32		mHash;
		
		//----------------------------------------------------------------//
				ZLHashedStringEntry		( cc8* name, size_t nameLength, const u32 hash );
				ZLHashedStringEntry		( const ZLHashedStringEntry& other );
				~ZLHashedStringEntry	();
	};
	
	const ZLHashedStringEntry* mHashedString;
	
	//----------------------------------------------------------------//
	static const ZLHashedStringEntry&	GetHashedStringEntry	( cc8* name );

public:

	static const u32 FNV_seed = 2166136261u;
	static const u32 FNV_prime = 16777619u;

	static const ZLHashedString	Empty;

	GET_CONST ( cc8*, String, mHashedString->mName )
	GET_CONST ( u32, Hash, mHashedString->mHash )

	//----------------------------------------------------------------//
	static u32		GetStringHash		( cc8* str, size_t strLength );
	static u32		Hash				( cc8* szKey, size_t nLength, const u32 seed = FNV_seed );
					ZLHashedString		();
					ZLHashedString		( cc8* name );
					ZLHashedString		( const ZLHashedString& other );
	
	//----------------------------------------------------------------//
	bool operator == ( const ZLHashedString& rhs ) const {
		return this->GetHash () == rhs.GetHash ();
	}
	
	//----------------------------------------------------------------//
	bool operator != ( const ZLHashedString& rhs ) const {
		return this->GetHash () != rhs.GetHash ();
	}
};

#endif
