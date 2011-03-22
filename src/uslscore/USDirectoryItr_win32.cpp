// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <errno.h>
#include <direct.h>
#include <io.h>

#include <uslscore/USFileName.h>
#include <uslscore/USDirectoryItr.h>

typedef struct _finddata_t _finddata;

//================================================================//
// USDirectoryItr
//================================================================//

//----------------------------------------------------------------//
cc8* USDirectoryItr::Current () {

	return this->mCurrentName;
}

//----------------------------------------------------------------//
void USDirectoryItr::Finish () {

	this->mPath.Clear ();

	if ( mSearchState > 0 ) {
		_findclose( mSearchState );
		mSearchState = -1;
	}
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextDirectory () {

	this->mPath.Clear ();
	this->mCurrentName.clear ();

	_finddata* fileStat = ( _finddata* )mSearchHandle;

	if ( mSearchState < 0 ) return 0;

	while (( !( fileStat->attrib & _A_SUBDIR )) || ( fileStat->name[0] == '.' ))
		if ( _findnext( mSearchState, fileStat ) < 0 ) {
			_findclose( mSearchState );
			mSearchState = -1;
			return 0;
		}

	this->mPath.Bless ( fileStat->name );
	this->mPath.ConvertToPath ();
	
	if ( _findnext( mSearchState, fileStat ) < 0 ) {
		_findclose( mSearchState );
		mSearchState = -1;
	}
	
	this->mCurrentName = this->mPath.mBuffer;
	return this->mPath.mBuffer;
}

//----------------------------------------------------------------//
cc8* USDirectoryItr::NextFile () {

	this->mPath.Clear ();
	this->mCurrentName.clear ();

	_finddata* fileStat = ( _finddata* )mSearchHandle;

	if ( mSearchState < 0 ) return 0;

	while ( fileStat->attrib & _A_SUBDIR )
		if ( _findnext( mSearchState, fileStat ) < 0 ) {
			_findclose( mSearchState );
			mSearchState = -1;
			return 0;
		}

	this->mPath.Bless ( fileStat->name );
	
	if ( _findnext( mSearchState, fileStat ) < 0 ) {
		_findclose( mSearchState );
		mSearchState = -1;
	}
	
	this->mCurrentName = this->mPath.mBuffer;
	return this->mPath.mBuffer;
}

//----------------------------------------------------------------//
void USDirectoryItr::Start () {
	_finddata* fileStat = ( _finddata* )mSearchHandle;
	Finish ();
	mSearchState = ( s32 )_findfirst( "*", fileStat );
}

//----------------------------------------------------------------//
USDirectoryItr::USDirectoryItr () {

	this->mPath.Clear ();

	mSearchHandle = malloc ( sizeof ( _finddata ));
	assert ( mSearchHandle );
	mSearchState = -1;
}

//----------------------------------------------------------------//
USDirectoryItr::~USDirectoryItr () {
	Finish ();
	free ( mSearchHandle );
}

#endif
