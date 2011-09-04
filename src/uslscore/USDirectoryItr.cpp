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
		moaio_dir_close (( MOAIODIR )this->mHandle );
		this->mHandle = 0;
	}
	this->mCurrent = 0;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextDirectory () {

	this->mCurrent = 0;

	MOAIODIR* dir = ( MOAIODIR* )this->mHandle;
	while ( moaio_dir_read_entry ( dir )) {
		if ( moaio_dir_entry_is_subdir ( dir )) {
			this->mCurrent = moaio_dir_entry_name ( dir );
			break;
		}
	}
	return this->mCurrent;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextEntry () {

	MOAIODIR* dir = ( MOAIODIR* )this->mHandle;

	moaio_dir_read_entry ( dir );
	this->mCurrent= moaio_dir_entry_name ( dir );

	return this->mCurrent;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextFile () {

	this->mCurrent = 0;

	MOAIODIR* dir = ( MOAIODIR* )this->mHandle;
	while ( moaio_dir_read_entry ( dir )) {
		if ( !moaio_dir_entry_is_subdir ( dir )) {
			this->mCurrent = moaio_dir_entry_name ( dir );
			break;
		}
	}
	return this->mCurrent;
}

//----------------------------------------------------------------//
void USDirectoryItr::Start () {

	this->Finish ();
	this->mHandle = moaio_dir_open ();
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

