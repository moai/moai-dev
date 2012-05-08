// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFSDIRECTORYITR_H
#define ZIPFSDIRECTORYITR_H

#ifdef _WIN32
	#include <direct.h>
	#include <io.h>
#else
	#include <sys/types.h>
	#include <dirent.h>
	#include <unistd.h>
#endif

class ZIPFSZipFileDir;
class ZIPFSZipFileEntry;
class ZIPFSVirtualPath;

//================================================================//
// ZIPFSDirectoryItr
//================================================================//
class ZIPFSDirectoryItr {
private:

	std::string				mDirName;

	ZIPFSZipFileDir*		mZipFileDir; // this is a ref to the directory being iterated
	
	ZIPFSZipFileDir*		mZipFileSubDir; // this is the sub directory iterator
	ZIPFSZipFileEntry*		mZipFileEntry; // this is the file entry iterator
	ZIPFSVirtualPath*		mVirtualSubDir;

	char const*				mName;
	bool					mIsDir;

	#ifdef _WIN32
		intptr_t			mHandle;
	#else
		DIR*				mHandle;
	#endif

	//----------------------------------------------------------------//
	int				ReadZipEntry	();

public:

	//----------------------------------------------------------------//
	void			Close					();
	char const*		GetEntryName			();
	bool			IsSubdir				();
	int				Open					();
	int				ReadEntry				();
					ZIPFSDirectoryItr		();
					~ZIPFSDirectoryItr		();
};

#endif
