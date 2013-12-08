// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLHASHEDSTRING_H
#define	ZLHASHEDSTRING_H

#include <zl-util/ZLAccessors.h>

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
				ZLHashedStringEntry		( cc8* name, const u32 nameLength, const u32 hash );
				ZLHashedStringEntry		( const ZLHashedStringEntry& other );
				~ZLHashedStringEntry	();
	};
	
	const ZLHashedStringEntry* mHashedString;
	
	//----------------------------------------------------------------//
	static const ZLHashedStringEntry&	GetHashedStringEntry	( cc8* name );

public:

	static const ZLHashedString	Empty;

	GET_CONST ( cc8*, String, mHashedString->mName )
	GET_CONST ( u32, Hash, mHashedString->mHash )

	//----------------------------------------------------------------//
	static u32		GetStringHash		( cc8* str, u32 strLength );
					ZLHashedString		();
					ZLHashedString		( cc8* name );
					ZLHashedString		( const ZLHashedString& other );
	
	//----------------------------------------------------------------//
	bool operator == ( const ZLHashedString& rhs ) const {
		return GetHash() == rhs.GetHash();
	}
	
	//----------------------------------------------------------------//
	bool operator != ( const ZLHashedString& rhs ) const {
		return GetHash() != rhs.GetHash();
	}
};

#endif
