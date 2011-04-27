// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SQLSTATEMENT_H
#define SQLSTATEMENT_H

#include <uslsext/USSqlColumn.h>
#include <sqlite3.h>

//================================================================//
// USSqlStatement
//================================================================//
class USSqlStatement {
protected:

	bool			mOwns;
	sqlite3_stmt*	mStatement;
	sqlite3*		mConnection;
	
public:

	//----------------------------------------------------------------//
	bool				BindDouble				( u32 idx, double value );
	bool				BindFloat				( u32 idx, float value );
	bool				BindInt					( u32 idx, int value );
	bool				BindInt64				( u32 idx, s64 value );
	bool				BindString				( u32 idx, cc8* value );
	void				Clear					();
	bool				CreateIndex				( cc8* table, cc8* column );
	bool				Execute					( cc8* statement, ... );
	cc8*				GetColumnName			( u32 iCol );
	cc8*				GetColumnTableName		( u32 iCol );
	double				GetDouble				( u32 iCol, double value );
	float				GetFloat				( u32 iCol, float value );
	int					GetInt					( u32 iCol, int value );
	s64					GetInt64				( u32 iCol, s64 value );
	cc8*				GetString				( u32 iCol, cc8* value );
	bool				Prepare					( cc8* statement, ... );
	void				PrintColumnInfo			();
	void				Reset					();
	void				Set						( sqlite3_stmt* statement );
	void				SetConnection			( sqlite3* connection );
	bool				Step					();
	bool				StepRow					();
	void				Take					( USSqlStatement& statement );
						USSqlStatement			();
						USSqlStatement			( const USSqlStatement& statement );
						USSqlStatement			( sqlite3* connection );
						USSqlStatement			( sqlite3* connection, cc8* statement );
						USSqlStatement			( sqlite3* connection,  sqlite3_stmt* statement );
	virtual				~USSqlStatement			();
	
	//----------------------------------------------------------------//
	inline void operator = ( const USSqlStatement& statement ) {
		this->Take (( USSqlStatement& )statement );
	};
	
	//----------------------------------------------------------------//
	inline sqlite3_stmt* operator -> () const {
		return this->mStatement;
	};

	//----------------------------------------------------------------//
	inline sqlite3_stmt& operator * () const {
		return *this->mStatement;
	};

	//----------------------------------------------------------------//
	inline operator sqlite3_stmt* () {
		return this->mStatement;
	};

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mStatement != 0 );
	}
};

#endif
