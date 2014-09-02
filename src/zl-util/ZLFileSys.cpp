// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-vfs/ZLVfsZipArchive.h>
#include <zl-vfs/ZLVfsFileSystem.h>
#include <zl-util/ZLDirectoryItr.h>
#include <zl-util/ZLFileStream.h>
#include <zl-util/ZLFileSys.h>

#include <errno.h>

//================================================================//
// ZLFileSys
//================================================================//

//----------------------------------------------------------------//
bool ZLFileSys::AffirmPath ( cc8* path ) {

	return ( zl_affirm_path ( path ) == 0 );
}

//----------------------------------------------------------------//
bool ZLFileSys::CheckFileExists ( cc8* path ) {
	//printf ( "CheckFileExists %s\n", path );
	zl_stat fileStat;
	
	if ( ZLFileSys::GetFileStat ( path, fileStat )) {
		return ( fileStat.mExists != 0 && fileStat.mIsDir == 0 );
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLFileSys::CheckPathExists ( cc8* path ) {
	
	zl_stat fileStat;
	
	if ( ZLFileSys::GetFileStat ( path, fileStat )) {
		return ( fileStat.mExists && fileStat.mIsDir );
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLFileSys::Copy ( cc8* path, cc8* newPath ) {

	zl_stat fileStat;

	if ( !ZLFileSys::GetFileStat ( path, fileStat )) return false;
	if ( !fileStat.mExists ) return false;

	if ( fileStat.mIsDir ) {
		
		bool result = true;
		
		STLString cwd = ZLFileSys::GetCurrentPath ();
		STLString toPath = ZLFileSys::GetAbsoluteDirPath ( newPath );
		
		ZLFileSys::AffirmPath ( toPath );
		
		ZLFileSys::SetCurrentPath ( path );
		
		ZLDIR* itr = zl_dir_open ();
		if ( itr ) {
			while ( zl_dir_read_entry ( itr )) {
				cc8* entry = zl_dir_entry_name ( itr );
				if ( strcmp ( entry, "." ) == 0 ) continue;
				if ( strcmp ( entry, ".." ) == 0 ) continue;
				
				STLString destEntry = toPath;
				destEntry.append ( entry );
				
				if ( !ZLFileSys::Copy ( entry, destEntry )) {
					result = false;
					break;
				}
			}
			zl_dir_close ( itr );
		}
		ZLFileSys::SetCurrentPath ( cwd );
		
		return result;
	}
	else {
		ZLFileStream infile;
		if ( infile.OpenRead ( path )) {
		
			ZLFileStream outfile;
			if ( outfile.OpenWrite ( newPath )) {
				outfile.WriteStream ( infile );
				return true;
			}
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
bool ZLFileSys::DeleteDirectory ( cc8* path, bool force, bool recursive ) {

	if ( ZLFileSys::CheckPathExists ( path ) == false ) return true;
		
	int result = zl_rmdir ( path );
	
	if ( result == 0 ) return true;
	if ( !( force || recursive )) return false;
	
	STLString currentDir = ZLFileSys::GetCurrentPath ();
	ZLFileSys::SetCurrentPath ( path );
	
	ZLDirectoryItr dirItr;
	
	if ( force ) {
		dirItr.Start ();
		while ( dirItr.NextFile ()) {
			ZLFileSys::DeleteFile ( dirItr.Current ());
		}
	}
	
	if ( recursive ) {
		dirItr.Start ();
		while ( dirItr.NextDirectory ()) {
			if( strcmp ( dirItr.Current (), ".." ) == 0 ||
				strcmp ( dirItr.Current (), "." ) == 0 ) {
				continue;
			}
			ZLFileSys::DeleteDirectory ( dirItr.Current (), force, recursive );
		}
	}

	ZLFileSys::SetCurrentPath ( currentDir );
	return ( zl_rmdir ( path ) == 0 );
}

//----------------------------------------------------------------//
bool ZLFileSys::DeleteFile ( cc8* path ) {

	if ( ZLFileSys::CheckFileExists ( path )) {
		return ( zl_remove ( path ) == 0 );
	}
	return true;
}

//----------------------------------------------------------------//
STLString ZLFileSys::GetAbsoluteDirPath ( cc8* path ) {

	return ZLVfsFileSystem::Get ().GetAbsoluteDirPath ( path );
}

//----------------------------------------------------------------//
STLString ZLFileSys::GetAbsoluteFilePath ( cc8* path ) {

	return ZLVfsFileSystem::Get ().GetAbsoluteFilePath ( path );
}

//----------------------------------------------------------------//
STLString ZLFileSys::GetCurrentPath () {

	return ZLVfsFileSystem::Get ().GetWorkingPath ();
}

//----------------------------------------------------------------//
bool ZLFileSys::GetFileStat ( cc8* filename, zl_stat& fileStat ) {

	int result = zl_get_stat ( filename, &fileStat );
	if ( result ) return false;
	return true;
}

//----------------------------------------------------------------//
STLString ZLFileSys::GetRelativePath ( cc8* path, cc8* base ) {

	return ZLVfsFileSystem::Get ().GetRelativePath ( path, base );
}

//----------------------------------------------------------------//
bool ZLFileSys::MountVirtualDirectory ( cc8* path, cc8* archive ) {

	int result = ZLVfsFileSystem::Get ().MountVirtual ( path, archive );
	return ( result == 0 );
}

//----------------------------------------------------------------//
STLString ZLFileSys::PathFromRef ( const char* path ) {

	return ZLVfsFileSystem::Get ().PathFromRef ( path );
}

//----------------------------------------------------------------//
STLString ZLFileSys::PathToRef ( const char* path ) {

	return ZLVfsFileSystem::Get ().PathToRef ( path );
}

//----------------------------------------------------------------//
bool ZLFileSys::Rename ( cc8* oldPath, cc8* newPath ) {

	if ( ZLFileSys::CheckFileExists ( oldPath )) {
		int result = zl_rename ( oldPath, newPath );
		return ( result == 0 );
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLFileSys::SetCurrentPath ( cc8* path ) {

	int result = zl_chdir ( path );
	return ( result == 0 );
}

//----------------------------------------------------------------//
void ZLFileSys::SetPathRef ( const char* referenceName, const char* path ) {

	ZLVfsFileSystem::Get ().SetPathRef ( referenceName, path );
}

//----------------------------------------------------------------//
STLString ZLFileSys::TruncateFilename ( const char* filename ) {

	return ZLVfsFileSystem::Get ().TruncateFilename ( filename );
}

//----------------------------------------------------------------//
bool ZLFileSys::StripPKZipTimestamps ( const char* infilename, const char* outfilename ) {

	return ZLVfsZipArchive::StripTimestamps ( infilename, outfilename ) == 0;
}
