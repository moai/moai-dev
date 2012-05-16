// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlcore/ZLFileSystem.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USFileSys.h>

#include <errno.h>

//================================================================//
// USFileSys
//================================================================//

//----------------------------------------------------------------//
bool USFileSys::AffirmPath ( cc8* path ) {

	return ( zl_affirm_path ( path ) == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::CheckFileExists ( cc8* path ) {

	zl_stat fileStat;
	
	if( USFileSys::GetFileStat ( path, fileStat ) ) {
		return ( fileStat.mExists != 0 && fileStat.mIsDir == 0 );
	}
	return false;
}

//----------------------------------------------------------------//
bool USFileSys::CheckPathExists ( cc8* path ) {

	STLString currentPath = USFileSys::GetCurrentPath ();
	bool exists = USFileSys::SetCurrentPath ( path );
	
	USFileSys::SetCurrentPath ( currentPath );
	return exists;
}

//----------------------------------------------------------------//
bool USFileSys::DeleteDirectory ( cc8* path, bool force, bool recursive ) {

	if ( USFileSys::CheckPathExists ( path ) == false ) return true;
		
	int result = zl_rmdir ( path );
	
	if ( result == 0 ) return true;
	if ( !( force || recursive )) return false;
	
	STLString currentDir = USFileSys::GetCurrentPath ();
	USFileSys::SetCurrentPath ( path );
	
	USDirectoryItr dirItr;
	
	if ( force ) {
		dirItr.Start ();
		while ( dirItr.NextFile ()) {
			USFileSys::DeleteFile ( dirItr.Current ());
		}
	}
	
	if ( recursive ) {
		dirItr.Start ();
		while ( dirItr.NextDirectory ()) {
			if( strcmp ( dirItr.Current (), ".." ) == 0 ||
				strcmp ( dirItr.Current (), "." ) == 0 ) {
				continue;
			}
			USFileSys::DeleteDirectory ( dirItr.Current (), force, recursive );
		}
	}

	USFileSys::SetCurrentPath ( currentDir );
	return ( zl_rmdir ( path ) == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::DeleteFile ( cc8* path ) {

	if ( USFileSys::CheckFileExists ( path )) {
		return ( zl_remove ( path ) == 0 );
	}
	return true;
}

//----------------------------------------------------------------//
STLString USFileSys::GetAbsoluteDirPath ( cc8* path ) {

	return ZLFileSystem::Get ().GetAbsoluteDirPath ( path );
}

//----------------------------------------------------------------//
STLString USFileSys::GetAbsoluteFilePath ( cc8* path ) {

	return ZLFileSystem::Get ().GetAbsoluteFilePath ( path );
}

//----------------------------------------------------------------//
STLString USFileSys::GetCurrentPath () {

	return ZLFileSystem::Get ().GetWorkingPath ();
}

//----------------------------------------------------------------//
bool USFileSys::GetFileStat ( cc8* filename, zl_stat& fileStat ) {

	int result = zl_get_stat ( filename, &fileStat );
	if ( result ) return false;
	return true;
}

//----------------------------------------------------------------//
STLString USFileSys::GetRelativePath ( cc8* path ) {

	return ZLFileSystem::Get ().GetRelativePath ( path );
}

//----------------------------------------------------------------//
bool USFileSys::MountVirtualDirectory ( cc8* path, cc8* archive ) {

	int result = ZLFileSystem::Get ().MountVirtual ( path, archive );
	return ( result == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::Rename ( cc8* oldPath, cc8* newPath ) {

	int result = zl_rename ( oldPath, newPath );
	return ( result == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::SetCurrentPath ( cc8* path ) {

	int result = zl_chdir ( path );
	return ( result == 0 );
}

//----------------------------------------------------------------//
STLString USFileSys::TruncateFilename ( const char* filename ) {

	return ZLFileSystem::Get ().TruncateFilename ( filename );
}
