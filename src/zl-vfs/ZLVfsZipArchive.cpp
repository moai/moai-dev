// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_config.h>
#include <zl-vfs/headers.h>
#include <zl-vfs/zl_util.h>
#include <zl-vfs/ZLVfsZipArchive.h>

using namespace std;

#define SCAN_BUFFER_SIZE 256

#define ARCHIVE_HEADER_SIGNATURE  0x06054b50
#define ENTRY_HEADER_SIGNATURE  0x02014b50
#define FILE_HEADER_SIGNATURE  0x04034b50

//================================================================//
// ZLVfsZipArchiveHeader
//================================================================//
	
//----------------------------------------------------------------//
int ZLVfsZipArchiveHeader::FindAndRead ( FILE* file, size_t* offset ) {

	size_t filelen;
	size_t cursor;
	char buffer [ SCAN_BUFFER_SIZE ];
	size_t scansize;
	int i;
	
	if ( !file ) return -1;

	fseek ( file, 0, SEEK_END );
	filelen = ftell ( file );

	if (SCAN_BUFFER_SIZE < filelen) {
		cursor = filelen - SCAN_BUFFER_SIZE;
	} else {
		cursor = 4; /* Past first 32bit number */
	}
	while ( cursor ) {
		
		scansize = (( cursor + SCAN_BUFFER_SIZE ) > filelen ) ? filelen - cursor : SCAN_BUFFER_SIZE;
		
		fseek ( file, cursor, SEEK_SET );
		fread ( buffer, scansize, 1, file );

		for ( i = scansize - 4; i >= 0; --i ) {
			
			// maybe found it
			if ( *( u32* )&buffer [ i ] == ARCHIVE_HEADER_SIGNATURE ) {

				if ( offset ) {
					( *offset ) = cursor + i;
				}

				fseek ( file, cursor + i, SEEK_SET );
				
				fread ( &this->mSignature, 4, 1, file );
				fread ( &this->mDiskNumber, 2, 1, file );
				fread ( &this->mStartDisk, 2, 1, file );
				fread ( &this->mTotalDiskEntries, 2, 1, file );
				fread ( &this->mTotalEntries, 2, 1, file );
				fread ( &this->mCDSize, 4, 1, file );
				fread ( &this->mCDAddr, 4, 1, file );
				fread ( &this->mCommentLength, 2, 1, file );
				
				this->mDataOffset = ( cursor + i ) - ( this->mCDSize + this->mCDAddr ); // saved for use in loading the entry files
				this->mCDAddr += this->mDataOffset;

				return 0;
			}
		}
		
		cursor = ( cursor > SCAN_BUFFER_SIZE ) ? cursor - ( SCAN_BUFFER_SIZE - 4 ) : 0;
	}
	return -1;
}

//----------------------------------------------------------------//
void ZLVfsZipArchiveHeader::Write ( FILE* file ) {
	
	fwrite ( &this->mSignature, 4, 1, file );
	fwrite ( &this->mDiskNumber, 2, 1, file );
	fwrite ( &this->mStartDisk, 2, 1, file );
	fwrite ( &this->mTotalDiskEntries, 2, 1, file );
	fwrite ( &this->mTotalEntries, 2, 1, file );
	fwrite ( &this->mCDSize, 4, 1, file );
	fwrite ( &this->mCDAddr, 4, 1, file );
	fwrite ( &this->mCommentLength, 2, 1, file );
}

//================================================================//
// ZLVfsZipEntryHeader
//================================================================//
	
//----------------------------------------------------------------//
int ZLVfsZipEntryHeader::Read ( FILE* file, u32 dataOffset ) {
	
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
	
	this->mFileHeaderAddr += dataOffset;

	return 0;
}

//----------------------------------------------------------------//
int ZLVfsZipEntryHeader::StripTimestampsAndSkip ( FILE* file, size_t* fileHeaderAddr ) {

	/*
	unsigned long signature;
	fread ( &signature, 4, 1, file );
	if ( signature != ENTRY_HEADER_SIGNATURE ) return -1;
	
	fseek ( file, 8, SEEK_CUR );
	
	unsigned long zero = 0;
	fwrite ( &zero, 4, 1, file );
	
	fseek ( file, 12, SEEK_CUR );

	unsigned short nameLength;
	unsigned short extraFieldLength;
	unsigned short commentLength;

	fread ( &nameLength, 2, 1, file );
	fread ( &extraFieldLength, 2, 1, file );
	fread ( &commentLength, 2, 1, file );
	
	fseek ( file, 8, SEEK_CUR );
	
	if ( fileHeaderAddr ) {
		*fileHeaderAddr = 0;
		fread ( fileHeaderAddr, 4, 1, file );
	}
	else {
		fseek ( file, 4, SEEK_CUR );
	}
	
	fseek ( file, nameLength, SEEK_CUR );
	
	for ( size_t i = 0; i < ( extraFieldLength + commentLength ); ++i ) {
		fwrite ( &zero, 1, 1, file );
	}
	*/
	return 0;
}

//----------------------------------------------------------------//
void ZLVfsZipEntryHeader::Write ( FILE* file ) {
	
	fwrite ( &this->mSignature, 4, 1, file );
	fwrite ( &this->mByVersion, 2, 1, file );
	fwrite ( &this->mVersionNeeded, 2, 1, file );
	fwrite ( &this->mFlag, 2, 1, file );
	fwrite ( &this->mCompression, 2, 1, file );
	fwrite ( &this->mLastModTime, 2, 1, file );
	fwrite ( &this->mLastModDate, 2, 1, file );
	fwrite ( &this->mCrc32, 4, 1, file );
	fwrite ( &this->mCompressedSize, 4, 1, file );
	fwrite ( &this->mUncompressedSize, 4, 1, file );
	fwrite ( &this->mNameLength, 2, 1, file );
	fwrite ( &this->mExtraFieldLength, 2, 1, file );
	fwrite ( &this->mCommentLength, 2, 1, file );
	fwrite ( &this->mDiskNumber, 2, 1, file );
	fwrite ( &this->mInternalAttributes, 2, 1, file );
	fwrite ( &this->mExternalAttributes, 4, 1, file );
	fwrite ( &this->mFileHeaderAddr, 4, 1, file );
}

//================================================================//
// ZLVfsZipFileHeader
//================================================================//

//----------------------------------------------------------------//
int ZLVfsZipFileHeader::Read ( FILE* file ) {
	
	fread ( &this->mSignature, 4, 1, file );
	
	if ( this->mSignature != FILE_HEADER_SIGNATURE ) return -1;
	
	fread ( &this->mVersionNeeded, 2, 1, file );
	fread ( &this->mFlag, 2, 1, file );
	fread ( &this->mCompression, 2, 1, file );
	fread ( &this->mLastModTime, 2, 1, file );
	fread ( &this->mLastModDate, 2, 1, file );
	fread ( &this->mCrc32, 4, 1, file );				// *not* to be trusted (Android)
	fread ( &this->mCompressedSize, 4, 1, file );		// *not* to be trusted (Android)
	fread ( &this->mUncompressedSize, 4, 1, file );		// *not* to be trusted (Android)
	fread ( &this->mNameLength, 2, 1, file );
	fread ( &this->mExtraFieldLength, 2, 1, file );
	
	return 0;
}

//----------------------------------------------------------------//
int ZLVfsZipFileHeader::StripTimestampsAndSkip ( FILE* file ) {

	unsigned long signature;
	fread ( &signature, 4, 1, file );
	if ( signature != FILE_HEADER_SIGNATURE ) return -1;
	
	fseek ( file, 6, SEEK_CUR );
	
	unsigned long zero = 0;
	fwrite ( &zero, 4, 1, file );
	
	fseek ( file, 12, SEEK_CUR );

	unsigned short nameLength;
	unsigned short extraFieldLength;

	fread ( &nameLength, 2, 1, file );
	fread ( &extraFieldLength, 2, 1, file );
	
	fseek ( file, nameLength + extraFieldLength, SEEK_CUR );
	
	return 0;
}

//----------------------------------------------------------------//
void ZLVfsZipFileHeader::Write ( FILE* file ) {
	
	fwrite ( &this->mSignature, 4, 1, file );
	fwrite ( &this->mVersionNeeded, 2, 1, file );
	fwrite ( &this->mFlag, 2, 1, file );
	fwrite ( &this->mCompression, 2, 1, file );
	fwrite ( &this->mLastModTime, 2, 1, file );
	fwrite ( &this->mLastModDate, 2, 1, file );
	fwrite ( &this->mCrc32, 4, 1, file );
	fwrite ( &this->mCompressedSize, 4, 1, file );
	fwrite ( &this->mUncompressedSize, 4, 1, file );
	fwrite ( &this->mNameLength, 2, 1, file );
	fwrite ( &this->mExtraFieldLength, 2, 1, file );
}

//================================================================//
// ZLVfsZipFileDir
//================================================================//

//----------------------------------------------------------------//
ZLVfsZipFileDir* ZLVfsZipFileDir::AffirmSubDir ( const char* path, size_t len ) {

	ZLVfsZipFileDir* dir = this->mChildDirs;
	
	for ( ; dir; dir = dir->mNext ) {
		if ( count_same_nocase ( dir->mName.c_str (), path ) == len ) return dir;
	}
	
	dir = new ZLVfsZipFileDir ();
	
	dir->mNext = this->mChildDirs;
	this->mChildDirs = dir;
	
	dir->mName.assign ( path, len );
	
	return dir;
}

//----------------------------------------------------------------//
ZLVfsZipFileDir::ZLVfsZipFileDir () :
	mNext ( 0 ),
	mChildDirs ( 0 ),
	mChildFiles ( 0 ) {
}

//----------------------------------------------------------------//
ZLVfsZipFileDir::~ZLVfsZipFileDir () {

	ZLVfsZipFileDir* dirCursor = this->mChildDirs;
	while ( dirCursor ) {
		ZLVfsZipFileDir* dir = dirCursor;
		dirCursor = dirCursor->mNext;
		delete dir;
	}

	ZLVfsZipFileEntry* entryCursor = this->mChildFiles;
	while ( entryCursor ) {
		ZLVfsZipFileEntry* entry = entryCursor;
		entryCursor = entryCursor->mNext;
		delete entry;
	}
}

//================================================================//
// ZLVfsZipArchive
//================================================================//

//----------------------------------------------------------------//
void ZLVfsZipArchive::AddEntry ( ZLVfsZipEntryHeader* header, const char* name ) {

	int i;
	const char* path = name;
	ZLVfsZipFileDir* dir = this->mRoot;
	
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
		
		ZLVfsZipFileEntry* entry = new ZLVfsZipFileEntry ();
		
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
ZLVfsZipFileDir* ZLVfsZipArchive::FindDir ( char const* path ) {

	size_t i;
	ZLVfsZipFileDir* dir;
	
	if ( !this->mRoot ) return 0;
	if ( !path ) return 0;
	
	// gobble the leading '/' (if any)
	if ( path [ 0 ] == '/' ) {
		path = &path [ 1 ];
	}
	
	dir = this->mRoot;
	
	for ( i = 0; path [ i ]; ) {
		if ( path [ i ] == '/' ) {
			
			ZLVfsZipFileDir* cursor = dir->mChildDirs;
			
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
ZLVfsZipFileEntry* ZLVfsZipArchive::FindEntry ( char const* filename ) {

	ZLVfsZipFileDir* dir;
	ZLVfsZipFileEntry* entry;
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
int ZLVfsZipArchive::Open ( const char* filename ) {

	ZLVfsZipArchiveHeader header;
	ZLVfsZipEntryHeader entryHeader;
	char* nameBuffer = 0;
	int nameBufferSize = 0;
	int result = 0;
	int i;

	FILE* file = fopen ( filename, "rb" );
	if ( !file ) goto error;
	
	result = header.FindAndRead ( file, 0 );
	if ( result ) goto error;
	
	if ( header.mDiskNumber != 0 ) goto error; // unsupported
	if ( header.mStartDisk != 0 ) goto error; // unsupported
	if ( header.mTotalDiskEntries != header.mTotalEntries ) goto error; // unsupported
	
	// seek to top of central directory
	fseek ( file, header.mCDAddr, SEEK_SET );
	
	this->mFilename = filename;
	this->mRoot = new ZLVfsZipFileDir ();
	
	// parse in the entries
	for ( i = 0; i < header.mTotalEntries; ++i ) {
	
		result = entryHeader.Read ( file, header.mDataOffset );
		if ( result ) goto error;
		
		if (( entryHeader.mNameLength + 1 ) > nameBufferSize ) {
			nameBufferSize += SCAN_BUFFER_SIZE;
			char* tryRealloc = ( char* )realloc ( nameBuffer, nameBufferSize );
			if ( tryRealloc == NULL ) {
				goto error;
			} else {
				nameBuffer = tryRealloc;
			}
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
/*
int ZLVfsZipArchive::StripTimestamps ( const char* filename ) {
	
	FILE* file = fopen ( filename, "rb+" );
	if ( !file ) return -1;
	
	size_t headerOffset;
	ZLVfsZipArchiveHeader header;
	int result = header.FindAndRead ( file, &headerOffset );
	if ( result ) return -1;
	
	//size_t bufferSize = ftell ( file );
	//void* buffer = malloc ( bufferSize );
	
	if ( header.mDiskNumber != 0 ) return -1; // unsupported
	if ( header.mStartDisk != 0 ) return -1; // unsupported
	if ( header.mTotalDiskEntries != header.mTotalEntries ) return -1; // unsupported
	
	size_t endOfFiles = 0;
	
	fseek ( file, header.mCDAddr, SEEK_SET );
	for ( int i = 0; i < header.mTotalEntries; ++i ) {	

		size_t fileHeaderAddr;
		ZLVfsZipEntryHeader::StripTimestampsAndSkip ( file, &fileHeaderAddr );
		
		size_t resumeAddr = ftell ( file );
		fseek ( file, fileHeaderAddr, SEEK_SET );
		ZLVfsZipFileHeader::StripTimestampsAndSkip ( file );
		endOfFiles = ftell ( file );
		fseek ( file, resumeAddr, SEEK_SET );
	}
	
	char zb = 0;
	
	fseek ( file, endOfFiles, SEEK_SET );
	for ( size_t i = 0; i < ( header.mCDAddr - endOfFiles ); ++i ) {
		fwrite ( &zb, 1, 1, file );
	}
	
	fclose ( file );
	return 1;
}
*/

//----------------------------------------------------------------//
int ZLVfsZipArchive::StripTimestamps ( const char* infilename, const char* outfilename ) {

	FILE* infile = fopen ( infilename, "rb" );
	if ( !infile ) return -1;
	
	FILE* outfile = fopen ( outfilename, "wb" );
	if ( !outfile ) {
		fclose ( infile );
		return -1;
	}

	ZLVfsZipArchiveHeader header;
	int result = header.FindAndRead ( infile, 0 );
	if ( result ) {
		fclose ( infile );
		fclose ( outfile );
		return -1;
	}
	
	assert ( header.mDiskNumber == 0 );
	assert ( header.mStartDisk == 0 );
	assert ( header.mTotalDiskEntries == header.mTotalEntries );
	
	size_t* fileHeaderAddrTable = ( size_t* )alloca ( header.mTotalEntries * sizeof ( size_t ));
	
	// copy the files
	fseek ( infile, header.mCDAddr, SEEK_SET );
	for ( int i = 0; i < header.mTotalEntries; ++i ) {
	
		ZLVfsZipEntryHeader entryHeader;
		entryHeader.Read ( infile, header.mDataOffset);
		fseek ( infile, entryHeader.mNameLength + entryHeader.mExtraFieldLength + entryHeader.mCommentLength, SEEK_CUR );
		size_t resumeAddr = ftell ( infile );
		fseek ( infile, entryHeader.mFileHeaderAddr, SEEK_SET );
		
		ZLVfsZipFileHeader fileHeader;
		fileHeader.Read ( infile );
		
		size_t skip = fileHeader.mExtraFieldLength;
		
		fileHeader.mLastModDate = 0;
		fileHeader.mLastModTime = 0;
		fileHeader.mExtraFieldLength = 0;
		fileHeader.mFlag ^= ( 1 << 3 ); // clear bit 3

		fileHeaderAddrTable [ i ] = ftell ( outfile );
		fileHeader.Write ( outfile );
		
		for ( size_t i = 0; i < fileHeader.mNameLength; ++i ) {
			putc ( getc ( infile ), outfile );
		}
		
		fseek ( infile, skip, SEEK_CUR );
		
		size_t size = fileHeader.mCompression == 0 ? fileHeader.mUncompressedSize : fileHeader.mCompressedSize;
		
		for ( size_t i = 0; i < size; ++i ) {
			putc ( getc ( infile ), outfile );
		}
		
		fseek ( infile, resumeAddr, SEEK_SET );
	}
	
	size_t directoryStartAddr = ftell ( outfile );
	
	// copy the directory
	fseek ( infile, header.mCDAddr, SEEK_SET );
	for ( int i = 0; i < header.mTotalEntries; ++i ) {	

		ZLVfsZipEntryHeader entryHeader;
		entryHeader.Read ( infile, header.mDataOffset );

		entryHeader.mFileHeaderAddr = fileHeaderAddrTable [ i ];
		entryHeader.mLastModDate = 0;
		entryHeader.mLastModTime = 0;

		size_t skip = entryHeader.mExtraFieldLength + entryHeader.mCommentLength;

		entryHeader.mExtraFieldLength = 0;
		entryHeader.mCommentLength = 0;

		entryHeader.Write ( outfile );
		
		for ( size_t i = 0; i < entryHeader.mNameLength; ++i ) {
			putc ( getc ( infile ), outfile );
		}
		
		fseek ( infile, skip, SEEK_CUR );
	}
	
	size_t directoryEndAddr = ftell ( outfile );
	
	// copy the header
	header.mCDAddr = directoryStartAddr;
	header.mCDSize = directoryEndAddr - directoryStartAddr;
	header.mCommentLength = 0;
	header.Write ( outfile );

	fclose ( infile );
	fclose ( outfile );

	return 0;
}

//----------------------------------------------------------------//
ZLVfsZipArchive::ZLVfsZipArchive () :
	mRoot ( 0 ) {
}

//----------------------------------------------------------------//
ZLVfsZipArchive::~ZLVfsZipArchive () {
	
	if ( this->mRoot ) {
		delete this->mRoot;
	}
}
