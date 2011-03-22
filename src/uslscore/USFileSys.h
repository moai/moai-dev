// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef FILESYS_H
#define FILESYS_H

#include <uslscore/STLString.h>

//================================================================//
// USFileStat
//================================================================//
struct USFileStat {

	bool	mExists;
	bool	mIsFile;
	bool	mIsDir;
	
	u64		mTimeCreated;	// Time created (in seconds from cardinal date)
	u64		mTimeModified;	// Time last modified (in seconds from cardinal date)
	u64		mTimeViewed;	// Time last viewed (in seconds from cardinal date)
	u64		mSize;			// Yeah, that
};

//================================================================//
// USFileSys
//================================================================//
namespace USFileSys {

	//----------------------------------------------------------------//
	void		AffirmPath			( cc8* path );
	bool		CheckFileExists		( cc8* path );
	bool		CheckPathExists		( cc8* path );
	bool		DeleteDirectory		( cc8* path, bool force = false, bool recursive = false );
	bool		DeleteFile			( cc8* path );
	STLString	Expand				( cc8* path );
	STLString	ExpandPath			( cc8* path );
	STLString	GetCurrentPath		();
	bool		GetFileStat			( cc8* path, USFileStat& fileStat );
	STLString	GetRelativePath		( cc8* path );
	bool		Rename				( cc8* oldPath, cc8* newPath );
	bool		SetCurrentPath		( cc8* path );
}

#endif
