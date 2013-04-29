// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLDIRECTORYITR_H
#define ZLDIRECTORYITR_H

#ifdef _WIN32
	#include <direct.h>
	#include <io.h>
#else
	#include <sys/types.h>
	#include <dirent.h>
	#include <unistd.h>
#endif

class ZLVfsZipFileDir;
class ZLVfsZipFileEntry;
class ZLVfsVirtualPath;

//================================================================//
// ZLVfsDirectoryItr
//================================================================//
class ZLVfsDirectoryItr {
private:

	std::string			mDirName;

	ZLVfsZipFileDir*		mZipFileDir; // this is a ref to the directory being iterated
	
	ZLVfsZipFileDir*		mZipFileSubDir; // this is the sub directory iterator
	ZLVfsZipFileEntry*		mZipFileEntry; // this is the file entry iterator
	ZLVfsVirtualPath*		mVirtualSubDir;

	std::string			mName;
	bool				mIsDir;

	#ifdef _WIN32
		intptr_t		mHandle;
	#else
		DIR*			mHandle;
	#endif

	//----------------------------------------------------------------//
	int				ReadZipEntry		();

public:

	//----------------------------------------------------------------//
	void			Close				();
	char const*		GetEntryName		();
	bool			IsSubdir			();
	int				Open				();
	int				ReadEntry			();
					ZLVfsDirectoryItr		();
					~ZLVfsDirectoryItr		();
};

#endif
