// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLFILESYS_H
#define ZLFILESYS_H

#include <zl-util/STLString.h>

//================================================================//
// ZLFileSys
//================================================================//
class ZLFileSys {
public:

	//----------------------------------------------------------------//
	static bool				AffirmPath				( cc8* path );
	static bool				CheckFileExists			( cc8* path, bool allowRemap = false );
	static bool				CheckPathExists			( cc8* path );
	static bool				Copy					( cc8* path, cc8* newPath );
	static bool				DeleteDirectory			( cc8* path, bool force = false, bool recursive = false );
	static bool				DeleteFile				( cc8* path );
	static STLString		GetAbsoluteDirPath		( cc8* path );
	static STLString		GetAbsoluteFilePath		( cc8* path, bool allowRemap = true );
	static STLString		GetCurrentPath			();
	static bool				GetFileStat				( cc8* path, zl_stat& fileStat );
	static STLString		GetRelativePath			( cc8* path );
	static bool				MountVirtualDirectory	( cc8* path, cc8* archive );
	static bool				Rename					( cc8* oldPath, cc8* newPath );
	static bool				SetCurrentPath			( cc8* path );
	static STLString		TruncateFilename		( const char* filename );
};

#endif
