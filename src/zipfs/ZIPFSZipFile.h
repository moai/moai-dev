// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFSZIPFILE_H
#define ZIPFSZIPFILE_H

#define ZIP_STREAM_FILEBUFFER_MAX 4096
#define ZIP_STREAM_CACHE_SIZE 4096
#define ZIP_STREAM_BLOCK_SIZE ( ZIP_STREAM_CACHE_SIZE >> 1 )
#define ZIP_STREAM_BUFFER_MAX ( ZIP_STREAM_FILEBUFFER_MAX + ZIP_STREAM_CACHE_SIZE )
#define ZIP_STREAM_UNGET_STACK_SIZE 32

//================================================================//
// ZIPFSZipFileEntry
//================================================================//
typedef struct ZIPFSZipFileEntry {

	char*			mName;
	unsigned long	mFileHeaderAddr;
	unsigned long	mCrc32;
	unsigned short	mCompression;
	unsigned long	mCompressedSize;
	unsigned long	mUncompressedSize;
	
	struct ZIPFSZipFileEntry*	mNext;

} ZIPFSZipFileEntry;

//----------------------------------------------------------------//
void	ZIPFSZipFileEntry_Delete	( ZIPFSZipFileEntry* self );

//================================================================//
// ZIPFSZipFileDir
//================================================================//
typedef struct ZIPFSZipFileDir {

	char*				mName;
	
	struct ZIPFSZipFileDir*		mNext;
	struct ZIPFSZipFileDir*		mChildDirs;
	struct ZIPFSZipFileEntry*	mChildFiles;

} ZIPFSZipFileDir;

//----------------------------------------------------------------//
void	ZIPFSZipFileDir_Delete	( ZIPFSZipFileDir* self );

//================================================================//
// ZIPFSZipFile
//================================================================//
typedef struct ZIPFSZipFile {

	char*				mFilename;
    ZIPFSZipFileDir*	mRoot;

} ZIPFSZipFile;

//----------------------------------------------------------------//
extern void					ZIPFSZipFile_Delete			( ZIPFSZipFile* self );
extern ZIPFSZipFileDir*		ZIPFSZipFile_FindDir		( ZIPFSZipFile* self, char const* path );
extern ZIPFSZipFileEntry*	ZIPFSZipFile_FindEntry		( ZIPFSZipFile* self, char const* filename );
extern ZIPFSZipFile*		ZIPFSZipFile_New			( const char* filename );

//================================================================//
// ZIPFSCacheBlock
//================================================================//
typedef struct ZIPFSCacheBlock {

	int					mBlockID;		// ID of currently cached blocks
	size_t				mBase;
	size_t				mSize;
	void*				mCache;			// pointer into cache

} ZIPFSCacheBlock;

//================================================================//
// ZIPFSZipStream
//================================================================//
typedef struct ZIPFSZipStream {

	FILE*				mFile;					// archive file
	ZIPFSZipFileEntry*	mEntry;					// address of the zip entry in the archive
	size_t				mBaseAddr;				// address of the zip data in the archive
	
	size_t				mCompressedCursor;		// cursor in the main stream
	size_t				mUncompressedCursor;	// cursor in the zip stream
	z_stream			mStream;				// underlying zip stream state
	
	int					mCompression;			// compression mode
	int					mFullyCached;			// nonzero if entire file has been uncompressed
	
	void*				mBuffer;
	size_t				mBufferSize;
	
	void*				mFileBuffer;			// buffer of compressed file data
	size_t				mFileBufferSize;		// size of buffer
	
	void*				mCache;					// decompressed data cache
	ZIPFSCacheBlock		mBlock [ 2 ];			// structure to hold block info

	int					mPrevBlockID;			// ID of the previous block decoded
	
	char				mUngetStack [ ZIP_STREAM_UNGET_STACK_SIZE ];
	int					mUngetStackTop;			

} ZIPFSZipStream;

//----------------------------------------------------------------//
extern int				ZIPFSZipStream_Close		( ZIPFSZipStream* self );
extern int				ZIPFSZipStream_IsAtEnd		( ZIPFSZipStream* self );
extern ZIPFSZipStream*	ZIPFSZipStream_Open			( ZIPFSZipFile* archive, const char* entryname );
extern size_t			ZIPFSZipStream_Read			( ZIPFSZipStream* self, void* buffer, size_t size );
extern int				ZIPFSZipStream_Seek			( ZIPFSZipStream* self, long int offset, int origin );
extern size_t			ZIPFSZipStream_Tell			( ZIPFSZipStream* self );
extern int				ZIPFSZipStream_UnGetChar	( ZIPFSZipStream* self, char c );

#endif