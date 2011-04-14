// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USSqlColumn.h>

//================================================================//
// USSqlColumn
//================================================================//

//----------------------------------------------------------------//
void USSqlColumn::Clear () {

	this->mColumnName				= "";
	this->mTableName				= "";
	this->mDeclaredType				= "";
	this->mCollationSequenceName	= "";
	this->mIsNullable				= false;
	this->mIsPrimaryKey				= false;
	this->mIsAuto					= false;
}

//----------------------------------------------------------------//
bool USSqlColumn::Exists ( sqlite3* connection, cc8* tableName, cc8* colName ) {

	int result = sqlite3_table_column_metadata (
		connection,
		NULL,
		tableName,
		colName,
		0,
		0,
		0,
		0,
		0
	);
	
	return ( result == SQLITE_OK );
}

//----------------------------------------------------------------//
cc8* USSqlColumn::GetName () {
	return this->mColumnName.str ();
}

//----------------------------------------------------------------//
bool USSqlColumn::GetSqlMetadata ( sqlite3* connection ) {

	this->mColumnExists = false;

	cc8* declType;
	cc8* collSeq;
	int noNulls;
	int primKey;
	int autoInc;
	
	int result = sqlite3_table_column_metadata (
		connection,
		NULL,
		this->mTableName,
		this->mColumnName,
		&declType,
		&collSeq,
		&noNulls,
		&primKey,
		&autoInc
	);
	
	if ( result == SQLITE_OK ) {
		this->mColumnExists				= true;
		this->mDeclaredType				= declType;
		this->mCollationSequenceName	= collSeq;
		this->mIsNullable				= ( noNulls == 0 );
		this->mIsPrimaryKey				= ( primKey != 0 );
		this->mIsAuto					= ( autoInc != 0 );
	}
	
	return this->mColumnExists;
}

//----------------------------------------------------------------//
bool USSqlColumn::GetSqlMetadata ( sqlite3* connection, sqlite3_stmt* statement, int iCol ) {

	cc8* tableName = sqlite3_column_table_name ( statement, iCol );
	cc8* colName = sqlite3_column_origin_name ( statement, iCol );
	
	return this->GetSqlMetadata ( connection, tableName, colName );
}

//----------------------------------------------------------------//
bool USSqlColumn::GetSqlMetadata ( sqlite3* connection, cc8* tableName, cc8* colName ) {

	this->mColumnName	= colName;
	this->mTableName	= tableName;

	return this->GetSqlMetadata ( connection );
}

//----------------------------------------------------------------//
cc8* USSqlColumn::GetTableName () {
	return this->mTableName.str ();
}

//----------------------------------------------------------------//
cc8* USSqlColumn::GetTypeDecl () {
	return this->mDeclaredType.str ();
}

//----------------------------------------------------------------//
bool USSqlColumn::IsAuto () {
	return this->mIsAuto;
}

//----------------------------------------------------------------//
bool USSqlColumn::IsNullable () {
	return this->mIsNullable;
}

//----------------------------------------------------------------//
bool USSqlColumn::IsPrimaryKey () {
	return this->mIsPrimaryKey;
}

//----------------------------------------------------------------//
void USSqlColumn::Print () {

	if ( this->mTableName.size () == 0 ) {
		printf ( "UNKNOWN" );
		return;
	}

	printf ( "%s.%s: %s", this->mTableName.str (), this->mColumnName.str (), this->mDeclaredType.str ());
	
	if ( this->mCollationSequenceName.size ()) {
		printf ( " %s", this->mCollationSequenceName.str ());
	}

	if ( this->mIsNullable ) {
		printf ( " NULLABLE" );
	}
	
	if ( this->mIsPrimaryKey ) {
		printf ( " PRIMARY" );
	}
	
	if ( this->mIsAuto ) {
		printf ( " AUTO" );
	}
}

//----------------------------------------------------------------//
USSqlColumn::USSqlColumn () :
	mIsAuto ( false ),
	mColumnExists ( false ) {
}

//----------------------------------------------------------------//
USSqlColumn::~USSqlColumn () {
}
