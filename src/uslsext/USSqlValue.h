// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SQLVALUE_H
#define SQLVALUE_H

#include <sqlite3.h>

//================================================================//
// USSqlValue
//================================================================//
class USSqlValue {
private:
	
	double			mFloatVal;
	s64				mIntVal;
	STLString		mTextVal;

	int mType;
	bool mIsDirty;

public:

	friend class USSqlCell;
	friend class USSqlRecord;

	//----------------------------------------------------------------//
	bool			Bind				( sqlite3_stmt* statement, int idx );
	double			GetDouble			( double value );
	float			GetFloat			( float value );
	s32				GetInt				( s32 value );
	s64				GetInt64			( s64 value );
	cc8*			GetString			( cc8* value );
	bool			IsDirty				();
	bool			IsNull				();
	bool			Load				( sqlite3_stmt* statement, int iCol );
	void			Print				();
	USSqlValue&		SetDouble			( double value );
	USSqlValue&		SetFloat			( float value );
	USSqlValue&		SetInt				( s32 value );
	USSqlValue&		SetInt64			( s64 value );
	USSqlValue&		SetNull				();
	USSqlValue&		SetString			( cc8* value );
					USSqlValue			();
					USSqlValue			( double value );
					USSqlValue			( float value );
					USSqlValue			( s32 value );
					USSqlValue			( s64 value );
					USSqlValue			( cc8* value );
					~USSqlValue			();
};

#endif
