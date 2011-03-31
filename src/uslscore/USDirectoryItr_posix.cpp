// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>

#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

//================================================================//
// USDirectoryItr
//================================================================//

//----------------------------------------------------------------//
cc8* USDirectoryItr::Current () {

	return this->mCurrentName;
}

//----------------------------------------------------------------//
void USDirectoryItr::Finish () {

	if ( this->mSearchHandle ) {
		closedir (( DIR* )this->mSearchHandle );
		this->mSearchHandle = 0;
	}
	this->mCurrentEntry = 0;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextDirectory () {

	USFileStat stat;

	this->mCurrentName.clear ();

	dirent* entry;
	while (( entry = readdir (( DIR* )this->mSearchHandle ))) {
		
		cc8* name = entry->d_name;
		if ( !name ) break;
		
		if ( name [ 0 ] == '.' ) {
			if ( name [ 1 ] == 0 ) continue;
			if (( name [ 1 ] == '.' ) && ( name [ 2 ] == 0 )) continue; 
		}
		
		if ( USFileSys::GetFileStat ( name, stat )) {
			if ( stat.mIsDir ) {
				this->mCurrentEntry = entry;
				this->mCurrentName = name;
				return name;
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextFile () {

	USFileStat stat;

	this->mCurrentName.clear ();

	dirent* entry;
	while (( entry = readdir (( DIR* )this->mSearchHandle ))) {
		
		cc8* name = entry->d_name;
		if ( !name ) break;
		
		if ( name [ 0 ] == '.' ) {
			if ( name [ 1 ] == 0 ) continue;
			if (( name [ 1 ] == '.' ) && ( name [ 2 ] == 0 )) continue; 
		}
		
		if ( USFileSys::GetFileStat ( name, stat )) {
			if ( stat.mIsFile ) {
				this->mCurrentEntry = entry;
				this->mCurrentName = name;
				return name;
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
void USDirectoryItr::Start () {

	this->Finish ();
	this->mSearchHandle = ( DIR* )opendir ( "." );
}

//----------------------------------------------------------------//
USDirectoryItr::USDirectoryItr () :
	mSearchHandle ( 0 ),
	mCurrentEntry ( 0 ) {
}

//----------------------------------------------------------------//
USDirectoryItr::~USDirectoryItr () {
	Finish ();
}

#endif
