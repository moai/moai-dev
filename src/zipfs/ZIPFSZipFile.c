// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs_util.h>
#include <zipfs/ZIPFSZipFile.h>

#define SCAN_BUFFER_SIZE 256

#define ARCHIVE_HEADER_SIGNATURE  0x06054b50
#define ENTRY_HEADER_SIGNATURE  0x02014b50
#define FILE_HEADER_SIGNATURE  0x04034b50

//================================================================//
// ArchiveHeader
//================================================================//
typedef struct ArchiveHeader {

	unsigned long	mSignature;			// 4 End of central directory signature = 0x06054b50
	unsigned short	mDiskNumber;		// 2 Number of this disk
	unsigned short	mStartDisk;			// 2 Disk where central directory starts
	unsigned short	mTotalDiskEntries;	// 2 Total number of entries on disk
	unsigned short	mTotalEntries;		// 2 Total number of central in archive
	unsigned long	mCDSize;			// 4 Size of central directory in bytes
	unsigned long	mCDAddr;			// 4 Offset of start of central directory, relative to start of archive
	unsigned short	mCommentLength;		// 2 ZIP file comment length

} ArchiveHeader;

//================================================================//
// EntryHeader
//================================================================//
typedef struct EntryHeader {

	unsigned long	mSignature;				// 4 Central directory file header signature = 0x02014b50
	unsigned short	mByVersion;				// 2 Version made by
	unsigned short	mVersionNeeded;			// 2 Version needed to extract (minimum)
	unsigned short	mFlag;					// 2 General purpose bit flag
	unsigned short	mCompression;			// 2 Compression method
	unsigned short	mLastModTime;			// 2 File last modification time
	unsigned short	mLastModDate;			// 2 File last modification date
	unsigned long	mCrc32;					// 4 CRC-32
	unsigned long	mCompressedSize;		// 4 Compressed size
	unsigned long	mUncompressedSize;		// 4 Uncompressed size
	unsigned short	mNameLength;			// 2 File name length (n)
	unsigned short	mExtraFieldLength;		// 2 Extra field length (m)
	unsigned short	mCommentLength;			// 2 File comment length (k)
	unsigned short	mDiskNumber;			// 2 Disk number where file starts
	unsigned short	mInternalAttributes;	// 2 Internal file attributes
	unsigned long	mExternalAttributes;	// 4 External file attributes
	unsigned long	mFileHeaderAddr;		// 4 Relative offset of file header

} EntryHeader;

//================================================================//
// FileHeader
//================================================================//
typedef struct FileHeader {

	unsigned long	mSignature;				// 4	Local file header signature = 0x04034b50 (read as a little-endian number)
	unsigned short	mVersionNeeded;			// 2	Version needed to extract (minimum)
	unsigned short	mFlag;					// 2	General purpose bit flag
	unsigned short	mCompression;			// 2	Compression method
	unsigned short	mLastModTime;			// 2	File last modification time
	unsigned short	mLastModDate;			// 2	File last modification date
	unsigned long	mCrc32;					// 4	CRC-32
	unsigned long	mCompressedSize;		// 4	Compressed size
	unsigned long	mUncompressedSize;		// 4	Uncompressed size
	unsigned short	mNameLength;			// 2	File name length
	unsigned short	mExtraFieldLength;		// 2	Extra field length

} FileHeader;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static int read_archive_header ( FILE* file, ArchiveHeader* header ) {

	size_t filelen;
	size_t cursor;
	char buffer [ SCAN_BUFFER_SIZE ];
	size_t stepsize;
	size_t scansize;
	int i;
	
	if ( !file ) return -1;

	fseek ( file, 0, SEEK_END );
	filelen = ftell ( file );
	
	cursor = filelen;
	stepsize = SCAN_BUFFER_SIZE - 4;
    while ( cursor ) {
		
		cursor = ( cursor > stepsize ) ? cursor - stepsize : 0;
		scansize = (( cursor + SCAN_BUFFER_SIZE ) > filelen ) ? filelen - cursor : SCAN_BUFFER_SIZE;
		
		fseek ( file, cursor, SEEK_SET );
		fread ( buffer, scansize, 1, file );

        for ( i = scansize - 4; i > 0; --i ) {
			
			// maybe found it
			if ( *( unsigned long* )&buffer [ i ] == ARCHIVE_HEADER_SIGNATURE ) {

				fseek ( file, cursor + i, SEEK_SET );
				
				fread ( &header->mSignature, 4, 1, file );
				fread ( &header->mDiskNumber, 2, 1, file );
				fread ( &header->mStartDisk, 2, 1, file );
				fread ( &header->mTotalDiskEntries, 2, 1, file );
				fread ( &header->mTotalEntries, 2, 1, file );
				fread ( &header->mCDSize, 4, 1, file );
				fread ( &header->mCDAddr, 4, 1, file );
				fread ( &header->mCommentLength, 2, 1, file );
				
				return 0;
			}
		}
    }
	return -1;
}

//----------------------------------------------------------------//
static int read_entry_header ( FILE* file, EntryHeader* header ) {
	
	fread ( &header->mSignature, 4, 1, file );
	
	if ( header->mSignature != ENTRY_HEADER_SIGNATURE ) return -1;
	
	fread ( &header->mByVersion, 2, 1, file );
	fread ( &header->mVersionNeeded, 2, 1, file );
	fread ( &header->mFlag, 2, 1, file );
	fread ( &header->mCompression, 2, 1, file );
	fread ( &header->mLastModTime, 2, 1, file );
	fread ( &header->mLastModDate, 2, 1, file );
	fread ( &header->mCrc32, 4, 1, file );
	fread ( &header->mCompressedSize, 4, 1, file );
	fread ( &header->mUncompressedSize, 4, 1, file );
	fread ( &header->mNameLength, 2, 1, file );
	fread ( &header->mExtraFieldLength, 2, 1, file );
	fread ( &header->mCommentLength, 2, 1, file );
	fread ( &header->mDiskNumber, 2, 1, file );
	fread ( &header->mInternalAttributes, 2, 1, file );
	fread ( &header->mExternalAttributes, 4, 1, file );
	fread ( &header->mFileHeaderAddr, 4, 1, file );
	
	return 0;
}

//----------------------------------------------------------------//
static int read_file_header ( FILE* file, FileHeader* header ) {
	
	fread ( &header->mSignature, 4, 1, file );
	
	if ( header->mSignature != FILE_HEADER_SIGNATURE ) return -1;
	
	fread ( &header->mVersionNeeded, 2, 1, file );
	fread ( &header->mFlag, 2, 1, file );
	fread ( &header->mCompression, 2, 1, file );
	fread ( &header->mLastModTime, 2, 1, file );
	fread ( &header->mLastModDate, 2, 1, file );
	fread ( &header->mCrc32, 4, 1, file );
	fread ( &header->mCompressedSize, 4, 1, file );
	fread ( &header->mUncompressedSize, 4, 1, file );
	fread ( &header->mNameLength, 2, 1, file );
	fread ( &header->mExtraFieldLength, 2, 1, file );
	
	return 0;
}

//================================================================//
// ZIPFSZipFileEntry
//================================================================//

//----------------------------------------------------------------//
void ZIPFSZipFileEntry_Delete ( ZIPFSZipFileEntry* self ) {

	if ( !self ) return;

	clear_string ( self->mName );
	memset ( self, 0, sizeof ( ZIPFSZipFileEntry ));
	free ( self );
}

//================================================================//
// ZIPFSZipFileDir private
//================================================================//

ZIPFSZipFileDir*	ZIPFSZipFileDir_AffirmSubDir	( ZIPFSZipFileDir* self, const char* name, size_t len );

//----------------------------------------------------------------//
ZIPFSZipFileDir* ZIPFSZipFileDir_AffirmSubDir ( ZIPFSZipFileDir* self, const char* path, size_t len ) {

	ZIPFSZipFileDir* dir = self->mChildDirs;
	
	for ( ; dir; dir = dir->mNext ) {
		if ( count_same_nocase ( dir->mName, path ) == len ) return dir;
	}
	
	dir = ( ZIPFSZipFileDir* )calloc ( 1, sizeof ( ZIPFSZipFileDir ));
	
	dir->mNext = self->mChildDirs;
	self->mChildDirs = dir;
	
	dir->mName = ( char* )malloc ( len + 1 );
	memcpy ( dir->mName, path, len );
	dir->mName [ len ] = 0;
	
	return dir;
}

//================================================================//
// ZIPFSZipFileDir
//================================================================//

//----------------------------------------------------------------//
void ZIPFSZipFileDir_Delete ( ZIPFSZipFileDir* self ) {

	ZIPFSZipFileDir* dirCursor;
	ZIPFSZipFileEntry* entryCursor;

	if ( !self ) return;

	dirCursor = self->mChildDirs;
	while ( dirCursor ) {
		ZIPFSZipFileDir* dir = dirCursor;
		dirCursor = dirCursor->mNext;
		
		ZIPFSZipFileDir_Delete ( dir );
	}

	entryCursor = self->mChildFiles;
	while ( entryCursor ) {
		ZIPFSZipFileEntry* entry = entryCursor;
		entryCursor = entryCursor->mNext;
		
		ZIPFSZipFileEntry_Delete ( entry );
	}

	clear_string ( self->mName );
	memset ( self, 0, sizeof ( ZIPFSZipFileDir ));
	free ( self );
}

//================================================================//
// ZIPFSZipFile private
//================================================================//

void ZIPFSZipFile_AddEntry ( ZIPFSZipFile* self, EntryHeader* header, const char* name );

//----------------------------------------------------------------//
void ZIPFSZipFile_AddEntry ( ZIPFSZipFile* self, EntryHeader* header, const char* name ) {

	int i;
	const char* path = name;
	ZIPFSZipFileDir* dir = self->mRoot;
	
	// gobble the leading '/' (if any)
	if ( path [ 0 ] == '/' ) {
		path = &path [ 1 ];
	}
	
	// build out directories
	for ( i = 0; path [ i ]; ) {
		if ( path [ i ] == '/' ) {
			dir = ZIPFSZipFileDir_AffirmSubDir ( dir, path, i );
			path = &path [ i + 1 ];
			i = 0;
			continue;
		}
		i++;
	}
	
	if ( path [ 0 ]) {
		
		ZIPFSZipFileEntry* entry = ( ZIPFSZipFileEntry* )calloc ( 1, sizeof ( ZIPFSZipFileEntry ));
		
		entry->mFileHeaderAddr		= header->mFileHeaderAddr;
		entry->mCrc32				= header->mCrc32;
		entry->mCompression			= header->mCompression;
		entry->mCompressedSize		= header->mCompressedSize;
		entry->mUncompressedSize	= header->mUncompressedSize;
	
		entry->mName = copy_string ( path );
		
		entry->mNext = dir->mChildFiles;
		dir->mChildFiles = entry;
	}
}

//================================================================//
// ZIPFSZipFile
//================================================================//

//----------------------------------------------------------------//
void ZIPFSZipFile_Delete ( ZIPFSZipFile* self ) {

	if ( !self ) return;
	
	clear_string ( self->mFilename );
	
	if ( self->mRoot ) {
		ZIPFSZipFileDir_Delete ( self->mRoot );
	}
	
	memset ( self, 0, sizeof ( ZIPFSZipFile ));
	free ( self );
}

//----------------------------------------------------------------//
ZIPFSZipFileDir* ZIPFSZipFile_FindDir ( ZIPFSZipFile* self, char const* path ) {

	size_t i;
	ZIPFSZipFileDir* dir;
	
	if ( !self->mRoot ) return 0;
	if ( !path ) return 0;
	
	// gobble the leading '/' (if any)
	if ( path [ 0 ] == '/' ) {
		path = &path [ 1 ];
	}
	
	dir = self->mRoot;
	
	for ( i = 0; path [ i ]; ) {
		if ( path [ i ] == '/' ) {
			
			ZIPFSZipFileDir* cursor = dir->mChildDirs;
			
			for ( ; cursor; cursor = cursor->mNext ) {
				if ( count_same_nocase ( cursor->mName, path ) == i ) {
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
ZIPFSZipFileEntry* ZIPFSZipFile_FindEntry ( ZIPFSZipFile* self, char const* filename ) {

	ZIPFSZipFileDir* dir;
	ZIPFSZipFileEntry* entry;
	size_t i;
	
	i = strlen ( filename ) - 1;
	if ( filename [ i ] == '/' ) return 0;
	
	dir = ZIPFSZipFile_FindDir ( self, filename );
	if ( !dir ) return 0;
	
	for ( ; i >= 0; --i ) {
		if ( filename [ i ] == '/' ) {
			filename = &filename [ i + 1 ];
			break;
		}
	}
	
	entry = dir->mChildFiles;
	for ( ; entry; entry = entry->mNext ) {
		if ( strcmp_ignore_case ( entry->mName, filename ) == 0 ) break;
	}

	return entry;
}

//----------------------------------------------------------------//
ZIPFSZipFile* ZIPFSZipFile_New ( const char* filename ) {

	ZIPFSZipFile* self = 0;
	ArchiveHeader header;
	EntryHeader entryHeader;
	char* nameBuffer = 0;
	int nameBufferSize = 0;
	int result;
	int i;

	FILE* file = fopen ( filename, "rb" );
	if ( !file ) goto error;
 
	result = read_archive_header ( file, &header );
	if ( result ) goto error;
	
	if ( header.mDiskNumber != 0 ) goto error; // unsupported
	if ( header.mStartDisk != 0 ) goto error; // unsupported
	if ( header.mTotalDiskEntries != header.mTotalEntries ) goto error; // unsupported
	
	// seek to top of central directory
	fseek ( file, header.mCDAddr, SEEK_SET );
	
	// create the info
	self = ( ZIPFSZipFile* )calloc ( 1, sizeof ( ZIPFSZipFile ));
	
	self->mFilename = copy_string ( filename );
	self->mRoot = ( ZIPFSZipFileDir* )calloc ( 1, sizeof ( ZIPFSZipFileDir ));
	
	// parse in the entries
	for ( i = 0; i < header.mTotalEntries; ++i ) {
	
		result = read_entry_header ( file, &entryHeader );
		if ( result ) goto error;
		
		if (( entryHeader.mNameLength + 1 ) > nameBufferSize ) {
			nameBufferSize += SCAN_BUFFER_SIZE;
			nameBuffer = ( char* )realloc ( nameBuffer, nameBufferSize );
		}
		
		fread ( nameBuffer, entryHeader.mNameLength, 1, file );
		nameBuffer [ entryHeader.mNameLength ] = 0;
		
		ZIPFSZipFile_AddEntry ( self, &entryHeader, nameBuffer );
	}
	
	goto finish;
	
error:

	if ( self ) {
		ZIPFSZipFile_Delete ( self );
		self = 0;
	}

finish:

	if ( nameBuffer ) {
		free ( nameBuffer );
	}

	if ( file ) {
		fclose ( file );
	}

	return self;
}

//================================================================//
// ZIPFSZipStream private
//================================================================//

//----------------------------------------------------------------//
int			ZIPFSZipStream_AffirmBlock			( ZIPFSZipStream* self );
int			ZIPFSZipStream_FullyCache			( ZIPFSZipStream* self );
size_t		ZIPFSZipStream_Inflate				( ZIPFSZipStream* self, void* dest, size_t size, void* buffer, size_t bufferSize );
int			ZIPFSZipStream_InitBuffers			( ZIPFSZipStream* self );
int			ZIPFSZipStream_ResetZipStream		( ZIPFSZipStream* self );

//----------------------------------------------------------------//
int ZIPFSZipStream_AffirmBlock ( ZIPFSZipStream* self ) {

	int blockID = ( int )( self->mUncompressedCursor / ZIP_STREAM_BLOCK_SIZE );
	int sign = blockID & 1 ? 1 : 0;
	
	ZIPFSCacheBlock* block = &self->mBlock [ sign ];
	
	// if the block is already cached, we're good
	if ( block->mBlockID == blockID ) return 0;
	
	// initialize the block
	block->mBlockID = blockID;
	block->mBase = blockID * ZIP_STREAM_BLOCK_SIZE;
	block->mSize = 0;
	
	if ( !self->mCompression ) {	
		fseek ( self->mFile, self->mBaseAddr + block->mBase, SEEK_SET );
		block->mSize = fread ( block->mCache, 1, ZIP_STREAM_BLOCK_SIZE, self->mFile );
		return 0;
	}
	
	// if the new block is behind the last block we loaded, reset
	if ( blockID < self->mPrevBlockID ) {
		ZIPFSZipStream_ResetZipStream ( self );
	}
	
	// decompress blocks until we're caught up
	for ( ; self->mPrevBlockID < blockID; ++self->mPrevBlockID ) {
		block->mSize = ZIPFSZipStream_Inflate ( self, block->mCache, ZIP_STREAM_BLOCK_SIZE, self->mFileBuffer, self->mFileBufferSize );
	}
	return 0;
}

//----------------------------------------------------------------//
int ZIPFSZipStream_FullyCache ( ZIPFSZipStream* self ) {

	int result;

	self->mBufferSize = self->mEntry->mUncompressedSize;
	self->mBuffer = malloc ( self->mBufferSize );
	
	self->mFileBufferSize = self->mBufferSize;
	self->mFileBuffer = self->mBuffer;
	
	if ( self->mEntry->mCompression ) {
	
		char buffer [ ZIP_STREAM_FILEBUFFER_MAX ];
	
		result = inflateInit2 ( &self->mStream, -MAX_WBITS );
		if ( result != Z_OK ) return -1;
	
		ZIPFSZipStream_Inflate ( self, self->mFileBuffer, self->mFileBufferSize, buffer, ZIP_STREAM_FILEBUFFER_MAX );
		inflateEnd ( &self->mStream );
	}
	else {
	
		fseek ( self->mFile, self->mBaseAddr, SEEK_SET );
		fread ( self->mFileBuffer, 1, self->mFileBufferSize, self->mFile );
	}
	
	fclose ( self->mFile );
	self->mFile = 0;
	self->mFullyCached = 1;
	
	return 0;
}

//----------------------------------------------------------------//
size_t ZIPFSZipStream_Inflate ( ZIPFSZipStream* self, void* dest, size_t size, void* buffer, size_t bufferSize ) {

	int result;
	FILE* file = self->mFile;
	z_stream* stream = &self->mStream;
    ZIPFSZipFileEntry* entry = self->mEntry;
    size_t totalRead = 0;
    size_t totalOut = 0;

	if ( !file )	return 0;
	if ( !stream )	return 0;
	if ( !size )	return 0;
    
	stream->next_out = dest;
	stream->avail_out = size;

    while ( totalRead < size ) {
		
		if ( stream->avail_in == 0 ) {
			
			size_t cacheSize = entry->mCompressedSize - self->mCompressedCursor;
			
			if ( cacheSize > 0 ) {
				if ( cacheSize > bufferSize ) {
					cacheSize = bufferSize;
				}

				cacheSize = fread ( buffer, 1, cacheSize, file );
				if ( cacheSize <= 0 ) break;

				self->mCompressedCursor += cacheSize;
				stream->next_in = ( Bytef* )buffer;
				stream->avail_in = cacheSize;
			}
		}

		totalOut = stream->total_out;
		result = inflate ( stream, Z_SYNC_FLUSH );
		totalRead += ( stream->total_out - totalOut );
		
		if ( result != Z_OK ) break;
	}
	
    return totalRead;
}

//----------------------------------------------------------------//
int ZIPFSZipStream_InitBuffers ( ZIPFSZipStream* self ) {

	int result;
	ZIPFSZipFileEntry* entry = self->mEntry;

	// compute the file buffer size
	self->mFileBufferSize = 0;
	
	self->mCompression = entry->mCompression;
	if ( self->mCompression ) {
		result = inflateInit2 ( &self->mStream, -MAX_WBITS );
		if ( result != Z_OK ) return -1;
		self->mFileBufferSize = entry->mCompressedSize < ZIP_STREAM_FILEBUFFER_MAX ? entry->mCompressedSize : ZIP_STREAM_FILEBUFFER_MAX;
	}
	
	// allocate the master buffer
	self->mBufferSize = self->mFileBufferSize + ZIP_STREAM_CACHE_SIZE;
	self->mBuffer = malloc ( self->mBufferSize );
	
	// assign all the pointers to their positions in the master buffer
	self->mFileBuffer = self->mBuffer;
	self->mCache = ( void* )(( size_t )self->mFileBuffer + self->mFileBufferSize );
	
	self->mBlock [ 0 ].mCache = self->mCache;
	self->mBlock [ 1 ].mCache = ( void* )(( size_t )self->mCache + ZIP_STREAM_BLOCK_SIZE );
	
	// initialize the block IDs
	self->mBlock [ 0 ].mBlockID = -1;
	self->mBlock [ 1 ].mBlockID = -1;
	
	self->mPrevBlockID = -1;

	return 0;
}

//----------------------------------------------------------------//
int ZIPFSZipStream_ResetZipStream ( ZIPFSZipStream* self ) {

	int result;
	FILE* file = self->mFile;
	z_stream* stream = &self->mStream;

	z_stream newStream;
	memset ( &newStream, 0, sizeof ( z_stream ));
	
	result = fseek ( file, self->mBaseAddr, SEEK_SET );
	if ( result ) return -1;
	
	result = inflateInit2 ( &newStream, -MAX_WBITS );
	if ( result != Z_OK ) return -1;

	inflateEnd ( stream );
	( *stream ) = newStream;

	self->mCompressedCursor = 0;
	self->mUncompressedCursor = 0;
	self->mPrevBlockID = -1;
	
	return 0;
}

//================================================================//
// ZIPFSZipStream
//================================================================//

//----------------------------------------------------------------//
int ZIPFSZipStream_Close ( ZIPFSZipStream* self ) {

	if ( !self ) return 0;

	if ( self->mFile ) {
		fclose ( self->mFile );
	}
	
	if ( self->mCompression ) {
		inflateEnd ( &self->mStream );
	}
	
	if ( self->mBuffer ) {
		free ( self->mBuffer );
	}
	
	memset ( self, 0, sizeof ( ZIPFSZipStream ));
	free ( self );
	return 0;
}

//----------------------------------------------------------------//
int ZIPFSZipStream_IsAtEnd ( ZIPFSZipStream* self ) {

	return self->mUncompressedCursor >= self->mEntry->mUncompressedSize ? 1 : 0;	
}

//----------------------------------------------------------------//
ZIPFSZipStream* ZIPFSZipStream_Open ( ZIPFSZipFile* archive, const char* entryname ) {

	int result;
	FILE* file = 0;
	ZIPFSZipStream* self = 0;
	ZIPFSZipFileEntry* entry;
	FileHeader fileHeader;
	
	entry = ZIPFSZipFile_FindEntry ( archive, entryname );
	if ( !entry ) goto error;

	file = fopen ( archive->mFilename, "rb" );
	if ( !file ) goto error;

    self = ( ZIPFSZipStream* )calloc ( 1, sizeof ( ZIPFSZipStream ));

	self->mFile = file;
	self->mEntry = entry;
	// finfo->entry = (( entry->symlink != NULL ) ? entry->symlink : entry );

	// seek to the base of the zip file header
	result = fseek ( file, entry->mFileHeaderAddr, SEEK_SET );
	if ( result ) goto error;

	// read local header
	result = read_file_header ( file, &fileHeader );
	if ( result ) goto error;
	
	// sanity check
	if ( fileHeader.mCrc32 != entry->mCrc32 ) goto error;

	// skip the extra field, etc.
	result = fseek ( file, fileHeader.mNameLength + fileHeader.mExtraFieldLength, SEEK_CUR );
	if ( result ) goto error;

	// this is the base address of the compressed file data
	self->mBaseAddr = ftell ( file );
    
    // looks like all systems are go, so time to set up the buffers (or not)
    result = ( entry->mUncompressedSize <= ZIP_STREAM_BUFFER_MAX ) ? ZIPFSZipStream_FullyCache ( self ) : ZIPFSZipStream_InitBuffers ( self );
	if ( result ) goto error;
    
	return self;

error:

	if ( self ) {
		ZIPFSZipStream_Close ( self );
	}
	return 0;
}

//----------------------------------------------------------------//
size_t ZIPFSZipStream_Read ( ZIPFSZipStream* self, void* buffer, size_t size ) {

	ZIPFSCacheBlock* block;
	size_t remaining = size;
	size_t available;
	size_t read;

	while ( self->mUngetStackTop && remaining ) {
		
		*( char* )buffer = self->mUngetStack [ --self->mUngetStackTop ];
		buffer = ( void* )(( size_t )buffer + 1 );
		self->mUncompressedCursor++;
		remaining--;
	}

	if ( !remaining ) return size;

	if ( self->mFullyCached ) {
		
		void* src = ( void* )(( size_t )self->mFileBuffer + self->mUncompressedCursor );
		
		available = self->mEntry->mUncompressedSize - self->mUncompressedCursor;
		read = available < remaining ? available : remaining;
		
		memcpy ( buffer, src, read );
		
		self->mUncompressedCursor += read;
		remaining -= read;
		return size - remaining;
	}
	
	while ( remaining ) {
		
		size_t cursor;
		void* src;
		size_t blockID = ( size_t )( self->mUncompressedCursor / ZIP_STREAM_BLOCK_SIZE );
		int sign = blockID & 1 ? 1 : 0;
		
		ZIPFSZipStream_AffirmBlock ( self );
		
		block = &self->mBlock [ sign ];
		cursor = self->mUncompressedCursor - block->mBase;
		available = block->mSize - cursor;
		read = available < remaining ? available : remaining;
		
		if ( !read ) break;
		
		src = ( void* )(( size_t )block->mCache + cursor );
		memcpy ( buffer, src, read );
		
		self->mUncompressedCursor += read;
		remaining -= read;
		buffer = ( void* )(( size_t )buffer + read );
	}
	
    return size - remaining;
}

//----------------------------------------------------------------//
int ZIPFSZipStream_Seek ( ZIPFSZipStream* self, long int offset, int origin ) {

	size_t absOffset = 0;
	
	switch ( origin ) {
		case SEEK_CUR: {
			absOffset = self->mUncompressedCursor + offset;
			break;
		}
		case SEEK_END: {
			absOffset = self->mEntry->mUncompressedSize + offset;
			break;
		}
		case SEEK_SET: {
			absOffset = offset;
			break;
		}
	}

	if ( absOffset > self->mEntry->mUncompressedSize ) return -1;
	
	self->mUncompressedCursor = absOffset;
	self->mUngetStackTop = 0;
    return 0;
}

//----------------------------------------------------------------//
size_t ZIPFSZipStream_Tell ( ZIPFSZipStream* self ) {

	return self->mUncompressedCursor;
}

//----------------------------------------------------------------//
int ZIPFSZipStream_UnGetChar ( ZIPFSZipStream* self, char c ) {

	if ( self->mUncompressedCursor && ( self->mUngetStackTop < ZIP_STREAM_UNGET_STACK_SIZE )) {
	
		self->mUncompressedCursor--;
		self->mUngetStack [ self->mUngetStackTop++ ] = c;
		
		return 0;
	}
	return -1;
}
