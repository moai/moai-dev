// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USSqlBinding.h>
#include <uslsext/USSqlConnection.h>
#include <uslsext/USSqlStatement.h>
#include <uslsext/USSqlRecord.h>
#include <uslsext/USSqlValue.h>

//================================================================//
// USSqlBinding
//================================================================//

//----------------------------------------------------------------//
bool USSqlBinding::SqlAffirmRecord ( sqlite3* connection, cc8* where, ... ) {

	STLSTRING_FROM_VA_ARGS ( whereClause, where, where );

	USSqlRecord record;
	record.Init ( connection, this->SqlTableName ());
	this->SqlSerializeToRecord ( record );
	
	bool result = false;
	
	if ( record.Check ( whereClause )) {
		result = record.Update ( whereClause );
	}
	else {
		result = ( record.Insert () != 0 );
	}
	
	if ( result ) {
		result = record.Select ( whereClause );
		if ( result ) {
			this->SqlSerializeFromRecord ( record );
		}
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlBinding::SqlCreateTable ( sqlite3* connection ) {

	USSqlRecord record;
	record.Init ( connection, this->SqlTableName ());
	this->SqlFormatRecord ( record );
	bool result = record.CreateTable ();
	if ( result ) {
		this->SqlCustomizeTable ( connection );
	}
	return result;
}

//----------------------------------------------------------------//
void USSqlBinding::SqlCustomizeTable ( sqlite3* connection ) {
	UNUSED ( connection );
}

//----------------------------------------------------------------//
bool USSqlBinding::SqlDeleteRecord ( sqlite3* connection, cc8* where, ... ) {

	STLSTRING_FROM_VA_ARGS ( whereClause, where, where );

	USSqlRecord record;
	record.Init ( connection, this->SqlTableName () );
	this->SqlSerializeToRecord ( record );
	return record.Delete ( whereClause );
}

//----------------------------------------------------------------//
bool USSqlBinding::SqlDropTable ( sqlite3* connection ) {
	
	USSqlStatement statement ( connection );
	
	STLString command;
	command.write ( "DROP TABLE IF EXISTS %s", this->SqlTableName ());
	statement.Prepare ( command );
	return statement.Step ();
}

//----------------------------------------------------------------//
s64 USSqlBinding::SqlInsertRecord ( sqlite3* connection ) {

	USSqlRecord record;
	record.Init ( connection, this->SqlTableName () );
	this->SqlSerializeToRecord ( record );
	return record.Insert ();
}

//----------------------------------------------------------------//
bool USSqlBinding::SqlInsertSelectRecord ( sqlite3* connection ) {

	s64 rowid = this->SqlInsertRecord ( connection );
	if ( rowid ) {
		return this->SqlSelectRecord ( connection, "WHERE ROWID = %lld", rowid );
	}
	return false;
}

//----------------------------------------------------------------//
void USSqlBinding::SqlLoadCells ( sqlite3* connection, sqlite3_stmt* statement ) {

	USSqlRecord record;
	record.Init ( connection, this->SqlTableName ());
	record.LoadCells ( statement );
	this->SqlSerializeFromRecord ( record );
}

//----------------------------------------------------------------//
bool USSqlBinding::SqlSelectRecord ( sqlite3* connection, cc8* where, ... ) {

	STLSTRING_FROM_VA_ARGS ( whereClause, where, where );

	USSqlRecord record;
	record.Init ( connection, this->SqlTableName ());
	if ( record.Select ( whereClause )) {
		this->SqlSerializeFromRecord ( record );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlBinding::SqlUpdateRecord ( sqlite3* connection, cc8* where, ... ) {

	STLSTRING_FROM_VA_ARGS ( whereClause, where, where );

	USSqlRecord record;
	record.Init ( connection, this->SqlTableName ());
	this->SqlSerializeToRecord ( record );
	return record.Update ( whereClause );
}

//----------------------------------------------------------------//
USSqlBinding::USSqlBinding () {
}

//----------------------------------------------------------------//
USSqlBinding::~USSqlBinding () {
}
