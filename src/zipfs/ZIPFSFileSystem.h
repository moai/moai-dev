// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFSFILESYSTEM_H
#define ZIPFSFILESYSTEM_H

#include <zipfs/zipfs_mutex.h>

class ZIPFSFile;
class ZIPFSVirtualPath;

//================================================================//
// ZIPFSFileSystem
//================================================================//
class ZIPFSFileSystem {
private:

	ZIPFS_MUTEX* mMutex;

	std::string mWorkingPath;

	ZIPFSVirtualPath* mVirtualPaths;

public:

	//----------------------------------------------------------------//
	int							AffirmPath				( const char* path );
	static std::string			BlessPath				( const char* path );
	int							ChangeDir				( const char* path );
	void						Cleanup					();
	static size_t				ComparePaths			( const char* p0, const char* p1 );
	ZIPFSVirtualPath*			FindBestVirtualPath		( char const* path );
	ZIPFSVirtualPath*			FindNextVirtualSubdir	( char const* path, ZIPFSVirtualPath* cursor ) ;
	ZIPFSVirtualPath*			FindVirtualPath			( char const* path );
	static ZIPFSFileSystem&		Get						();
	std::string					GetAbsoluteDirPath		( const char* path );
	std::string					GetAbsoluteFilePath		( const char* path );
	static std::string			GetBasename				( const char* filename );
	std::string					GetRelativePath			( const char* path );
	std::string					GetWorkingPath			();
	void						Init					();
	static bool					IsSeparator				( char c );
	bool						IsVirtualPath			( char const* path );
	int							MakeDir					( char const* path );
	int							MountVirtual			( const char* path, const char* archive );
	static std::string			NormalizePath			( const char* path );
	int							Remove					( const char* path );
	int							RemoveDir				( const char* path );
	int							Rename					( const char* oldname, const char* newname );
	std::string					TruncateFilename		( const char* filename );
								ZIPFSFileSystem			();
								~ZIPFSFileSystem		();
};

#endif
