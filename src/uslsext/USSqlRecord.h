// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SQLRECORD_H
#define SQLRECORD_H

#include <uslsext/USSqlColumn.h>
#include <uslsext/USSqlValue.h>
#include <sqlite3.h>

//================================================================//
// USSqlCell
//================================================================//
class USSqlCell :
	public USSqlColumn {
protected:
	
	u32				mColID;
	USSqlValue		mValue;

	//----------------------------------------------------------------//
	bool			Init				( sqlite3* connection, sqlite3_stmt* statement, int iCol );
	void			Print				();
	void			SetRowID			( s64 rowID );
	bool			SkipWrite			();

public:

	friend class USSqlRecord;

	//----------------------------------------------------------------//
					USSqlCell			();
	virtual			~USSqlCell			();
};

//================================================================//
// USSqlRecord
//================================================================//
class USSqlRecord {
private:

	sqlite3*		mConnection;
	STLString		mName;

	typedef STLMap < string, USSqlCell >::iterator CellIt;
	STLMap < string, USSqlCell > mCells;

	//----------------------------------------------------------------//
	USSqlCell*		GetCell				( cc8* name );
	USSqlCell*		GetCell				( cc8* name, int type );

public:

	friend class USSqlConnection;

	//----------------------------------------------------------------//
	void			AddColumn			( cc8* name, cc8* declType );
	bool			Affirm				( cc8* where, ... );
	bool			Check				( cc8* where, ... );
	void			Clear				();
	bool			CreateTable			();
	bool			Delete				( cc8* where, ... );
	void			DropColumn			( cc8* name );
	bool			ExtendTable			();
	bool			GetBool				( cc8* colName, bool value );
	double			GetDouble			( cc8* colName, double value );
	float			GetFloat			( cc8* colName, float value );
	sqlite3*		GetConnection		();
	s32				GetInt				( cc8* colName, s32 value );
	s64				GetInt64			( cc8* colName, s64 value );
	cc8*			GetString			( cc8* colName, cc8* value );
	void			Init				( sqlite3* connection, cc8* tableName );
	s64				Insert				();
	bool			InsertSelect		();
	bool			LoadCells			( sqlite3_stmt* statement );
	void			PrintCells			();
	void			Reset				();
	bool			Select				( cc8* where, ... );
	void			SetBool				( cc8* colName, bool value );
	void			SetDouble			( cc8* colName, double value );
	void			SetFloat			( cc8* colName, float value );
	void			SetInt				( cc8* colName, s32 value );
	void			SetInt64			( cc8* colName, s64 value );
	void			SetNull				( cc8* colName );
	void			SetString			( cc8* colName, cc8* value );
	bool			Update				( cc8* where, ... );
					USSqlRecord			();
	virtual			~USSqlRecord		();
};

#endif
