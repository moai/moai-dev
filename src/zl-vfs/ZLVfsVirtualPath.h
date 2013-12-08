// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLVIRTUALPATH_H
#define ZLVIRTUALPATH_H

#include <zl-vfs/ZLVfsZipArchive.h>

//================================================================//
// ZLVfsVirtualPath
//================================================================//
class ZLVfsVirtualPath {
public:

	std::string			mPath;
	std::string			mName;
	ZLVfsZipArchive*		mArchive;

	ZLVfsVirtualPath*		mNext;

public:

	//----------------------------------------------------------------//
	const char*			GetLocalPath		(const char* path );
	ZLVfsVirtualPath*		PushFront			( ZLVfsVirtualPath* list );
	int					SetArchive			( const char* archive );
	int					SetPath				( const char* path );
						ZLVfsVirtualPath		();
						~ZLVfsVirtualPath		();
};

#endif