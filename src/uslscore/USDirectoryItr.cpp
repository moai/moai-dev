// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
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

	if ( this->mDir ) {
		zl_dir_close ( this->mDir );
		this->mDir = 0;
	}
	this->mCurrent = 0;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextDirectory () {

	this->mCurrent = 0;

	while ( zl_dir_read_entry ( this->mDir )) {
		if ( zl_dir_entry_is_subdir ( this->mDir )) {
			this->mCurrent = zl_dir_entry_name ( this->mDir );
			break;
		}
	}
	return this->mCurrent;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextEntry () {

	zl_dir_read_entry ( this->mDir );
	this->mCurrent= zl_dir_entry_name ( this->mDir );

	return this->mCurrent;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextFile () {

	this->mCurrent = 0;

	while ( zl_dir_read_entry ( this->mDir )) {
		if ( !zl_dir_entry_is_subdir ( this->mDir )) {
			this->mCurrent = zl_dir_entry_name ( this->mDir );
			break;
		}
	}
	return this->mCurrent;
}

//----------------------------------------------------------------//
void USDirectoryItr::Start () {

	this->Finish ();
	this->mDir = zl_dir_open ();
}

//----------------------------------------------------------------//
USDirectoryItr::USDirectoryItr () :
	mDir ( 0 ),
	mCurrent ( 0 ) {
}

//----------------------------------------------------------------//
USDirectoryItr::~USDirectoryItr () {
	Finish ();
}

