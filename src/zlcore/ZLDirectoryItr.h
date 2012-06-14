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

class ZLZipFileDir;
class ZLZipFileEntry;
class ZLVirtualPath;

//================================================================//
// ZLDirectoryItr
//================================================================//
class ZLDirectoryItr {
private:

	std::string			mDirName;

	ZLZipFileDir*		mZipFileDir; // this is a ref to the directory being iterated
	
	ZLZipFileDir*		mZipFileSubDir; // this is the sub directory iterator
	ZLZipFileEntry*		mZipFileEntry; // this is the file entry iterator
	ZLVirtualPath*		mVirtualSubDir;

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
					ZLDirectoryItr		();
					~ZLDirectoryItr		();
};

#endif
