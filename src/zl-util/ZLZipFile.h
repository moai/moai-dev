// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLZIPFILE_H
#define ZLZIPFILE_H

#include <zl-util/STLList.h>
#include <zl-util/STLString.h>
#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLLeanArray.h>

//================================================================//
// ZLZipEntry
//================================================================//
class ZLZipEntry {
private:

	//local file header signature		4 bytes ( 0x04034b50 )
	//version needed to extract			2 bytes
	//general purpose bit flag			2 bytes
	//compression method				2 bytes
	//last mod file time				2 bytes
	//last mod file date				2 bytes
	//crc-32							4 bytes
	//compressed size					4 bytes
	//uncompressed size					4 bytes
	//file name length					2 bytes
	//extra field length				2 bytes
	//file name							( variable size )
	//extra field						( variable size )
	
	//compressed bytes					( variable size )
	
	//crc-32							4 bytes
	//compressed size					4 bytes
	//uncompressed size					4 bytes

	static const u32 LOCAL_FILE_HEADER_SIG = 0x04034b50;

	enum {
		BIT_HAS_DESCRIPTOR	= 1 << 0x03,
	};

	u16 mVersionRequired;
	u16 mBitFlag;
	u16 mCompressionMethod;
	u16 mLastTime;
	u16 mLastDate;
	u32 mCRC32;
	u32 mCompressedSize;
	u32 mUncompressedSize;

	STLString mName;
	STLString mExtra;

	u32 mDataAddr; // location of data in file

	ZLStream* mStream;

public:

	friend class ZLZipFile;

	GET ( cc8*, Name, mName )
	GET ( u32, UncompressedSize, mUncompressedSize )
	
	//----------------------------------------------------------------//
	int		Inflate			( ZLStream& stream );
	bool	SerializeIn		( ZLStream& stream );
			ZLZipEntry		();
			~ZLZipEntry		();
};


//================================================================//
// ZLZipFile
//================================================================//
class ZLZipFile {
private:

	typedef STLList < ZLZipEntry >::iterator EntryListIt;
	STLList < ZLZipEntry > mEntryList;

	ZLLeanArray < ZLZipEntry* > mEntryTable;

	ZLStream* mStream;

	//----------------------------------------------------------------//
	void		SerializeIn		();

public:

	//----------------------------------------------------------------//
	void		Close			();
	ZLZipEntry&	GetEntry		( u32 idx );
	u32			GetTotal		();
	int			Inflate			();
	void		Open			( ZLStream& stream );
				ZLZipFile		();
	virtual		~ZLZipFile		();
};

#endif
