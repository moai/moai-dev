// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlcore/zl_util.h>
#include <zlcore/ZLZipArchive.h>

using namespace std;

#define SCAN_BUFFER_SIZE 256

#define ARCHIVE_HEADER_SIGNATURE  0x06054b50
#define ENTRY_HEADER_SIGNATURE  0x02014b50
#define FILE_HEADER_SIGNATURE  0x04034b50

//================================================================//
// ZLZipArchiveHeader
//================================================================//
	
//----------------------------------------------------------------//
int ZLZipArchiveHeader::FindAndRead ( FILE* file ) {

	size_t filelen;
	size_t cursor;
	char buffer [ SCAN_BUFFER_SIZE ];
	size_t scansize;
	int i;
	
	if ( !file ) return -1;

	fseek ( file, 0, SEEK_END );
	filelen = ftell ( file );
	
	cursor = filelen - SCAN_BUFFER_SIZE;
	while ( cursor ) {
		
		scansize = (( cursor + SCAN_BUFFER_SIZE ) > filelen ) ? filelen - cursor : SCAN_BUFFER_SIZE;
		
		fseek ( file, cursor, SEEK_SET );
		fread ( buffer, scansize, 1, file );

		for ( i = scansize - 4; i >= 0; --i ) {
			
			// maybe found it
			if ( *( unsigned long* )&buffer [ i ] == ARCHIVE_HEADER_SIGNATURE ) {

				fseek ( file, cursor + i, SEEK_SET );
				
				fread ( &this->mSignature, 4, 1, file );
				fread ( &this->mDiskNumber, 2, 1, file );
				fread ( &this->mStartDisk, 2, 1, file );
				fread ( &this->mTotalDiskEntries, 2, 1, file );
				fread ( &this->mTotalEntries, 2, 1, file );
				fread ( &this->mCDSize, 4, 1, file );
				fread ( &this->mCDAddr, 4, 1, file );
				fread ( &this->mCommentLength, 2, 1, file );
				
				return 0;
			}
		}
		
		cursor = ( cursor > SCAN_BUFFER_SIZE ) ? cursor - ( SCAN_BUFFER_SIZE - 4 ) : 0;
	}
	return -1;
}

//================================================================//
// ZLZipEntryHeader
//================================================================//
	
//----------------------------------------------------------------//
int ZLZipEntryHeader::Read ( FILE* file ) {
	
	fread ( &this->mSignature, 4, 1, file );
	
	if ( this->mSignature != ENTRY_HEADER_SIGNATURE ) return -1;
	
	fread ( &this->mByVersion, 2, 1, file );
	fread ( &this->mVersionNeeded, 2, 1, file );
	fread ( &this->mFlag, 2, 1, file );
	fread ( &this->mCompression, 2, 1, file );
	fread ( &this->mLastModTime, 2, 1, file );
	fread ( &this->mLastModDate, 2, 1, file );
	fread ( &this->mCrc32, 4, 1, file );
	fread ( &this->mCompressedSize, 4, 1, file );
	fread ( &this->mUncompressedSize, 4, 1, file );
	fread ( &this->mNameLength, 2, 1, file );
	fread ( &this->mExtraFieldLength, 2, 1, file );
	fread ( &this->mCommentLength, 2, 1, file );
	fread ( &this->mDiskNumber, 2, 1, file );
	fread ( &this->mInternalAttributes, 2, 1, file );
	fread ( &this->mExternalAttributes, 4, 1, file );
	fread ( &this->mFileHeaderAddr, 4, 1, file );
	
	return 0;
}

//================================================================//
// ZLZipFileHeader
//================================================================//

//----------------------------------------------------------------//
int ZLZipFileHeader::Read ( FILE* file ) {
	
	fread ( &this->mSignature, 4, 1, file );
	
	if ( this->mSignature != FILE_HEADER_SIGNATURE ) return -1;
	
	fread ( &this->mVersionNeeded, 2, 1, file );
	fread ( &this->mFlag, 2, 1, file );
	fread ( &this->mCompression, 2, 1, file );
	fread ( &this->mLastModTime, 2, 1, file );
	fread ( &this->mLastModDate, 2, 1, file );
	fread ( &this->mCrc32, 4, 1, file );				// *not* to be trusted (Android)
	fread ( &this->mCompressedSize, 4, 1, file );		// *not* to be trusted (Android)
	fread ( &this->mUncompressedSize, 4, 1, file );	// *not* to be trusted (Android)
	fread ( &this->mNameLength, 2, 1, file );
	fread ( &this->mExtraFieldLength, 2, 1, file );
	
	return 0;
}

//================================================================//
// ZLZipFileDir
//================================================================//

//----------------------------------------------------------------//
ZLZipFileDir* ZLZipFileDir::AffirmSubDir ( const char* path, size_t len ) {

	ZLZipFileDir* dir = this->mChildDirs;
	
	for ( ; dir; dir = dir->mNext ) {
		if ( count_same_nocase ( dir->mName.c_str (), path ) == len ) return dir;
	}
	
	dir = new ZLZipFileDir ();
	
	dir->mNext = this->mChildDirs;
	this->mChildDirs = dir;
	
	dir->mName.assign ( path, len );
	
	return dir;
}

//----------------------------------------------------------------//
ZLZipFileDir::ZLZipFileDir () :
	mNext ( 0 ),
	mChildDirs ( 0 ),
	mChildFiles ( 0 ) {
}

//----------------------------------------------------------------//
ZLZipFileDir::~ZLZipFileDir () {

	ZLZipFileDir* dirCursor = this->mChildDirs;
	while ( dirCursor ) {
		ZLZipFileDir* dir = dirCursor;
		dirCursor = dirCursor->mNext;
		delete dir;
	}

	ZLZipFileEntry* entryCursor = this->mChildFiles;
	while ( entryCursor ) {
		ZLZipFileEntry* entry = entryCursor;
		entryCursor = entryCursor->mNext;
		delete entry;
	}
}

//================================================================//
// ZLZipArchive
//================================================================//

//----------------------------------------------------------------//
void ZLZipArchive::AddEntry ( ZLZipEntryHeader* header, const char* name ) {

	int i;
	const char* path = name;
	ZLZipFileDir* dir = this->mRoot;
	
	// gobble the leading '/' (if any)
	if ( path [ 0 ] == '/' ) {
		path = &path [ 1 ];
	}
	
	// build out directories
	for ( i = 0; path [ i ]; ) {
		if ( path [ i ] == '/' ) {
			dir = dir->AffirmSubDir ( path, i + 1 );
			path = &path [ i + 1 ];
			i = 0;
			continue;
		}
		i++;
	}
	
	if ( path [ 0 ]) {
		
		ZLZipFileEntry* entry = new ZLZipFileEntry ();
		
		entry->mFileHeaderAddr		= header->mFileHeaderAddr;
		entry->mCrc32				= header->mCrc32;
		entry->mCompression			= header->mCompression;
		entry->mCompressedSize		= header->mCompressedSize;
		entry->mUncompressedSize	= header->mUncompressedSize;
	
		entry->mName = path;
		
		entry->mNext = dir->mChildFiles;
		dir->mChildFiles = entry;
	}
}

//----------------------------------------------------------------//
ZLZipFileDir* ZLZipArchive::FindDir ( char const* path ) {

	size_t i;
	ZLZipFileDir* dir;
	
	if ( !this->mRoot ) return 0;
	if ( !path ) return 0;
	
	// gobble the leading '/' (if any)
	if ( path [ 0 ] == '/' ) {
		path = &path [ 1 ];
	}
	
	dir = this->mRoot;
	
	for ( i = 0; path [ i ]; ) {
		if ( path [ i ] == '/' ) {
			
			ZLZipFileDir* cursor = dir->mChildDirs;
			
			for ( ; cursor; cursor = cursor->mNext ) {
				if ( count_same_nocase ( cursor->mName.c_str (), path ) == cursor->mName.length ()) {
					dir = cursor;
					break;
				}
			}
			
			// no match found; can't resolve directory
			if ( !cursor ) return 0;
			
			path = &path [ i + 1 ];
			i = 0;
			continue;
		}
		i++;
	}
	
	return dir;
}

//----------------------------------------------------------------//
ZLZipFileEntry* ZLZipArchive::FindEntry ( char const* filename ) {

	ZLZipFileDir* dir;
	ZLZipFileEntry* entry;
	int i;
	
	if ( !filename ) return 0;
	
	i = strlen ( filename ) - 1;
	if ( filename [ i ] == '/' ) return 0;
	
	dir = this->FindDir ( filename );
	if ( !dir ) return 0;
	
	for ( ; i >= 0; --i ) {
		if ( filename [ i ] == '/' ) {
			filename = &filename [ i + 1 ];
			break;
		}
	}
	
	entry = dir->mChildFiles;
	for ( ; entry; entry = entry->mNext ) {
		if ( strcmp_ignore_case ( entry->mName.c_str (), filename ) == 0 ) break;
	}

	return entry;
}

//----------------------------------------------------------------//
int ZLZipArchive::Open ( const char* filename ) {

	ZLZipArchiveHeader header;
	ZLZipEntryHeader entryHeader;
	char* nameBuffer = 0;
	int nameBufferSize = 0;
	int result = 0;
	int i;

	FILE* file = fopen ( filename, "rb" );
	if ( !file ) goto error;
		
	result = header.FindAndRead ( file );
	if ( result ) goto error;
	
	if ( header.mDiskNumber != 0 ) goto error; // unsupported
	if ( header.mStartDisk != 0 ) goto error; // unsupported
	if ( header.mTotalDiskEntries != header.mTotalEntries ) goto error; // unsupported
	
	// seek to top of central directory
	fseek ( file, header.mCDAddr, SEEK_SET );
	
	this->mFilename = filename;
	this->mRoot = new ZLZipFileDir ();
	
	// parse in the entries
	for ( i = 0; i < header.mTotalEntries; ++i ) {
	
		result = entryHeader.Read ( file );
		if ( result ) goto error;
		
		if (( entryHeader.mNameLength + 1 ) > nameBufferSize ) {
			nameBufferSize += SCAN_BUFFER_SIZE;
			nameBuffer = ( char* )realloc ( nameBuffer, nameBufferSize );
		}
		
		fread ( nameBuffer, entryHeader.mNameLength, 1, file );
		nameBuffer [ entryHeader.mNameLength ] = 0;
		
		// skip the extra field, etc.
		result = fseek ( file, entryHeader.mCommentLength + entryHeader.mExtraFieldLength, SEEK_CUR );
		if ( result ) goto error;
		
		this->AddEntry ( &entryHeader, nameBuffer );
	}
	
	goto finish;
	
error:

	result = -1;

finish:

	if ( nameBuffer ) {
		free ( nameBuffer );
	}

	if ( file ) {
		fclose ( file );
	}
	
	return result;
}

//----------------------------------------------------------------//
ZLZipArchive::ZLZipArchive () :
	mRoot ( 0 ) {
}

//----------------------------------------------------------------//
ZLZipArchive::~ZLZipArchive () {
	
	if ( this->mRoot ) {
		delete this->mRoot;
	}
}
