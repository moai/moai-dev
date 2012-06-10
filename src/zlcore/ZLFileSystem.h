// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLFILESYSTEM_H
#define ZLFILESYSTEM_H

#include <zlcore/zl_mutex.h>

class ZLFile;
class ZLVirtualPath;

//================================================================//
// ZLFileSystem
//================================================================//
class ZLFileSystem {
private:

	ZL_MUTEX* mMutex;

	std::string mWorkingPath;

	ZLVirtualPath* mVirtualPaths;

public:

	//----------------------------------------------------------------//
	int						AffirmPath				( const char* path );
	static std::string		BlessPath				( const char* path );
	int						ChangeDir				( const char* path );
	void					Cleanup					();
	static size_t			ComparePaths			( const char* p0, const char* p1 );
	ZLVirtualPath*			FindBestVirtualPath		( char const* path );
	ZLVirtualPath*			FindNextVirtualSubdir	( char const* path, ZLVirtualPath* cursor ) ;
	ZLVirtualPath*			FindVirtualPath			( char const* path );
	static ZLFileSystem&	Get						();
	std::string				GetAbsoluteDirPath		( const char* path );
	std::string				GetAbsoluteFilePath		( const char* path );
	static std::string		GetBasename				( const char* filename );
	std::string				GetRelativePath			( const char* path );
	std::string				GetWorkingPath			();
	void					Init					();
	static bool				IsSeparator				( char c );
	bool					IsVirtualPath			( char const* path );
	int						MakeDir					( char const* path );
	int						MountVirtual			( const char* path, const char* archive );
	static std::string		NormalizeDirPath		( const char* path );
	static std::string		NormalizeFilePath		( const char* path );
	int						Remove					( const char* path );
	int						RemoveDir				( const char* path );
	int						Rename					( const char* oldname, const char* newname );
	std::string				TruncateFilename		( const char* filename );
							ZLFileSystem			();
							~ZLFileSystem			();
};

#endif
