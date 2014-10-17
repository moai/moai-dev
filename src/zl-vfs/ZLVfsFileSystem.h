// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLFILESYSTEM_H
#define ZLFILESYSTEM_H

#include <zl-vfs/zl_mutex.h>

class ZLVfsFile;
class ZLVfsVirtualPath;

typedef std::string ( *FileRemapCallback )( const std::string& remappedFilename );

//================================================================//
// ZLVfsFileSystem
//================================================================//
class ZLVfsFileSystem {
private:

	ZL_MUTEX* mMutex;

	std::string mWorkingPath;

	ZLVfsVirtualPath* mVirtualPaths;

	FileRemapCallback mFileRemapCallback;

	typedef std::map < std::string, std::string >::iterator		ReferenceMapIt;
	std::map < std::string, std::string >						mReferenceMap;

public:

	//----------------------------------------------------------------//
	int							AffirmPath					( const char* path );
	static std::string			BlessPath					( const char* path );
	int							ChangeDir					( const char* path );
	void						Cleanup						();
	static size_t				ComparePaths				( const char* p0, const char* p1 );
	ZLVfsVirtualPath*			FindBestVirtualPath			( char const* path );
	ZLVfsVirtualPath*			FindNextVirtualSubdir		( char const* path, ZLVfsVirtualPath* cursor ) ;
	ZLVfsVirtualPath*			FindVirtualPath				( char const* path );
	static ZLVfsFileSystem&		Get							();
	std::string					GetAbsoluteDirPath			( const char* path );
	std::string					GetAbsoluteFilePath			( const char* path );
	std::string					GetAbsolutePath				( const char* path );
	static std::string			GetBasename					( const char* filename );
	std::string					GetRelativePath				( const char* path, const char* base = 0 );
	std::string					GetWorkingPath				();
	void						Init						();
	static bool					IsSeparator					( char c );
	bool						IsVirtualPath				( char const* path );
	int							MakeDir						( char const* path );
	int							MountVirtual				( const char* path, const char* archive );
	static std::string			NormalizeDirPath			( const char* path );
	static std::string			NormalizeFilePath			( const char* path );
	std::string					PathFromRef					( const char* path );
	std::string					PathToRef					( const char* path );
	int							Remove						( const char* path );
	int							RemoveDir					( const char* path );
	int							Rename						( const char* oldname, const char* newname );
	void						SetFileRemapCallback		( FileRemapCallback callbackFct );
	void						SetPathRef					( const char* referenceName, const char* path );
	std::string					TruncateFilename			( const char* filename );
								ZLVfsFileSystem				();
								~ZLVfsFileSystem			();
};

#endif
