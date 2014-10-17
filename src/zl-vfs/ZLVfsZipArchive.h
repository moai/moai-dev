// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLZIPARCHIVE_H
#define ZLZIPARCHIVE_H

#include <zl-common/zl_types.h>

//================================================================//
// ZLVfsZipArchiveHeader
//================================================================//
class ZLVfsZipArchiveHeader {
public:

	u32	mSignature;			// 4 End of central directory signature = 0x06054b50
	u16	mDiskNumber;		// 2 Number of this disk
	u16	mStartDisk;			// 2 Disk where central directory starts
	u16	mTotalDiskEntries;	// 2 Total number of entries on disk
	u16	mTotalEntries;		// 2 Total number of central in archive
	u32	mCDSize;			// 4 Size of central directory in bytes
	u32	mCDAddr;			// 4 Offset of start of central directory, relative to start of archive
	u16	mCommentLength;		// 2 ZIP file comment length
	
	u32 mDataOffset;		// amount of junk before the central directory

	//----------------------------------------------------------------//
	int				FindAndRead				( FILE* file, size_t* offset );
	void			Write					( FILE* file );
};

//================================================================//
// ZLVfsZipEntryHeader
//================================================================//
class ZLVfsZipEntryHeader {
public:

	u32	mSignature;				// 4 Central directory file header signature = 0x02014b50
	u16	mByVersion;				// 2 Version made by
	u16	mVersionNeeded;			// 2 Version needed to extract (minimum)
	u16	mFlag;					// 2 General purpose bit flag
	u16	mCompression;			// 2 Compression method
	u16	mLastModTime;			// 2 File last modification time
	u16	mLastModDate;			// 2 File last modification date
	u32	mCrc32;					// 4 CRC-32
	u32	mCompressedSize;		// 4 Compressed size
	u32	mUncompressedSize;		// 4 Uncompressed size
	u16	mNameLength;			// 2 File name length (n)
	u16	mExtraFieldLength;		// 2 Extra field length (m)
	u16	mCommentLength;			// 2 File comment length (k)
	u16	mDiskNumber;			// 2 Disk number where file starts
	u16	mInternalAttributes;	// 2 Internal file attributes
	u32	mExternalAttributes;	// 4 External file attributes
	u32	mFileHeaderAddr;		// 4 Relative offset of file header
	
	//----------------------------------------------------------------//
	int				Read					( FILE* file, u32 dataOffset );
	static int		StripTimestampsAndSkip	( FILE* file, size_t* fileHeaderAdd );
	void			Write					( FILE* file );
};

//================================================================//
// ZLVfsZipFileHeader
//================================================================//
class ZLVfsZipFileHeader {
public:

	u32	mSignature;				// 4	Local file header signature = 0x04034b50 (read as a little-endian number)
	u16	mVersionNeeded;			// 2	Version needed to extract (minimum)
	u16	mFlag;					// 2	General purpose bit flag
	u16	mCompression;			// 2	Compression method
	u16	mLastModTime;			// 2	File last modification time
	u16	mLastModDate;			// 2	File last modification date
	u32	mCrc32;					// 4	CRC-32 (*not* to be trusted - Android)
	u32	mCompressedSize;		// 4	Compressed size (*not* to be trusted - Android)
	u32	mUncompressedSize;		// 4	Uncompressed size (*not* to be trusted - Android)
	u16	mNameLength;			// 2	File name length
	u16	mExtraFieldLength;		// 2	Extra field length

	//----------------------------------------------------------------//
	int				Read					( FILE* file );
	static int		StripTimestampsAndSkip	( FILE* file );
	void			Write					( FILE* file );
};

//================================================================//
// ZLVfsZipFileEntry
//================================================================//
class ZLVfsZipFileEntry {
public:

	std::string		mName;
	u32	mFileHeaderAddr;
	u32	mCrc32;
	u16	mCompression;
	u32	mCompressedSize;
	u32	mUncompressedSize;
	
	ZLVfsZipFileEntry*	mNext;
};

//================================================================//
// ZLVfsZipFileDir
//================================================================//
class ZLVfsZipFileDir {
public:

	friend class ZLVfsZipArchive;

	std::string		mName;
	
	ZLVfsZipFileDir*	mNext;
	ZLVfsZipFileDir*	mChildDirs;
	ZLVfsZipFileEntry*	mChildFiles;

	//----------------------------------------------------------------//
	ZLVfsZipFileDir*	AffirmSubDir			( const char* name, size_t len );

public:

	//----------------------------------------------------------------//
						ZLVfsZipFileDir			();
						~ZLVfsZipFileDir		();
};

//================================================================//
// ZLVfsZipArchive
//================================================================//
class ZLVfsZipArchive {
public:

	friend class ZLVfsZipStream;

	std::string				mFilename;
    ZLVfsZipFileDir*		mRoot;

	//----------------------------------------------------------------//
	void					AddEntry			( ZLVfsZipEntryHeader* header, const char* name );

public:

	//----------------------------------------------------------------//
	void					Delete				();
	ZLVfsZipFileDir*		FindDir				( char const* path );
	ZLVfsZipFileEntry*		FindEntry			( char const* filename );
	int						Open				( const char* filename );
	static int				StripTimestamps		( const char* infilename, const char* outfilename );
							ZLVfsZipArchive		();
							~ZLVfsZipArchive	();
};

#endif
