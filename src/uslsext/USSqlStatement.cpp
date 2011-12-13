// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USSqlStatement.h>

//================================================================//
// USSqlStatement
//================================================================//

SUPPRESS_EMPTY_FILE_WARNING
#if USE_SQL

//----------------------------------------------------------------//
bool USSqlStatement::BindDouble ( u32 idx, double value ) {

	if ( this->mStatement ) {
		return ( sqlite3_bind_double ( this->mStatement, idx, value ) == SQLITE_OK );
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlStatement::BindFloat ( u32 idx, float value ) {

	return this->BindDouble ( idx, value );
}

//----------------------------------------------------------------//
bool USSqlStatement::BindInt ( u32 idx, int value ) {

	if ( this->mStatement ) {
		return ( sqlite3_bind_int ( this->mStatement, idx, value ) == SQLITE_OK );
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlStatement::BindInt64 ( u32 idx, s64 value ) {

	if ( this->mStatement ) {
		return ( sqlite3_bind_int64 ( this->mStatement, idx, value ) == SQLITE_OK );
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlStatement::BindString ( u32 idx, cc8* value ) {

	if ( this->mStatement ) {
		return ( sqlite3_bind_text ( this->mStatement, idx, value, -1, SQLITE_TRANSIENT ) == SQLITE_OK );
	}
	return false;
}

//----------------------------------------------------------------//
void USSqlStatement::Clear () {

	if ( this->mOwns && this->mStatement ) {
		sqlite3_finalize ( this->mStatement );
	}
	this->mStatement = 0;
	this->mOwns = false;
}

//----------------------------------------------------------------//
bool USSqlStatement::CreateIndex ( cc8* table, cc8* column ) {

	return this->Execute ( "CREATE INDEX IF NOT EXISTS idx_%s_%s ON %s ( %s )", table, column, table, column );
}

//----------------------------------------------------------------//
bool USSqlStatement::Execute ( cc8* statement, ... ) {

	this->Clear ();
	assert ( this->mConnection );
	
	va_list args;
	va_start ( args, statement );
	char buffer [ 1024 ];
	vsprintf ( buffer, statement, args );
	va_end ( args );
	
	if ( sqlite3_prepare_v2 ( this->mConnection, buffer, -1, &this->mStatement, NULL ) == SQLITE_OK ) {
		this->mOwns = true;
		if ( !this->Step ()) return false;
		return true;
	}
	
	printf ( "SQLite Error: %s\n", sqlite3_errmsg ( this->mConnection ));
	this->mStatement = 0;
	return false;
}

//----------------------------------------------------------------//
cc8* USSqlStatement::GetColumnName ( u32 iCol ) {

	assert ( this->mStatement );
	return sqlite3_column_origin_name ( this->mStatement, iCol );
}

//----------------------------------------------------------------//
cc8* USSqlStatement::GetColumnTableName ( u32 iCol ) {

	assert ( this->mStatement );
	return sqlite3_column_table_name ( this->mStatement, iCol );
}

//----------------------------------------------------------------//
float USSqlStatement::GetFloat ( u32 iCol, float value ) {

	return ( float )this->GetDouble ( iCol, value );
}

//----------------------------------------------------------------//
double USSqlStatement::GetDouble ( u32 iCol, double value ) {

	if ( sqlite3_column_type ( this->mStatement, iCol ) == SQLITE_FLOAT ) {
		return sqlite3_column_double ( this->mStatement, iCol );
	}
	return value;
}

//----------------------------------------------------------------//
int USSqlStatement::GetInt ( u32 iCol, int value ) {

	if ( sqlite3_column_type ( this->mStatement, iCol ) == SQLITE_INTEGER ) {
		return sqlite3_column_int ( this->mStatement, iCol );
	}
	return value;
}

//----------------------------------------------------------------//
s64 USSqlStatement::GetInt64 ( u32 iCol, s64 value ) {

	if ( sqlite3_column_type ( this->mStatement, iCol ) == SQLITE_INTEGER ) {
		return sqlite3_column_int64 ( this->mStatement, iCol );
	}
	return value;
}

//----------------------------------------------------------------//
cc8* USSqlStatement::GetString ( u32 iCol, cc8* value ) {

	if ( sqlite3_column_type ( this->mStatement, iCol ) == SQLITE_TEXT ) {
		return ( cc8* )sqlite3_column_text ( this->mStatement, iCol );
	}
	return value;
}

//----------------------------------------------------------------//
bool USSqlStatement::Prepare ( cc8* statement, ... ) {

	this->Clear ();
	assert ( this->mConnection );
	
	va_list args;
	va_start ( args, statement );
	char buffer [ 1024 ];
	vsprintf ( buffer, statement, args );
	va_end ( args );
	
	if ( sqlite3_prepare_v2 ( this->mConnection, buffer, -1, &this->mStatement, NULL ) == SQLITE_OK ) {
		this->mOwns = true;
		return true;
	}
	
	printf ( "SQLite Error: %s\n", sqlite3_errmsg ( this->mConnection ));
	this->mStatement = 0;
	return false;
}

//----------------------------------------------------------------//
void USSqlStatement::PrintColumnInfo () {

	if ( !this->mStatement ) return;

	printf ( "statement:\n" );

	int count = sqlite3_column_count ( this->mStatement );
	for ( int i = 0; i < count; ++i ) {
	
		USSqlColumn colInfo;
		colInfo.GetSqlMetadata ( this->mConnection, this->mStatement, i );
		
		printf ( "\tcolumn %d: ", i );
		colInfo.Print ();
		printf ( "\n" );
	}
}

//----------------------------------------------------------------//
void USSqlStatement::Reset () {

	if ( !this->mStatement ) return;
	sqlite3_reset ( this->mStatement );
	//this->mColumns.clear ();
}

//----------------------------------------------------------------//
void USSqlStatement::Set ( sqlite3_stmt* statement ) {

	assert ( this->mConnection );

	this->Clear ();
	this->mStatement = statement;
	this->mOwns = false;
}

//----------------------------------------------------------------//
void USSqlStatement::SetConnection ( sqlite3 *connection ) {

	this->Clear ();
	this->mConnection = connection;
}

//----------------------------------------------------------------//
bool USSqlStatement::Step () {

	if ( this->mStatement ) {
	
		int result = sqlite3_step ( this->mStatement );
	
		if (( result == SQLITE_ROW ) || ( result == SQLITE_DONE )) {
			return true;
		}
		printf ( "SQLite Error: %s\n", sqlite3_errmsg ( this->mConnection ));
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlStatement::StepRow () {

	if ( this->mStatement ) {
	
		int result = sqlite3_step ( this->mStatement );
	
		if ( result == SQLITE_ROW ) {
			return true;
		}
		
		if ( result != SQLITE_DONE ) {
			printf ( "SQLite Error: %s\n", sqlite3_errmsg ( this->mConnection ));
		}
	}
	return false;
}

//----------------------------------------------------------------//
void USSqlStatement::Take ( USSqlStatement& statement ) {
	
	this->mOwns = statement.mOwns;
	this->mStatement = statement.mStatement;
	this->mConnection = statement.mConnection;
	
	(( USSqlStatement& )statement ).mOwns = false;
	(( USSqlStatement& )statement ).mStatement = 0;
	(( USSqlStatement& )statement ).mConnection = 0;
};

//----------------------------------------------------------------//
USSqlStatement::USSqlStatement () :
	mOwns ( false ),
	mStatement ( 0 ),
	mConnection ( 0 ) {
}

//----------------------------------------------------------------//
USSqlStatement::USSqlStatement ( sqlite3* connection ) :
	mOwns ( false ),
	mStatement ( 0 ),
	mConnection ( connection ) {
}

//----------------------------------------------------------------//
USSqlStatement::USSqlStatement ( sqlite3* connection, cc8* statement ) :
	mOwns ( false ),
	mStatement ( 0 ),
	mConnection ( connection ) {

	this->Prepare ( statement );
}

//----------------------------------------------------------------//
USSqlStatement::USSqlStatement ( sqlite3* connection, sqlite3_stmt* statement ) :
	mOwns ( false ),
	mStatement ( statement ),
	mConnection ( connection ) {
}

//----------------------------------------------------------------//
USSqlStatement::USSqlStatement ( const USSqlStatement& statement ) {
	this->Take (( USSqlStatement& )statement );
}

//----------------------------------------------------------------//
USSqlStatement::~USSqlStatement () {

	this->Clear ();
}

#endif
