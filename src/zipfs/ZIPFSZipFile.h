// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFSZIPFILE_H
#define ZIPFSZIPFILE_H

#define ZIP_STREAM_BUFFER_MAX 4096

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
// ZIPFSZipStream
//================================================================//
typedef struct ZIPFSZipStream {

	FILE*				mFile;
	ZIPFSZipFileEntry*	mEntry;
	size_t				mCompressedCursor;
	size_t				mUncompressedCursor;
	z_stream			mStream;
	void*				mBuffer;
	size_t				mBufferSize;
	size_t				mBaseAddr;

} ZIPFSZipStream;

//----------------------------------------------------------------//
extern int				ZIPFSZipStream_Close	( ZIPFSZipStream* self );
extern ZIPFSZipStream*	ZIPFSZipStream_Open		( ZIPFSZipFile* archive, const char* entryname );
extern size_t			ZIPFSZipStream_Read		( ZIPFSZipStream* self, void* buffer, size_t size );
extern int				ZIPFSZipStream_Seek		( ZIPFSZipStream* self, long int offset, int origin );
extern size_t			ZIPFSZipStream_Tell		( ZIPFSZipStream* self );

#endif