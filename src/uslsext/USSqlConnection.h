// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H

#include <sqlite3.h>

class USSqlRecord;

//================================================================//
// USSqlConnection
//================================================================//
class USSqlConnection {
protected:

	bool			mOwns;
	sqlite3*		mDatabase;

public:

	//----------------------------------------------------------------//
	void		Close					();
	bool		Open					( cc8* filename );
				USSqlConnection			();
				USSqlConnection			( sqlite3* database );
	virtual		~USSqlConnection		();
	
	//----------------------------------------------------------------//
	inline sqlite3* operator -> () const {
		return this->mDatabase;
	};

	//----------------------------------------------------------------//
	inline sqlite3& operator * () const {
		return *this->mDatabase;
	};

	//----------------------------------------------------------------//
	inline operator sqlite3* () {
		return this->mDatabase;
	};

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mDatabase != 0 );
	}
};

#endif
