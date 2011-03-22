// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USFileCache.h>
#include <uslsext/USSqlBinding.h>
#include <uslsext/USSqlRecord.h>
#include <uslsext/USSqlStatement.h>
#include <uslsext/USSqlValue.h>

#define SQL_PARTITIONS "partitions"
#define SQL_PARTITIONS_PRIORITY "priority"
#define SQL_PARTITIONS_MAX_SIZE "maxSize"
#define SQL_PARTITIONS_SIZE "size"

//================================================================//
// USDataStorePartition
//================================================================//
class USDataStorePartition :
	public USSqlBinding {
public:

	int mPriority;
	s64 mMaxSize;
	s64 mSize;

	//-------------------------------------------------------------------//
	USDataStorePartition () :
		mPriority ( 0 ),
		mMaxSize ( 0 ),
		mSize ( 0 ) {
	}

	//-------------------------------------------------------------------//
	~USDataStorePartition () {
	}

	//-------------------------------------------------------------------//
	void SqlFormatRecord ( USSqlRecord& record ) {
		
		record.AddColumn ( SQL_PARTITIONS_PRIORITY, "INTEGER UNIQUE NOT NULL" );
		record.AddColumn ( SQL_PARTITIONS_MAX_SIZE, "INTEGER NOT NULL" );
		record.AddColumn ( SQL_PARTITIONS_SIZE, "INTEGER NOT NULL" );
	}

	//-------------------------------------------------------------------//
	void SqlSerializeFromRecord ( USSqlRecord& record ) {
		
		this->mPriority		= record.GetInt ( SQL_PARTITIONS_PRIORITY, 0 );
		this->mMaxSize		= record.GetInt64 ( SQL_PARTITIONS_MAX_SIZE, 0 );
		this->mSize			= record.GetInt64 ( SQL_PARTITIONS_SIZE, 0 );
	}

	//-------------------------------------------------------------------//
	void SqlSerializeToRecord ( USSqlRecord& record ) {
		
		record.SetInt		( SQL_PARTITIONS_PRIORITY, this->mPriority );
		record.SetInt64		( SQL_PARTITIONS_MAX_SIZE, this->mMaxSize );
		record.SetInt64		( SQL_PARTITIONS_SIZE, this->mSize );
	}

	//-------------------------------------------------------------------//
	cc8* SqlTableName () const {
		return SQL_PARTITIONS;
	}
};

#define SQL_ENTRIES "entries"
#define SQL_ENTRIES_STORE_KEY "storeKey"
#define SQL_ENTRIES_SIZE "size"
#define SQL_ENTRIES_PRIORITY "priority"
#define SQL_ENTRIES_TIME_USED "timeUsed"
#define SQL_ENTRIES_TOUCHED "touched"

//================================================================//
// USDataStoreEntry
//================================================================//
class USDataStoreEntry :
	public USSqlBinding {
public:

	s64			mTimeUsed;
	STLString	mStoreKey;
	s32			mSize;
	s32			mPriority;
	bool		mTouched;

	//-------------------------------------------------------------------//
	USDataStoreEntry () :
		mTimeUsed ( 0 ),
		mStoreKey ( "" ),
		mSize ( 0 ),
		mPriority ( 0 ),
		mTouched ( false ) {
	}

	//-------------------------------------------------------------------//
	~USDataStoreEntry () {
	}

	//-------------------------------------------------------------------//
	void SqlFormatRecord ( USSqlRecord& record ) {
		
		record.AddColumn ( SQL_ENTRIES_STORE_KEY, "TEXT PRIMARY KEY NOT NULL" );
		record.AddColumn ( SQL_ENTRIES_SIZE, "INTEGER NOT NULL" );
		record.AddColumn ( SQL_ENTRIES_PRIORITY, "INTEGER NOT NULL" );
		record.AddColumn ( SQL_ENTRIES_TIME_USED, "INTEGER NOT NULL" );
		record.AddColumn ( SQL_ENTRIES_TOUCHED, "INTEGER NOT NULL" );
	}

	//-------------------------------------------------------------------//
	void SqlSerializeFromRecord ( USSqlRecord& record ) {
	
		this->mStoreKey		= record.GetString ( SQL_ENTRIES_STORE_KEY, "" );
		this->mSize			= record.GetInt ( SQL_ENTRIES_SIZE, 0 );
		this->mPriority		= record.GetInt ( SQL_ENTRIES_PRIORITY, 0 );
		this->mTimeUsed		= record.GetInt64 ( SQL_ENTRIES_TIME_USED, 0 );
		this->mTouched		= record.GetBool ( SQL_ENTRIES_TOUCHED, false );
	}

	//-------------------------------------------------------------------//
	void SqlSerializeToRecord ( USSqlRecord& record ) {
		
		record.SetString	( SQL_ENTRIES_STORE_KEY, this->mStoreKey );
		record.SetInt		( SQL_ENTRIES_SIZE, this->mSize );
		record.SetInt		( SQL_ENTRIES_PRIORITY, this->mPriority );
		record.SetInt64		( SQL_ENTRIES_TIME_USED, ( s64 )time ( 0 ));
		record.SetBool		( SQL_ENTRIES_TOUCHED, this->mTouched );
	}

	//-------------------------------------------------------------------//
	cc8* SqlTableName () const {
		return SQL_ENTRIES;
	}
};

#define SQL_DATAREF "dataRef"
#define SQL_DATAREF_ID "refId"
#define SQL_DATAREF_REF_KEY "refKey"
#define SQL_DATAREF_STORE_KEY "storeKey"

//================================================================//
// USDataStoreRef
//================================================================//
class USDataStoreRef :
	public USSqlBinding {
public:

	s64 mID;
	STLString mRefKey;
	STLString mStoreKey;

	//-------------------------------------------------------------------//
	USDataStoreRef () {
	}

	//-------------------------------------------------------------------//
	~USDataStoreRef () {
	}

	//-------------------------------------------------------------------//
	void SqlFormatRecord ( USSqlRecord& record ) {
		
		record.AddColumn ( SQL_DATAREF_ID, "INTEGER PRIMARY KEY AUTOINCREMENT" );
		record.AddColumn ( SQL_DATAREF_REF_KEY, "TEXT NOT NULL" );
		record.AddColumn ( SQL_DATAREF_STORE_KEY, "TEXT NOT NULL" );
	}

	//-------------------------------------------------------------------//
	void SqlSerializeFromRecord ( USSqlRecord& record ) {
		
		this->mID			= record.GetInt64 ( SQL_DATAREF_ID, 0 );
		this->mRefKey		= record.GetString ( SQL_DATAREF_REF_KEY, "" );
		this->mStoreKey		= record.GetString ( SQL_DATAREF_STORE_KEY, "" );
	}

	//-------------------------------------------------------------------//
	void SqlSerializeToRecord ( USSqlRecord& record ) {
		
		record.SetInt64		( SQL_DATAREF_ID, this->mID );
		record.SetString	( SQL_DATAREF_REF_KEY, this->mRefKey );
		record.SetString	( SQL_DATAREF_STORE_KEY, this->mStoreKey );
	}

	//-------------------------------------------------------------------//
	cc8* SqlTableName () const {
		return SQL_DATAREF;
	}
};

//================================================================//
// USFileCache
//================================================================//

//-------------------------------------------------------------------//
u32 USFileCache::AffirmFile ( cc8* name, const void* data, u32 size, int priority ) {

	if ( !( name && data && size )) return MISSING_PARAM;
	if ( this->BlessFile ( name, priority ) == OK ) return OK;

	// make room for the file
	u32 result = this->Alloc ( priority, size );
	if ( result ) {
		return result;
	}
	
	// start a transaction
	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	// save out the file
	STLString path = this->GetFilePath ( name );
	USFileStream file;
	file.OpenWrite ( path );
	file.WriteBytes ( data, size );
	
	// update the entry
	USDataStoreEntry entry;
	entry.mStoreKey = name;
	entry.mSize = size;
	entry.mPriority = priority;
	entry.mTouched = false;
	entry.SqlAffirmRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, name );
	
	// commit the transaction
	statement.Execute ( "COMMIT TRANSACTION" );
	
	return OK;
}

//-------------------------------------------------------------------//
u32 USFileCache::AffirmFileEntry ( cc8* name, u32 size, int priority ) {

	if ( !( name && size )) return MISSING_PARAM;
	if ( this->BlessFile ( name, priority ) == OK ) return OK;

	// make room for the file
	u32 result = this->Alloc ( priority, size );
	if ( result ) {
		return result;
	}
	
	// start a transaction
	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	// update the entry
	USDataStoreEntry entry;
	entry.mStoreKey = name;
	entry.mSize = size;
	entry.mPriority = priority;
	entry.mTouched = false;
	entry.SqlAffirmRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, name );
	
	// commit the transaction
	statement.Execute ( "COMMIT TRANSACTION" );
	
	return OK;
}

//-------------------------------------------------------------------//
u32 USFileCache::AffirmPartition ( int priority, u32 maxSize ) {

	USSqlValue param;

	USDataStorePartition partition;
	bool exists = partition.SqlSelectRecord (
		this->mConnection,
		"WHERE %s = %d",
		SQL_PARTITIONS_PRIORITY,
		priority
	);
	
	// no change; bail
	if ( exists && ( partition.mMaxSize == maxSize )) return OK;
	
	// update the size
	if ( maxSize < partition.mSize ) {
		return PARTITION_FULL;
	}
	partition.mMaxSize = maxSize;
	
	// only needed if partition is new
	if ( exists == false ) {
		partition.mPriority = priority;
		partition.mSize = 0;
	}
	
	// affirm
	partition.SqlAffirmRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, partition.mPriority );
	
	return OK;
}

//-------------------------------------------------------------------//
u32 USFileCache::AffirmRef ( cc8* refKey, cc8* storeKey ) {

	// select the dataRef that matches refKey/storeKey
	USSqlStatement statement(this->mConnection);
	statement.Prepare (
		"SELECT * FROM %s WHERE %s = '%s' AND %s = '%s'",
		SQL_DATAREF,
		SQL_DATAREF_REF_KEY,
		refKey,
		SQL_DATAREF_STORE_KEY,
		storeKey
	);

	// bail if entry exists
	if ( statement.StepRow ()) return OK;
	
	// add a new dataref
	USDataStoreRef ref;
	ref.mRefKey = refKey;
	ref.mStoreKey = storeKey;
	ref.SqlInsertRecord ( this->mConnection );
	
	return OK;
}

//-------------------------------------------------------------------//
u32 USFileCache::Alloc ( int priority, u32 size ) {

	USSqlValue param;

	USDataStorePartition partition;
	bool result = partition.SqlSelectRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, priority );
	
	if ( result == false ) return PARTITION_UNKNOWN;
	if (( partition.mSize + size ) <= partition.mMaxSize ) {
		partition.mSize += size;
		partition.SqlUpdateRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, priority );
		return OK;
	}
	
	// select all the entries == priority
	// sort ascending by last time used
	// don't delete anything w/ a ref count
	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	statement.Prepare (
		"SELECT %s.* FROM %s WHERE %s.%s = %d AND %s.%s NOT IN ( SELECT %s FROM %s ) ORDER BY %s ASC",
		SQL_ENTRIES,
		SQL_ENTRIES,
		
		SQL_ENTRIES,
		SQL_ENTRIES_PRIORITY,
		priority,
		
		SQL_ENTRIES,
		SQL_ENTRIES_STORE_KEY,
		
		SQL_DATAREF_STORE_KEY,
		SQL_DATAREF,
		SQL_ENTRIES_TIME_USED
	);
	
	u32 freeSpace = 0;
	int newPriority = priority - 1;
	
	// step through each entry and make room
	while ( statement.StepRow ()) {
		
		// get the entry
		USDataStoreEntry entry;
		entry.SqlLoadCells ( this->mConnection, statement );
		
		// see if we can make room
		if ( newPriority < 0 ) {
			entry.SqlDeleteRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, entry.mStoreKey.str ());
		}
		else {
			if ( this->Alloc ( newPriority, entry.mSize ) != OK ) break;
			entry.mPriority = newPriority;
			entry.SqlUpdateRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, entry.mStoreKey.str ());
		}
		
		freeSpace += entry.mSize;
		
		// if the partition is down to size, we're done
		if ( freeSpace >= size ) break;
	}
	
	partition.mSize -= freeSpace;
	
	if ( freeSpace >= size ) {
		partition.mSize += size;
	}
	
	// store the partitions
	partition.SqlUpdateRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, partition.mPriority );
	
	statement.Execute ( "COMMIT TRANSACTION" );
	
	return ( freeSpace >= size ) ? OK : PARTITION_FULL;
}

//-------------------------------------------------------------------//
u32 USFileCache::BlessFile ( cc8* name, int priority ) {

	USSqlValue param;
	USDataStoreEntry entry;
	
	// bail if no such entry
	if ( !entry.SqlSelectRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, name )) {
		return ENRTY_UNKNOWN;
	}
	
	// bail if missing file
	STLString filePath = this->GetFilePath ( entry.mStoreKey );
	if ( USFileSys::CheckFileExists ( filePath ) == false ) {
		this->DeleteEntry ( entry );
		return ENRTY_UNKNOWN;
	}

	// We want to go ahead and update the priority, even if it wasn't touched,
	// so continue (and explicity make sure it's not touched)

	// untouch the entry
	entry.mTouched = false;

	// just update the entry and bail if there's nothing to change
	if ( entry.mPriority == priority ) {
		entry.SqlUpdateRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, name );
		return OK;
	}
	
	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	// try to change priority...
	u32 result = OK;
	if ( entry.mPriority != priority ) {
	
		result = this->Alloc ( priority, entry.mSize );
		if ( result == OK ) {
		
			// select the partitions
			USDataStorePartition partition;
			
			partition.SqlSelectRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, entry.mPriority );
			partition.mSize -= entry.mSize;
			partition.SqlUpdateRecord  ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, entry.mPriority );
			
			partition.SqlSelectRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, priority );
			partition.mSize += entry.mSize;
			partition.SqlUpdateRecord  ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, priority );
			
			entry.mPriority = priority;
		}
	}
	
	entry.SqlUpdateRecord  ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, entry.mStoreKey.str ());
	statement.Execute ( "COMMIT TRANSACTION" );
	
	return result;
}

//-------------------------------------------------------------------//
void USFileCache::Close () {

	this->mConnection.Close ();
}

//-------------------------------------------------------------------//
void USFileCache::Create () {

	USFileSys::DeleteDirectory ( this->mCacheFolderPath );
	USFileSys::AffirmPath ( this->mCacheFolderPath );
	
	USFileSys::DeleteFile ( this->mCacheFilename );
	this->mConnection.Open ( this->mCacheFilename );
	
	USDataStorePartition partition;
	partition.SqlCreateTable ( this->mConnection );
	
	USDataStoreRef dataRef;
	dataRef.SqlCreateTable ( this->mConnection );
	
	USDataStoreEntry entry;
	entry.SqlCreateTable ( this->mConnection );
}

//-------------------------------------------------------------------//
void USFileCache::CurseFile ( cc8* filename ) {
	
	USSqlValue param;
	USDataStoreEntry entry;
	bool result = entry.SqlSelectRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, filename );
	if ( result == false ) {
		return;
	}
	
	entry.mTouched = true;
	entry.SqlUpdateRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, filename );

}

//-------------------------------------------------------------------//
void USFileCache::Delete () {

	this->Close ();
	
	if ( !this->mCacheFilename.size ()) return;
	if ( !this->mCacheFolderPath.size ()) return;
	
	USFileSys::DeleteFile ( this->mCacheFilename );
	USFileSys::DeleteDirectory ( this->mCacheFolderPath, true, true );
}

//-------------------------------------------------------------------//
void USFileCache::DeleteEntry ( USDataStoreEntry& entry ) {

	// delete it from the db
	entry.SqlDeleteRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, entry.mStoreKey.str ());
		
	// remove it's file from the partition and file system
	STLString filePath = this->GetFilePath ( entry.mStoreKey );
	USFileSys::DeleteFile ( filePath );
	
	// remove it from the partition
	USSqlValue param;
	USDataStorePartition partition;
	partition.SqlSelectRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, entry.mPriority );
	partition.mSize -= entry.mSize;
	assert ( partition.mSize >= 0 );
	partition.SqlUpdateRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, entry.mPriority );
}

//-------------------------------------------------------------------//
void USFileCache::DeleteNonReferencedEntries () {

	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	// get all the non referenced entries
	statement.Prepare (
		"SELECT %s.* FROM %s WHERE %s.%s NOT IN ( SELECT %s FROM %s )",
		SQL_ENTRIES,
		SQL_ENTRIES,
		
		SQL_ENTRIES,
		SQL_ENTRIES_STORE_KEY,
		
		SQL_DATAREF_STORE_KEY,
		SQL_DATAREF
	);

	USDataStoreEntry entry;
	
	while( statement.StepRow() ) {
		entry.SqlLoadCells( this->mConnection, statement );
		this->DeleteEntry( entry );
	}
	
	statement.Execute ( "COMMIT TRANSACTION" );
}

//-------------------------------------------------------------------//
void USFileCache::DeleteTouched () {
	
	// select all the entries == priority
	// sort ascending by last time used
	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	statement.Prepare (
		"SELECT * FROM %s WHERE %s = %d",
		SQL_ENTRIES,
		SQL_ENTRIES_TOUCHED,
		1
	);
	
	// record for stepping entries
	USSqlRecord record;
	record.Init ( this->mConnection, SQL_ENTRIES );
	
	// step through each entry and delete it
	while ( statement.StepRow ()) {
	
		// read in the cells
		record.LoadCells ( statement );
		
		// get the entry
		USDataStoreEntry entry;
		entry.SqlSerializeFromRecord ( record );
		
		// delete it from the store
		this->DeleteEntry ( entry );
	}
	
	statement.Execute ( "COMMIT TRANSACTION" );
}

//-------------------------------------------------------------------//
bool USFileCache::GetFile ( cc8* name, USFileStream& stream ) {
	
	if ( !name ) return false;
	STLString filePath = this->GetFileName ( name );
	
	if ( filePath.size ()) {
		stream.OpenRead ( filePath );
		return true;
	}
	return false;
}

//-------------------------------------------------------------------//
STLString USFileCache::GetFileName ( cc8* name ) {

	if ( !name ) return "";

	USSqlValue param;

	USDataStoreEntry entry;
	bool result = entry.SqlSelectRecord ( this->mConnection, "WHERE %s = '%s'", SQL_ENTRIES_STORE_KEY, name );
	if ( result == false ) {
		return "";
	}
	
	STLString filePath = this->GetFilePath ( entry.mStoreKey );
	if ( USFileSys::CheckFileExists ( filePath )) {
		return filePath;
	}
	return "";
}

//-------------------------------------------------------------------//
STLString USFileCache::GetFilePath ( cc8* filename ) {

	if ( !filename ) return "";

	STLString result = this->mCacheFolderPath;
	result += filename;
	//result.Write ( "0x%016llx", fileID );
	return result;
}

//-------------------------------------------------------------------//
f32 USFileCache::GetNonReferencedSize () {

	u32 nonReferencedBytes = 0;

	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	// get all the non referenced entries
	statement.Prepare (
		"SELECT * FROM %s WHERE %s.%s NOT IN ( SELECT %s FROM %s )",
		SQL_ENTRIES,
		
		SQL_ENTRIES,
		SQL_ENTRIES_STORE_KEY,
		
		SQL_DATAREF_STORE_KEY,
		SQL_DATAREF
	);

	USDataStoreEntry entry;
	
	while( statement.StepRow() ) {
		entry.SqlLoadCells( this->mConnection, statement );
		nonReferencedBytes += entry.mSize;
	}

	statement.Execute ( "COMMIT TRANSACTION" );

	return (( f32 )nonReferencedBytes / MBYTE );
}

//-------------------------------------------------------------------//
f32 USFileCache::GetPartitionMaxSize ( int priority ) {

	// get parition
	USSqlValue param;
	USDataStorePartition partition;
	bool success = partition.SqlSelectRecord ( this->mConnection, "WHERE %s = %d", SQL_PARTITIONS_PRIORITY, priority );
	
	// check for no such partition
	if ( success == false ) return 0;
	
	// otherwise, return partion's max size
	return ( f32 )partition.mMaxSize;
}

//-------------------------------------------------------------------//
f32 USFileCache::GetSizeUsedFor ( cc8* refKey ) {
	
	if ( !refKey ) return 0;
	
	u32 bytes = 0;
	
	USSqlStatement statement ( this->mConnection );
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	// get all the non referenced entries
	statement.Prepare (
		"SELECT * FROM %s WHERE %s.%s IN ( SELECT %s FROM %s WHERE %s.%s='%s' )",
		SQL_ENTRIES,
					   
		SQL_ENTRIES,
		SQL_ENTRIES_STORE_KEY,
					   
		SQL_DATAREF_STORE_KEY,

		SQL_DATAREF,
					   					   
		SQL_DATAREF,
		SQL_DATAREF_REF_KEY,
		refKey
	);
	
	USDataStoreEntry entry;
	
	while( statement.StepRow() ) {
		entry.SqlLoadCells( this->mConnection, statement );
		bytes += entry.mSize;
	}
	
	statement.Execute ( "COMMIT TRANSACTION" );
	
	return (( f32 )bytes / MBYTE );
}

//-------------------------------------------------------------------//
bool USFileCache::IsConnected () {

	return ( this->mConnection != 0 );
}

//-------------------------------------------------------------------//
bool USFileCache::Open () {

	this->Close ();

	if ( !this->mCacheFilename.size ()) return 0;
	if ( !this->mCacheFolderPath.size ()) return 0;

	if ( USFileSys::CheckFileExists ( this->mCacheFilename ) == false ) {
		this->Create ();
		return true;
	}

	if ( this->mConnection.Open ( this->mCacheFilename )) {
		
		USFileSys::AffirmPath ( this->mCacheFolderPath );
		this->DeleteTouched ();
		return true;
	}
	return false;
}

//-------------------------------------------------------------------//
void USFileCache::ReleaseRef ( cc8* refKey ) {
	
	if ( !refKey ) return;
	
	USSqlValue param;
	
	USSqlStatement statement(this->mConnection);
	statement.Execute ( "BEGIN IMMEDIATE TRANSACTION" );
	
	// select the dataRef that matches refKey/storeKey
	statement.Prepare (
		"SELECT * FROM %s WHERE %s = '%s'",
		SQL_DATAREF,
		SQL_DATAREF_REF_KEY,
		refKey
	);
	
	// bail if entry exists
	while ( statement.StepRow ()) {
		
		USDataStoreRef ref;
		ref.SqlLoadCells ( this->mConnection, statement );
		ref.SqlDeleteRecord ( this->mConnection, SQL_DATAREF_ID );
	}
	
	statement.Execute ( "COMMIT TRANSACTION" );
}

//-------------------------------------------------------------------//
void USFileCache::SetCacheFilename ( cc8* filename ) {

	if ( !filename ) return;

	this->mCacheFilename = USFileSys::Expand ( filename );
	this->mCacheFolderPath = USFileSys::GetCurrentPath ();
	this->mCacheFolderPath.write ( "_%s/", filename );
}

//-------------------------------------------------------------------//
USFileCache::USFileCache () :
	mConnection ( 0 ) {
}

//-------------------------------------------------------------------//
USFileCache::~USFileCache () {

	this->Close ();
}
