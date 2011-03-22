// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USSqlConnection.h>
#include <uslsext/USSqlValue.h>

//================================================================//
// USSqlValue
//================================================================//

//----------------------------------------------------------------//
bool USSqlValue::Bind ( sqlite3_stmt* statement, int idx ) {

	int result = SQLITE_ERROR;

	switch ( this->mType ) {
		case SQLITE_INTEGER: {
			result = sqlite3_bind_int64 ( statement, idx, this->mIntVal );
			break;
		}
		case SQLITE_FLOAT: {
			result = sqlite3_bind_double ( statement, idx, this->mFloatVal );
			break;
		}
		case SQLITE_BLOB: {
			assert ( false ); // coming soon!
			break;
		}
		case SQLITE_NULL: {
			result = sqlite3_bind_null ( statement, idx );
			break;
		}
		case SQLITE_TEXT: {
			result = sqlite3_bind_text ( statement, idx,this->mTextVal, -1, 0 );
			break;
		}
	}
	
	return ( result == SQLITE_OK );
}

//----------------------------------------------------------------//
double USSqlValue::GetDouble ( double value ) {

	if ( this->mType == SQLITE_FLOAT ) {
		return this->mFloatVal;
	}
	return value;
}

//----------------------------------------------------------------//
float USSqlValue::GetFloat ( float value ) {
	return ( float )this->GetDouble ( value );
}

//----------------------------------------------------------------//
s32 USSqlValue::GetInt ( s32 value ) {
	return ( s32 )this->GetInt64 ( value );
}

//----------------------------------------------------------------//
s64 USSqlValue::GetInt64 ( s64 value ) {

	if ( this->mType == SQLITE_INTEGER ) {
		return this->mIntVal;
	}
	return value;
}

//----------------------------------------------------------------//
cc8* USSqlValue::GetString ( cc8* value ) {

	if ( this->mType == SQLITE_TEXT ) {
		return this->mTextVal.str ();
	}
	return value;
}

//----------------------------------------------------------------//
bool USSqlValue::IsDirty () {

	return this->mIsDirty;
}

//----------------------------------------------------------------//
bool USSqlValue::IsNull () {

	return ( this->mType == SQLITE_NULL );
}

//----------------------------------------------------------------//
bool USSqlValue::Load ( sqlite3_stmt* statement, int iCol ) {

	this->mType = sqlite3_column_type ( statement, iCol );

	switch ( this->mType ) {
		case SQLITE_INTEGER: {
			this->mIntVal = sqlite3_column_int64 ( statement, iCol );
			break;
		}
		case SQLITE_FLOAT: {
			this->mFloatVal = sqlite3_column_double ( statement, iCol );
			break;
		}
		case SQLITE_BLOB: {
			assert ( false ); // coming soon!
			break;
		}
		case SQLITE_NULL: {
			break;
		}
		case SQLITE_TEXT: {
			this->mTextVal = ( cc8* )sqlite3_column_text ( statement, iCol );
			break;
		}
	}
	
	this->mIsDirty = false;
	return true;
}

//----------------------------------------------------------------//
void USSqlValue::Print () {

	switch ( this->mType ) {
		case SQLITE_INTEGER: {
			printf ( "%d", ( int )this->mIntVal );
			break;
		}
		case SQLITE_FLOAT: {
			printf ( "%f", ( float )this->mFloatVal );
			break;
		}
		case SQLITE_BLOB: {
			break;
		}
		case SQLITE_NULL: {
			printf ( "NULL" );
			break;
		}
		case SQLITE_TEXT: {
			printf ( "%s", this->mTextVal.str ());
			break;
		}
	}
}

//----------------------------------------------------------------//
USSqlValue& USSqlValue::SetDouble ( double value ) {

	this->mType = SQLITE_FLOAT;
	this->mFloatVal = value;
	this->mIsDirty = true;
	
	return *this;
}

//----------------------------------------------------------------//
USSqlValue& USSqlValue::SetFloat ( float value ) {
	return this->SetDouble ( value );
}

//----------------------------------------------------------------//
USSqlValue& USSqlValue::SetInt ( s32 value ) {
	return this->SetInt64 ( value );
}

//----------------------------------------------------------------//
USSqlValue& USSqlValue::SetInt64 ( s64 value ) {

	this->mType = SQLITE_INTEGER;
	this->mIntVal = value;
	this->mIsDirty = true;
	
	return* this;
}

//----------------------------------------------------------------//
USSqlValue& USSqlValue::SetNull () {

	this->mType = SQLITE_NULL;
	this->mIsDirty = true;
	return *this;
}

//----------------------------------------------------------------//
USSqlValue& USSqlValue::SetString ( cc8* value ) {

	this->mType = SQLITE_TEXT;
	this->mTextVal = value;
	this->mIsDirty = true;
	
	return* this;
}

//----------------------------------------------------------------//
USSqlValue::USSqlValue () {
	this->SetNull ();
}

//----------------------------------------------------------------//
USSqlValue::USSqlValue ( double value ) {
	this->SetDouble ( value );
}

//----------------------------------------------------------------//
USSqlValue::USSqlValue ( float value ) {
	this->SetFloat ( value );
}

//----------------------------------------------------------------//
USSqlValue::USSqlValue ( s32 value ) {
	this->SetInt ( value );
}

//----------------------------------------------------------------//
USSqlValue::USSqlValue  ( s64 value ) {
	this->SetInt64 ( value );
}

//----------------------------------------------------------------//
USSqlValue::USSqlValue ( cc8* value ) {
	this->SetString ( value );
}

//----------------------------------------------------------------//
USSqlValue::~USSqlValue () {
}

