// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SQLCOLUMN_H
#define SQLCOLUMN_H

#include <sqlite3.h>

//================================================================//
// USSqlColumn
//================================================================//
class USSqlColumn {
protected:

	STLString	mColumnName;
	STLString	mTableName;
	STLString	mDeclaredType;
	STLString	mCollationSequenceName;
	bool		mIsNullable;
	bool		mIsPrimaryKey;
	bool		mIsAuto;
	bool		mColumnExists;

	//----------------------------------------------------------------//
	
public:

	friend class USSqlRecord;

	//----------------------------------------------------------------//
	void				Clear					();
	static bool			Exists					( sqlite3* connection, cc8* tableName, cc8* colName );
	static bool			Exists					( sqlite3* connection, cc8* tableName, cc8* colName, int type );
	cc8*				GetName					();
	bool				GetSqlMetadata			( sqlite3* connection );
	bool				GetSqlMetadata			( sqlite3* connection, sqlite3_stmt* statement, int iCol );
	bool				GetSqlMetadata			( sqlite3* connection, cc8* tableName, cc8* colName );
	cc8*				GetTableName			();
	cc8*				GetTypeDecl				();
	bool				IsAuto					();
	bool				IsNullable				();
	bool				IsPrimaryKey			();
	void				Print					();
						USSqlColumn				();
	virtual				~USSqlColumn			();
};

#endif
