// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USSqlConnection.h>
#include <uslsext/USSqlRecord.h>

//================================================================//
// USSqlConnection
//================================================================//

SUPPRESS_EMPTY_FILE_WARNING
#if USE_SQL

//----------------------------------------------------------------//
void USSqlConnection::Close () {

	if ( this->mOwns && this->mDatabase ) {
	
		sqlite3_exec ( this->mDatabase, "VACUUM", 0, 0, 0 );
	
		sqlite3_close ( this->mDatabase );
	}
	this->mDatabase = 0;
	this->mOwns = false;
}

//----------------------------------------------------------------//
bool USSqlConnection::Open ( cc8* filename ) {

	this->Close ();

	if ( sqlite3_open ( filename, &this->mDatabase ) == SQLITE_OK ) {
	
		sqlite3_exec ( this->mDatabase, "PRAGMA CACHE_SIZE = 10;", 0, 0, 0 );
		sqlite3_exec ( this->mDatabase, "PRAGMA SYNCHRONOUS = 0;", 0, 0, 0 );
		sqlite3_exec ( this->mDatabase, "PRAGMA LOCKING_MODE = EXCLUSIVE;", 0, 0, 0 );
	
		this->mOwns = true;
		return true;
	}
	
	this->mDatabase = 0;
	return false;
}

//----------------------------------------------------------------//
USSqlConnection::USSqlConnection () :
	mOwns ( false ),
	mDatabase ( 0 ) {
}

//----------------------------------------------------------------//
USSqlConnection::USSqlConnection ( sqlite3* database ) :
	mOwns ( false ),
	mDatabase ( database ) {
}

//----------------------------------------------------------------//
USSqlConnection::~USSqlConnection () {

	this->Close ();
}

#endif
