// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIOZIPFILE_H
#define MOAIOZIPFILE_H

#define ZIP_STREAM_BUFFER_MAX 4096

//================================================================//
// MOAIOZipEntry
//================================================================//
typedef struct MOAIOZipEntry {

	char*			mName;
	unsigned long	mFileHeaderAddr;
	unsigned long	mCrc32;
	unsigned short	mCompression;
	unsigned long	mCompressedSize;
	unsigned long	mUncompressedSize;

} MOAIOZipEntry;

//================================================================//
// MOAIOZipFile
//================================================================//
typedef struct MOAIOZipFile {

	char*				mFilename;
    int					mTotalEntries;
    MOAIOZipEntry*		mEntries;

} MOAIOZipFile;

//----------------------------------------------------------------//
extern void				MOAIOZipFile_Delete			( MOAIOZipFile* self );
extern MOAIOZipEntry*	MOAIOZipFile_FindEntry		( MOAIOZipFile* self, char const* name );
extern MOAIOZipFile*	MOAIOZipFile_New			( const char* filename );

//================================================================//
// MOAIOZipStream
//================================================================//
typedef struct MOAIOZipStream {

	FILE*				mFile;
	MOAIOZipEntry*		mEntry;
	size_t				mCompressedCursor;
	size_t				mUncompressedCursor;
	z_stream			mStream;
	void*				mBuffer;
	size_t				mBufferSize;
	size_t				mBaseAddr;

} MOAIOZipStream;

//----------------------------------------------------------------//
extern int				MOAIOZipStream_Close	( MOAIOZipStream* self );
extern MOAIOZipStream*	MOAIOZipStream_Open		( MOAIOZipFile* archive, const char* entryname );
extern size_t			MOAIOZipStream_Read		( MOAIOZipStream* self, void* buffer, size_t size );
extern int				MOAIOZipStream_Seek		( MOAIOZipStream* self, long int offset, int origin );
extern size_t			MOAIOZipStream_Tell		( MOAIOZipStream* self );

#endif