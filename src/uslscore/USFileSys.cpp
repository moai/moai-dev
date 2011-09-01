// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>

#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#ifdef _WIN32
	#define S_ISDIR(B) (((B)&_S_IFDIR)!=0)
#endif

#ifndef _WIN32

	int mkdir ( const char* path );

	//----------------------------------------------------------------//
	int mkdir ( const char* path ) {
		return mkdir ( path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
	}

#endif

//================================================================//
// USFileSys
//================================================================//

//----------------------------------------------------------------//
void USFileSys::AffirmPath ( cc8* path ) {

	assert ( path );

	USFilename fullpath;
	fullpath.GetPath ( path );

	bool more = true;
	char* cursor = fullpath.mBuffer;
	
	// While there is still some string left in dirname...
	while ( *cursor ) {

		// Copy the next directory name into newDir
		while (( *cursor ) && ( *cursor != '/' )) ++cursor;
		
		if ( *cursor ) {
			more = true;
			*cursor = 0;
		}
		else more = false;
		
		if ( *fullpath.mBuffer ) {
			int result = mkdir ( fullpath.mBuffer );
			assert ( !( result && ( errno != EEXIST )));
		}
		
		if ( more ) {
			*cursor = '/';
		}
		else {
			return;
		}
		++cursor;
	}
}

//----------------------------------------------------------------//
bool USFileSys::CheckFileExists ( cc8* path ) {

	USFileStat fileStat;
	USFileSys::GetFileStat ( path, fileStat );
	return fileStat.mExists;
}

//----------------------------------------------------------------//
bool USFileSys::CheckPathExists ( cc8* path ) {

	USFilename currentDir;
	currentDir.GetCurrentPath ();
	
	USFilename pathBuf;
	pathBuf.GetPath ( path );

	bool exists = USFileSys::SetCurrentPath ( pathBuf.mBuffer );
	
	USFileSys::SetCurrentPath ( currentDir.mBuffer );
	return exists;
}

//----------------------------------------------------------------//
bool USFileSys::DeleteDirectory ( cc8* path, bool force, bool recursive ) {

	if ( USFileSys::CheckPathExists ( path ) == false ) return true;
		
	int result = rmdir ( path );
	
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
			USFileSys::DeleteDirectory ( dirItr.Current (), force, recursive );
		}
	}

	USFileSys::SetCurrentPath ( currentDir );
	return ( rmdir( path ) == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::DeleteFile ( cc8* path ) {

	if ( USFileSys::CheckFileExists ( path )) {
		return ( remove ( path ) == 0 );
	}
	return true;
}

//----------------------------------------------------------------//
STLString USFileSys::Expand ( cc8* path ) {

	USFilename fullpath;
	return fullpath.Expand ( path );
}

//----------------------------------------------------------------//
STLString USFileSys::ExpandPath ( cc8* path ) {

	USFilename fullpath;
	return fullpath.ExpandPath ( path );
}

//----------------------------------------------------------------//
STLString USFileSys::GetCurrentPath () {

	USFilename fullpath;
	return fullpath.GetCurrentPath ();
}

//----------------------------------------------------------------//
bool USFileSys::GetFileStat ( cc8* filename, USFileStat& fileStat ) {

	fileStat.mExists = false;

	struct stat s;
	int result = stat ( filename, &s );
	if ( result ) return false;

	fileStat.mExists		= true;
	
	fileStat.mIsDir			= S_ISDIR ( s.st_mode );
	fileStat.mIsFile		= !fileStat.mIsDir;
	
	fileStat.mSize			= s.st_size;
	fileStat.mTimeCreated	= s.st_ctime;
	fileStat.mTimeModified	= s.st_mtime;
	fileStat.mTimeViewed	= s.st_atime;
	
	return true;
}

//----------------------------------------------------------------//
STLString USFileSys::GetRelativePath ( cc8* path ) {

	USFilename filename;
	return filename.GetRelativePath ( path );
}

//----------------------------------------------------------------//
bool USFileSys::Rename ( cc8* oldPath, cc8* newPath ) {

	int result = rename ( oldPath, newPath );
	return ( result == 0 );
}

//----------------------------------------------------------------//
bool USFileSys::SetCurrentPath ( cc8* path ) {

	int result = chdir ( path );
	return ( result == 0 );
}
