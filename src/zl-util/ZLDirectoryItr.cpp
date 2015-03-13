// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLFileSys.h>
#include <zl-util/ZLDirectoryItr.h>

//================================================================//
// ZLDirectoryItr
//================================================================//

//----------------------------------------------------------------//
cc8* ZLDirectoryItr::Current () {

	return this->mCurrent.size () ? this->mCurrent.c_str () : 0;
}

//----------------------------------------------------------------//
void ZLDirectoryItr::Finish () {

	if ( this->mItr ) {
		zl_dir_close ( this->mItr );
		this->mItr = 0;
	}
	this->mCurrent.clear ();
}

//----------------------------------------------------------------//
cc8* ZLDirectoryItr::NextDirectory () {

	this->mCurrent.clear ();

	while ( zl_dir_read_entry ( this->mItr )) {
		if ( zl_dir_entry_is_subdir ( this->mItr )) {
		
			this->mCurrent = zl_dir_entry_name ( this->mItr );
			
			if (( strcmp ( this->Current (), "." ) != 0 ) && ( strcmp ( this->Current (), ".." ) != 0 )) {
				return this->mCurrent.c_str ();
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
cc8* ZLDirectoryItr::NextEntry () {

	zl_dir_read_entry ( this->mItr );
	this->mCurrent = zl_dir_entry_name ( this->mItr );

	return this->mCurrent.size () ? this->mCurrent.c_str () : 0;
}

//----------------------------------------------------------------//
cc8* ZLDirectoryItr::NextFile () {

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
void ZLDirectoryItr::Start () {

	this->Finish ();
	this->mItr = zl_dir_open ();
}

//----------------------------------------------------------------//
ZLDirectoryItr::ZLDirectoryItr () :
	mItr ( 0 ) {
}

//----------------------------------------------------------------//
ZLDirectoryItr::~ZLDirectoryItr () {
	Finish ();
}

