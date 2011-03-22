// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USSqlConnection.h>
#include <uslsext/USSqlStatement.h>
#include <uslsext/USSqlRecord.h>

//================================================================//
// USSqlCell
//================================================================//

//----------------------------------------------------------------//
bool USSqlCell::Init ( sqlite3* connection, sqlite3_stmt* statement, int iCol ) {

	USSqlColumn::GetSqlMetadata ( connection, statement, iCol );
	this->mColID = iCol;
	
	return true;
}

//----------------------------------------------------------------//
void USSqlCell::Print () {

	this->USSqlColumn::Print ();
	printf ( " = " );
	this->mValue.Print ();
}

//----------------------------------------------------------------//
void USSqlCell::SetRowID ( s64 rowID ) {

	if ( this->mIsAuto ) {
		this->mValue.SetInt64 ( rowID );
	}
}

//----------------------------------------------------------------//
bool USSqlCell::SkipWrite () {

	if ( this->mIsAuto ) return true;
	if ( !this->mValue.mIsDirty ) return true;
	return false;
}

//----------------------------------------------------------------//
USSqlCell::USSqlCell () :
	mColID ( 0xffffffff ) {
}

//----------------------------------------------------------------//
USSqlCell::~USSqlCell () {
}

//================================================================//
// USSqlRecord
//================================================================//

//----------------------------------------------------------------//
void USSqlRecord::AddColumn ( cc8* name, cc8* declType ) {

	USSqlCell& cell = this->mCells [ name ];
	
	cell.mTableName = this->mName;
	cell.mColumnName = name;
	cell.mDeclaredType = declType;
	
	//USSqlColumn& col = cell;
	//col.Init ( this->mConnection, this->mName, name );
}

//----------------------------------------------------------------//
bool USSqlRecord::Affirm ( cc8* where, ... ) {

	STLString whereClause;
	STLSTRING_APPEND_VA_ARGS ( whereClause, where, where );

	STLString command;
	command.write ( "SELECT * FROM %s %s", this->mName.str (), whereClause.str ());
	
	USSqlStatement statement ( this->mConnection );
	statement.Prepare ( command );

	if ( statement.StepRow ()) {
		return this->Update ( whereClause );
	}
	s64 rowid = this->Insert ();
	if ( rowid ) {
		return this->Select ( "WHERE ROWID = %lld", rowid );
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlRecord::Check ( cc8* where, ... ) {

	STLString whereClause;
	STLSTRING_APPEND_VA_ARGS ( whereClause, where, where );
	
	STLString command;
	command.write ( "SELECT * FROM %s %s", this->mName.str (), whereClause.str ());
	
	USSqlStatement statement ( this->mConnection );
	statement.Prepare ( command );
	
	return statement.StepRow ();
}

//----------------------------------------------------------------//
void USSqlRecord::Clear () {

	this->mCells.clear ();
	this->mName = "";
	this->mConnection = 0;
}

//----------------------------------------------------------------//
bool USSqlRecord::CreateTable () {

	USSqlStatement statement ( this->mConnection );
	
	STLString command;
	
	command.write ( "CREATE TABLE IF NOT EXISTS %s ( ", this->mName.str ());
	
	cc8* delim = "";
	CellIt cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		
		command.write ( "%s%s %s", delim, cell.mColumnName.str (), cell.mDeclaredType.str ());
		delim = ", ";
	}
	
	command.write ( " )" );
	
	statement.Prepare ( command );
	bool result = statement.Step ();
	
	if ( result ) {
		cellIt = this->mCells.begin ();
		for ( ; cellIt != this->mCells.end (); ++cellIt ) {
			USSqlCell& cell = cellIt->second;
			cell.GetSqlMetadata ( this->mConnection );
		}
	}
	
	return result;
}

//----------------------------------------------------------------//
bool USSqlRecord::Delete ( cc8* where, ... ) {

	if ( !this->mConnection ) return false;
	USSqlStatement statement ( this->mConnection );
	
	STLString command;
	command.write ( "DELETE FROM %s ", this->mName.str ());
	STLSTRING_APPEND_VA_ARGS ( command, where, where );
	
	statement.Prepare ( command );
	
	return statement.Step ();
}

//----------------------------------------------------------------//
void USSqlRecord::DropColumn ( cc8* name ) {
	UNUSED ( name );
}

//----------------------------------------------------------------//
bool USSqlRecord::ExtendTable () {

	USSqlStatement statement ( this->mConnection );
	CellIt cellIt;

	cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		cell.GetSqlMetadata ( this->mConnection );
	}

	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		
		if ( !cell.mColumnExists ) {
			statement.Execute ( "ALTER TABLE %s ADD COLUMN %s %s", this->mName.str (), cell.mColumnName.str (), cell.mDeclaredType.str ());
		}
	}
	
	statement.Execute ( "COMMIT TRANSACTION" );
	
	cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		cell.GetSqlMetadata ( this->mConnection );
	}
	
	return true;
}

//----------------------------------------------------------------//
bool USSqlRecord::GetBool ( cc8* colName, bool value ) {

	int intVal = value ? 1 : 0;
	intVal = this->GetInt ( colName, intVal );
	return intVal ? true : false;
}

//----------------------------------------------------------------//
USSqlCell* USSqlRecord::GetCell ( cc8* name ) {

	if ( this->mCells.contains ( name )) {
		return &this->mCells [ name ];
	}
	
	if ( USSqlColumn::Exists ( this->mConnection, this->mName, name )) {
		USSqlCell& cell = this->mCells [ name ];
		cell.USSqlColumn::GetSqlMetadata ( this->mConnection, this->mName, name );
		return &cell;
	}
	
	return 0;
}

//----------------------------------------------------------------//
USSqlCell* USSqlRecord::GetCell ( cc8* name, int type ) {

	if ( this->mCells.contains ( name )) {
		USSqlCell& cell = this->mCells [ name ];
		if ( cell.mValue.mType == type ) {
			return &cell;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
sqlite3* USSqlRecord::GetConnection () {
	return this->mConnection;
}

//----------------------------------------------------------------//
double USSqlRecord::GetDouble ( cc8* colName, double value ) {

	USSqlCell* cell = this->GetCell ( colName, SQLITE_FLOAT );
	if ( cell ) {
		return cell->mValue.GetDouble ( value );
	}
	return value;
}

//----------------------------------------------------------------//
float USSqlRecord::GetFloat ( cc8* colName, float value ) {

	return ( float )this->GetDouble ( colName, value );
}

//----------------------------------------------------------------//
s32 USSqlRecord::GetInt ( cc8* colName, s32 value ) {

	return ( s32 )this->GetInt64 ( colName, value );
}

//----------------------------------------------------------------//
s64 USSqlRecord::GetInt64 ( cc8* colName, s64 value ) {

	USSqlCell* cell = this->GetCell ( colName, SQLITE_INTEGER );
	if ( cell ) {
		return cell->mValue.GetInt64 ( value );
	}
	return value;
}

//----------------------------------------------------------------//
cc8* USSqlRecord::GetString ( cc8* colName, cc8* value ) {

	USSqlCell* cell = this->GetCell ( colName, SQLITE_TEXT );
	if ( cell ) {
		return cell->mValue.GetString ( value );
	}
	return value;
}

//----------------------------------------------------------------//
void USSqlRecord::Init ( sqlite3* connection, cc8* tableName ) {

	this->Clear ();
	this->mName = tableName;
	this->mConnection = connection;
}

//----------------------------------------------------------------//
s64 USSqlRecord::Insert () {

	if ( !this->mConnection ) return false;
	USSqlStatement statement ( this->mConnection );
	
	STLString command;
	command.write ( "INSERT INTO %s ( ", this->mName.str ());
	
	cc8* delim;
	CellIt cellIt;
	
	delim = "";
	cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		if ( cell.SkipWrite ()) continue;
		
		command.write ( "%s%s", delim, cell.mColumnName.str ());
		delim = ", ";
	}
	
	command += " ) VALUES ( ";
	
	delim = "";
	cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		if ( cell.SkipWrite ()) continue;
		
		command.write ( "%s?", delim );
		delim = ", ";
	}
	
	command.write ( " )" );
	
	statement.Prepare ( command );
	
	cellIt = this->mCells.begin ();
	int idx = 1;
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		if ( cell.SkipWrite ()) continue;
		
		cell.mValue.Bind ( statement, idx );
		++idx;
	}
	
	bool result = statement.Step ();
	
	if ( result ) {
		return sqlite3_last_insert_rowid ( this->mConnection );
	}
	return 0;
}

//----------------------------------------------------------------//
bool USSqlRecord::InsertSelect () {

	s64 rowid = this->Insert ();
	if ( rowid ) {
		return this->Select ( "WHERE ROWID = %lld", rowid );
	}
	return false;
}

//----------------------------------------------------------------//
bool USSqlRecord::LoadCells ( sqlite3_stmt* statement ) {

	bool addCells = ( this->mCells.size () == 0 );
		
	int count = sqlite3_column_count ( statement );
	for ( int i = 0; i < count; ++i ) {
		
		cc8* collTableName = sqlite3_column_table_name ( statement, i );
		if ( strcmp ( collTableName, this->mName ) != 0 ) continue;
		
		cc8* collName = sqlite3_column_origin_name ( statement, i );
		
		if ( addCells || this->mCells.contains ( collName )) {
		
			USSqlCell& cell = this->mCells [ collName ];
			
			cell.GetSqlMetadata ( this->mConnection, statement, i );
			cell.mValue.Load ( statement, i );
		}
	}
	
	return true;
}

//----------------------------------------------------------------//
void USSqlRecord::PrintCells () {

	printf ( "Record row:\n" );

	// just the values, ma'am...
	CellIt cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		
		printf ( "\t" );
		cell.Print ();
		printf ( "\n" );
	}
}

//----------------------------------------------------------------//
void USSqlRecord::Reset () {

	this->mCells.clear ();
}

//----------------------------------------------------------------//
bool USSqlRecord::Select ( cc8* where, ... ) {

	if ( !this->mConnection ) return false;
	USSqlStatement statement ( this->mConnection );

	cc8* tableName = this->mName.str ();
	
	STLString command;
	command.write ( "SELECT * FROM %s ", tableName );
	STLSTRING_APPEND_VA_ARGS ( command, where, where );
	
	statement.Prepare ( command );
	
	if ( statement.StepRow ()) {
		this->LoadCells ( statement );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void USSqlRecord::SetBool ( cc8* colName, bool value ) {

	int intVal = value ? 1 : 0;
	this->SetInt ( colName, intVal );
}

//----------------------------------------------------------------//
void USSqlRecord::SetDouble ( cc8* colName, double value ) {

	USSqlCell* cell = this->GetCell ( colName );
	if ( cell ) {
		cell->mValue.SetDouble ( value );
	}
}

//----------------------------------------------------------------//
void USSqlRecord::SetFloat ( cc8* colName, float value ) {

	this->SetDouble ( colName, value );
}

//----------------------------------------------------------------//
void USSqlRecord::SetInt ( cc8* colName, s32 value ) {

	this->SetInt64 ( colName, value );
}

//----------------------------------------------------------------//
void USSqlRecord::SetInt64 ( cc8* colName, s64 value ) {

	USSqlCell* cell = this->GetCell ( colName );
	if ( cell ) {
		//if ( cell->mIsAuto ) return;
		cell->mValue.SetInt64 ( value );
	}
}

//----------------------------------------------------------------//
void USSqlRecord::SetNull ( cc8* colName ) {
	UNUSED ( colName );
}

//----------------------------------------------------------------//
void USSqlRecord::SetString ( cc8* colName, cc8* value ) {

	USSqlCell* cell = this->GetCell ( colName );
	if ( cell ) {
		cell->mValue.SetString ( value );
	}
}

//----------------------------------------------------------------//
bool USSqlRecord::Update ( cc8* where, ... ) {

	if ( !this->mConnection ) return false;
	USSqlStatement statement ( this->mConnection );
	
	STLString command;
	command.write ( "UPDATE %s SET ", this->mName.str ());
	
	cc8* delim = "";
	CellIt cellIt = this->mCells.begin ();
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		if ( cell.SkipWrite ()) continue;
		
		command.write ( "%s%s = ?", delim, cell.mColumnName.str ());
		delim = ", ";
	}
	
	command.write ( " " );
	STLSTRING_APPEND_VA_ARGS ( command, where, where );
	
	// prepare the statement
	statement.Prepare ( command );
	
	// bind the values
	cellIt = this->mCells.begin ();
	int idx = 1;
	for ( ; cellIt != this->mCells.end (); ++cellIt ) {
		USSqlCell& cell = cellIt->second;
		if ( cell.SkipWrite ()) continue;
		
		cell.mValue.Bind ( statement, idx );
		++idx;
	}
	
	return statement.Step ();
}

//----------------------------------------------------------------//
USSqlRecord::USSqlRecord () :
	mConnection ( 0 ) {
}

//----------------------------------------------------------------//
USSqlRecord::~USSqlRecord () {
}
