// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLVIRTUALPATH_H
#define ZLVIRTUALPATH_H

#include <zlcore/ZLZipArchive.h>

//================================================================//
// ZLVirtualPath
//================================================================//
class ZLVirtualPath {
public:

	std::string			mPath;
	std::string			mName;
	ZLZipArchive*		mArchive;

	ZLVirtualPath*		mNext;

public:

	//----------------------------------------------------------------//
	const char*			GetLocalPath		(const char* path );
	ZLVirtualPath*		PushFront			( ZLVirtualPath* list );
	int					SetArchive			( const char* archive );
	int					SetPath				( const char* path );
						ZLVirtualPath		();
						~ZLVirtualPath		();
};

#endif