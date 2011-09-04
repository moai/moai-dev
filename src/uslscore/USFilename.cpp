// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>

//================================================================//
// USFilename
//================================================================//

//----------------------------------------------------------------//
char* USFilename::GetAbsoluteDirPath ( cc8* path ) {

	path = moaio_get_abs_dirpath ( path );
	strcpy ( this->mBuffer, path );
	
	return this->mBuffer;
}

//----------------------------------------------------------------//
char* USFilename::GetAbsoluteFilePath ( cc8* path ) {

	path = moaio_get_abs_filepath ( path );
	strcpy ( this->mBuffer, path );
	
	return this->mBuffer;
}

//----------------------------------------------------------------//
char* USFilename::GetCurrentPath () {

	return moaio_getcwd ( this->mBuffer, PATH_MAX );
}

//----------------------------------------------------------------//
char* USFilename::GetRelativePath ( cc8* path ) {
	
	path = moaio_get_rel_path ( path );
	strcpy ( this->mBuffer, path );
	return this->mBuffer;
}

//----------------------------------------------------------------//
char* USFilename::TruncateFilename ( cc8* filename ) {

	filename = moaio_bless_path ( filename );
	
	int len = 0;
	for ( u32 i = 0; filename [ i ]; ++i ) {
		if ( filename [ i ] == '/' ) {
			len = i + 1;
		}
	}

	memcpy ( this->mBuffer, filename, len );
	this->mBuffer [ len ] = 0;
	return this->mBuffer;
}

//----------------------------------------------------------------//
USFilename::USFilename () {
	this->mBuffer [ 0 ] = 0;
}

//----------------------------------------------------------------//
USFilename::~USFilename () {
}
