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

	return this->mCurrent.size () ? this->mCurrent.c_str () : 0;
}

//----------------------------------------------------------------//
void USDirectoryItr::Finish () {

	if ( this->mItr ) {
		zl_dir_close ( this->mItr );
		this->mItr = 0;
	}
	this->mCurrent.clear ();
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextDirectory () {

	this->mCurrent.clear ();

	while ( zl_dir_read_entry ( this->mItr )) {
		if ( zl_dir_entry_is_subdir ( this->mItr )) {
			this->mCurrent = zl_dir_entry_name ( this->mItr );
			break;
		}
	}
	return this->mCurrent.size () ? this->mCurrent.c_str () : 0;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextEntry () {

	zl_dir_read_entry ( this->mItr );
	this->mCurrent = zl_dir_entry_name ( this->mItr );

	return this->mCurrent.size () ? this->mCurrent.c_str () : 0;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextFile () {

	this->mCurrent.clear ();

	while ( zl_dir_read_entry ( this->mItr )) {
		if ( !zl_dir_entry_is_subdir ( this->mItr )) {
			this->mCurrent = zl_dir_entry_name ( this->mItr );
			break;
		}
	}
	return this->mCurrent.size () ? this->mCurrent.c_str () : 0;
}

//----------------------------------------------------------------//
void USDirectoryItr::Start () {

	this->Finish ();
	this->mItr = zl_dir_open ();
}

//----------------------------------------------------------------//
USDirectoryItr::USDirectoryItr () :
	mItr ( 0 ) {
}

//----------------------------------------------------------------//
USDirectoryItr::~USDirectoryItr () {
	Finish ();
}

