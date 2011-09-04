// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>

//================================================================//
// USDirectoryItr
//================================================================//

//----------------------------------------------------------------//
cc8* USDirectoryItr::Current () {

	return this->mCurrent;
}

//----------------------------------------------------------------//
void USDirectoryItr::Finish () {

	if ( this->mHandle ) {
		zipfs_dir_close (( ZIPFSDIR )this->mHandle );
		this->mHandle = 0;
	}
	this->mCurrent = 0;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextDirectory () {

	this->mCurrent = 0;

	ZIPFSDIR* dir = ( ZIPFSDIR* )this->mHandle;
	while ( zipfs_dir_read_entry ( dir )) {
		if ( zipfs_dir_entry_is_subdir ( dir )) {
			this->mCurrent = zipfs_dir_entry_name ( dir );
			break;
		}
	}
	return this->mCurrent;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextEntry () {

	ZIPFSDIR* dir = ( ZIPFSDIR* )this->mHandle;

	zipfs_dir_read_entry ( dir );
	this->mCurrent= zipfs_dir_entry_name ( dir );

	return this->mCurrent;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextFile () {

	this->mCurrent = 0;

	ZIPFSDIR* dir = ( ZIPFSDIR* )this->mHandle;
	while ( zipfs_dir_read_entry ( dir )) {
		if ( !zipfs_dir_entry_is_subdir ( dir )) {
			this->mCurrent = zipfs_dir_entry_name ( dir );
			break;
		}
	}
	return this->mCurrent;
}

//----------------------------------------------------------------//
void USDirectoryItr::Start () {

	this->Finish ();
	this->mHandle = zipfs_dir_open ();
}

//----------------------------------------------------------------//
USDirectoryItr::USDirectoryItr () :
	mHandle ( 0 ),
	mCurrent ( 0 ) {
}

//----------------------------------------------------------------//
USDirectoryItr::~USDirectoryItr () {
	Finish ();
}

