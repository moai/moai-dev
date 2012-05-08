// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFSVIRTUALPATH_H
#define ZIPFSVIRTUALPATH_H

#include <zipfs/ZIPFSZipArchive.h>

//================================================================//
// ZIPFSVirtualPath
//================================================================//
class ZIPFSVirtualPath {
public:

	std::string			mPath;
	std::string			mName;
	ZIPFSZipArchive*	mArchive;

	ZIPFSVirtualPath*	mNext;

public:

	//----------------------------------------------------------------//
	const char*			GetLocalPath		(const char* path );
	ZIPFSVirtualPath*	PushFront			( ZIPFSVirtualPath* list );
	int					SetArchive			( const char* archive );
	int					SetPath				( const char* path );
						ZIPFSVirtualPath	();
						~ZIPFSVirtualPath	();
};

#endif