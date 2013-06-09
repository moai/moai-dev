// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <iostream>
#include <typeinfo>
#include <assert.h>
#include <zlib.h>

#include <zl-util/ZLFileStream.h>
#include <zl-util/ZLZip.h>
#include <zl-util/ZLZipFile.h>

//----------------------------------------------------------------//
int ZLZipEntry::Inflate ( ZLStream& stream ) {
	
	if ( !this->mUncompressedSize ) return 0;
	
	this->mStream->Seek ( this->mDataAddr, SEEK_SET );
	int ret = ZLZip::Inflate ( *this->mStream, stream );
	
	return ret;
}

//----------------------------------------------------------------//
bool ZLZipEntry::SerializeIn ( ZLStream& stream ) {
	
	this->mStream = &stream;
	
	u32 signature = stream.Read < u32 >( 0 );
	if ( signature != LOCAL_FILE_HEADER_SIG ) return false;
	
	this->mVersionRequired		= stream.Read < u16 >( 0 ); //version needed to extract	2 bytes
	this->mBitFlag				= stream.Read < u16 >( 0 ); //general purpose bit flag		2 bytes
	this->mCompressionMethod	= stream.Read < u16 >( 0 ); //compression method			2 bytes
	this->mLastTime				= stream.Read < u16 >( 0 ); //last mod file time			2 bytes
	this->mLastDate				= stream.Read < u16 >( 0 ); //last mod file date			2 bytes
	this->mCRC32				= stream.Read < u32 >( 0 ); //crc-32						4 bytes
	this->mCompressedSize		= stream.Read < u32 >( 0 ); //compressed size				4 bytes
	this->mUncompressedSize		= stream.Read < u32 >( 0 ); //uncompressed size			4 bytes
	
	int nameLen		= ( int )stream.Read < u16 >( 0 ); //file name length		2 bytes
	int extraLen	= ( int )stream.Read < u16 >( 0 ); //extra field length	2 bytes
	
	this->mName		= stream.ReadString ( nameLen );
	this->mExtra	= stream.ReadString ( extraLen );
	
	this->mDataAddr = stream.GetCursor ();
	stream.Seek ( this->mCompressedSize, SEEK_CUR );
	
	if ( this->mBitFlag & BIT_HAS_DESCRIPTOR ) {
		
		u32 sigOrCRC = stream.Read < u32 >( 0 );
		u32 crc32;
		
		//crc-32						4 bytes
		if ( sigOrCRC == 0x04034b50 ) {
			crc32 = stream.Read < u32 >( 0 );
		}
		else {
			crc32 = sigOrCRC;
		}
		
		//u32 compressedSize = stream.Read < u32 >(); //compressed size				4 bytes
		//u32 uncompressedSize = stream.Read < u32 >(); //uncompressed size			4 bytes
		
		stream.Read < u32 >( 0 ); //compressed size			4 bytes
		stream.Read < u32 >( 0 ); //uncompressed size		4 bytes
	}
	
	return true;
}

//----------------------------------------------------------------//
ZLZipEntry::ZLZipEntry () :
	mStream ( 0 ) {
}

//----------------------------------------------------------------//
ZLZipEntry::~ZLZipEntry () {
}

//================================================================//
// ZLZipFile
//================================================================//

//----------------------------------------------------------------//
void ZLZipFile::Close () {

	this->mStream = 0;
	this->mEntryTable.Clear ();
	this->mEntryList.clear ();
}

//----------------------------------------------------------------//
ZLZipEntry& ZLZipFile::GetEntry ( u32 idx ) {

	return *this->mEntryTable [ idx ];
}

//----------------------------------------------------------------//
u32 ZLZipFile::GetTotal () {

	return this->mEntryTable.Size ();
}

//----------------------------------------------------------------//
int ZLZipFile::Inflate () {
	
	u32 total = this->mEntryTable.Size ();
	for ( u32 i = 0; i < total; ++i ) {
	
		ZLFileStream stream;
		stream.OpenWrite ( this->mEntryTable [ i ]->GetName ());
		int ret = this->mEntryTable [ i ]->Inflate ( stream );
		stream.Close ();
		
		if ( ret ) return ret;
	}
	return 0;
}

//----------------------------------------------------------------//
void ZLZipFile::Open ( ZLStream& stream ) {

	this->Close ();
	this->mStream = &stream;
	this->SerializeIn ();
}

//----------------------------------------------------------------//
void ZLZipFile::SerializeIn () {

	// read all the descriptors
	bool more = true;
	while ( more ) {
	
		u32 sig = this->mStream->Read < u32 >( 0 );
		this->mStream->Seek ( -( s32 )sizeof ( u32 ), SEEK_CUR );
		
		if ( sig != ZLZipEntry::LOCAL_FILE_HEADER_SIG ) {
			break;
		}
		
		this->mEntryList.push_back ( ZLZipEntry ());
		ZLZipEntry& entry = this->mEntryList.back ();
		entry.SerializeIn ( *this->mStream );
	}

	u32 totalEntries = ( u32 )this->mEntryList.size ();
	if ( totalEntries ) {
	
		this->mEntryTable.Init ( totalEntries );
		
		EntryListIt entryIt = this->mEntryList.begin ();
		for ( u32 i = 0; entryIt != this->mEntryList.end (); ++entryIt, ++i ) {
			ZLZipEntry& entry = *entryIt;
			this->mEntryTable [ i ] = &entry;
		}
	}
}

//----------------------------------------------------------------//
ZLZipFile::ZLZipFile () :
	mStream ( 0 ) {
}

//----------------------------------------------------------------//
ZLZipFile::~ZLZipFile() {
	this->Close ();	
}
