// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>

#include <errno.h>

//================================================================//
// USFileSys
//================================================================//

//----------------------------------------------------------------//
bool USFileSys::AffirmPath ( cc8* path ) {

	return ( zipfs_affirm_path ( path ) == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::CheckFileExists ( cc8* path ) {

	zipfs_stat fileStat;
	
	if( USFileSys::GetFileStat ( path, fileStat ) ) {
		return ( fileStat.mExists != 0 && fileStat.mIsDir == 0 );
	}
	return false;
}

//----------------------------------------------------------------//
bool USFileSys::CheckPathExists ( cc8* path ) {

	zipfs_stat fileStat;
	
	if( USFileSys::GetFileStat ( path, fileStat ) ) {
		return ( fileStat.mExists != 0 && fileStat.mIsDir != 0 );
	}
	return false;
}

//----------------------------------------------------------------//
bool USFileSys::DeleteDirectory ( cc8* path, bool force, bool recursive ) {

	if ( USFileSys::CheckPathExists ( path ) == false ) return true;
		
	int result = zipfs_rmdir ( path );
	
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
	return ( zipfs_rmdir ( path ) == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::DeleteFile ( cc8* path ) {

	if ( USFileSys::CheckFileExists ( path )) {
		return ( zipfs_remove ( path ) == 0 );
	}
	return true;
}

//----------------------------------------------------------------//
STLString USFileSys::GetAbsoluteDirPath ( cc8* path ) {

	return zipfs_get_abs_dirpath ( path );
}

//----------------------------------------------------------------//
STLString USFileSys::GetAbsoluteFilePath ( cc8* path ) {

	return zipfs_get_abs_filepath ( path );
}

//----------------------------------------------------------------//
STLString USFileSys::GetCurrentPath () {

	return zipfs_get_working_path ();
}

//----------------------------------------------------------------//
bool USFileSys::GetFileStat ( cc8* filename, zipfs_stat& fileStat ) {

	int result = zipfs_get_stat ( filename, &fileStat );
	if ( result ) return false;
	return true;
}

//----------------------------------------------------------------//
STLString USFileSys::GetRelativePath ( cc8* path ) {

	USFilename filename;
	return filename.GetRelativePath ( path );
}

//----------------------------------------------------------------//
bool USFileSys::MountVirtualDirectory ( cc8* path, cc8* archive ) {

	int result = zipfs_mount_virtual ( path, archive );
	return ( result == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::Rename ( cc8* oldPath, cc8* newPath ) {

	int result = zipfs_rename ( oldPath, newPath );
	return ( result == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::SetCurrentPath ( cc8* path ) {

	int result = zipfs_chdir ( path );
	return ( result == 0 );
}
