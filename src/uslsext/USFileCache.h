// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USFILECACHE_H
#define USFILECACHE_H

#include <uslsext/USSqlConnection.h>

#define KBYTE 0x00000400
#define MBYTE 0x00100000

class USDataStoreEntry;

//================================================================//
// USFileCache
//================================================================//
class USFileCache {
protected:

	USSqlConnection mConnection;
	STLString mCacheFilename;
	STLString mCacheFolderPath;

	//-------------------------------------------------------------------//
	u32			Alloc				( int priority, u32 size );
	void		Create				();
	void		DeleteEntry			( USDataStoreEntry& entry );
	void		DeleteTouched		();

public:

	enum {
		OK = 0,
		ENRTY_UNKNOWN,
		PARTITION_UNKNOWN,
		PARTITION_FULL,
		MISSING_PARAM,
	};

	GET ( STLString, CacheFilename, mCacheFilename )
	GET ( STLString, CacheFolderPath, mCacheFolderPath )

	//-------------------------------------------------------------------//
	u32			AffirmFile					( cc8* name, const void* data, u32 size, int priority );
	u32			AffirmFileEntry				( cc8* name, u32 size, int priority );
	u32			AffirmPartition				( int priority, u32 maxSize );
	u32			AffirmRef					( cc8* refKey, cc8* storeKey );
	u32			BlessFile					( cc8* name, int priority );
	void		Close						();
	void		CurseFile					( cc8* filename );
	void		Delete						();
	void		DeleteNonReferencedEntries	();
	bool		GetFile						( cc8* name, USFileStream& stream );
	STLString	GetFilePath					( cc8* filename );
	STLString	GetFileName					( cc8* name );
	float			GetPartitionMaxSize			( int priority );
	float			GetNonReferencedSize		();
	float			GetSizeUsedFor				( cc8* refKey );
	bool		IsConnected					();
	bool		Open						();
	void		ReleaseRef					( cc8* refKey );
	void		SetCacheFilename			( cc8* filename );
				USFileCache					();
				~USFileCache				();
};

#endif
