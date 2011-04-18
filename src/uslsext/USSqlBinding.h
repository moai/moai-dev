// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef SQLBINDING_H
#define SQLBINDING_H

#include <sqlite3.h>

class USSqlRecord;
class USSqlValue;

//================================================================//
// USSqlBinding
//================================================================//
class USSqlBinding {
public:

	//----------------------------------------------------------------//
	bool			SqlAffirmRecord			( sqlite3* connection, cc8* where, ... );
	bool			SqlCreateTable			( sqlite3* connection );
	virtual void	SqlCustomizeTable		( sqlite3* connection );
	bool			SqlDeleteRecord			( sqlite3* connection, cc8* where, ... );
	bool			SqlDropTable			( sqlite3* connection );
	virtual void	SqlFormatRecord			( USSqlRecord& record ) = 0;
	s64				SqlInsertRecord			( sqlite3* connection );
	bool			SqlInsertSelectRecord	( sqlite3* connection );
	void			SqlLoadCells			( sqlite3* connection, sqlite3_stmt* statement );
	bool			SqlSelectRecord			( sqlite3* connection, cc8* where, ... );
	virtual void	SqlSerializeFromRecord	( USSqlRecord& record ) = 0;
	virtual void	SqlSerializeToRecord	( USSqlRecord& record ) = 0;
	virtual cc8*	SqlTableName			() const = 0;
	bool			SqlUpdateRecord			( sqlite3* connection, cc8* where, ... );
					USSqlBinding			();
	virtual			~USSqlBinding			();
};

#endif
