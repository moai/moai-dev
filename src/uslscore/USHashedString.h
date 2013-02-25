// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USHASHEDSTRING_H
#define	USHASHEDSTRING_H

#include <uslscore/USAccessors.h>

#define DECLARE_NAME( name ) static USHashedString name ( #name )

//================================================================//
// USHashedString
//================================================================//
class USHashedString {
private:
	
	//================================================================//
	// USHashedStringEntry
	//================================================================//
	struct USHashedStringEntry {
		
		char*	mName;
		u32		mHash;
		
		//----------------------------------------------------------------//
				USHashedStringEntry		( cc8* name, const u32 nameLength, const u32 hash );
				USHashedStringEntry		( const USHashedStringEntry& other );
				~USHashedStringEntry	();
	};
	
	const USHashedStringEntry* mHashedString;
	
	//----------------------------------------------------------------//
	static const USHashedStringEntry&	GetHashedStringEntry	( cc8* name );

public:

	static const USHashedString	Empty;

	GET_CONST ( cc8*, String, mHashedString->mName )
	GET_CONST ( u32, Hash, mHashedString->mHash )

	//----------------------------------------------------------------//
	static u32		GetStringHash		( cc8* str, u32 strLength );
					USHashedString		();
					USHashedString		( cc8* name );
					USHashedString		( const USHashedString& other );
	
	//----------------------------------------------------------------//
	bool operator == ( const USHashedString& rhs ) const {
		return GetHash() == rhs.GetHash();
	}
	
	//----------------------------------------------------------------//
	bool operator != ( const USHashedString& rhs ) const {
		return GetHash() != rhs.GetHash();
	}
};

#endif
