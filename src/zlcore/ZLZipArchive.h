// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLZIPARCHIVE_H
#define ZLZIPARCHIVE_H

//================================================================//
// ZLZipArchiveHeader
//================================================================//
class ZLZipArchiveHeader {
public:

	unsigned long	mSignature;			// 4 End of central directory signature = 0x06054b50
	unsigned short	mDiskNumber;		// 2 Number of this disk
	unsigned short	mStartDisk;			// 2 Disk where central directory starts
	unsigned short	mTotalDiskEntries;	// 2 Total number of entries on disk
	unsigned short	mTotalEntries;		// 2 Total number of central in archive
	unsigned long	mCDSize;			// 4 Size of central directory in bytes
	unsigned long	mCDAddr;			// 4 Offset of start of central directory, relative to start of archive
	unsigned short	mCommentLength;		// 2 ZIP file comment length
	
	//----------------------------------------------------------------//
	int		FindAndRead		( FILE* file );
};

//================================================================//
// ZLZipEntryHeader
//================================================================//
class ZLZipEntryHeader {
public:

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
	
	//----------------------------------------------------------------//
	int		Read	( FILE* file );
};

//================================================================//
// ZLZipFileHeader
//================================================================//
class ZLZipFileHeader {
public:

	unsigned long	mSignature;				// 4	Local file header signature = 0x04034b50 (read as a little-endian number)
	unsigned short	mVersionNeeded;			// 2	Version needed to extract (minimum)
	unsigned short	mFlag;					// 2	General purpose bit flag
	unsigned short	mCompression;			// 2	Compression method
	unsigned short	mLastModTime;			// 2	File last modification time
	unsigned short	mLastModDate;			// 2	File last modification date
	unsigned long	mCrc32;					// 4	CRC-32 (*not* to be trusted - Android)
	unsigned long	mCompressedSize;		// 4	Compressed size (*not* to be trusted - Android)
	unsigned long	mUncompressedSize;		// 4	Uncompressed size (*not* to be trusted - Android)
	unsigned short	mNameLength;			// 2	File name length
	unsigned short	mExtraFieldLength;		// 2	Extra field length

	//----------------------------------------------------------------//
	int		Read	( FILE* file );
};

//================================================================//
// ZLZipFileEntry
//================================================================//
class ZLZipFileEntry {
public:

	std::string		mName;
	unsigned long	mFileHeaderAddr;
	unsigned long	mCrc32;
	unsigned short	mCompression;
	unsigned long	mCompressedSize;
	unsigned long	mUncompressedSize;
	
	ZLZipFileEntry*	mNext;
};

//================================================================//
// ZLZipFileDir
//================================================================//
class ZLZipFileDir {
public:

	friend class ZLZipArchive;

	std::string		mName;
	
	ZLZipFileDir*	mNext;
	ZLZipFileDir*	mChildDirs;
	ZLZipFileEntry*	mChildFiles;

	//----------------------------------------------------------------//
	ZLZipFileDir*	AffirmSubDir		( const char* name, size_t len );

public:

	//----------------------------------------------------------------//
					ZLZipFileDir		();
					~ZLZipFileDir		();
};

//================================================================//
// ZLZipArchive
//================================================================//
class ZLZipArchive {
public:

	friend class ZLZipStream;

	std::string			mFilename;
    ZLZipFileDir*		mRoot;

	//----------------------------------------------------------------//
	void				AddEntry		( ZLZipEntryHeader* header, const char* name );

public:

	//----------------------------------------------------------------//
	void				Delete				();
	ZLZipFileDir*		FindDir				( char const* path );
	ZLZipFileEntry*		FindEntry			( char const* filename );
	int					Open				( const char* filename );
						ZLZipArchive		();
						~ZLZipArchive	();
};

#endif