// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERCRC32_H
#define USHASHWRITERCRC32_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterCRC32
//================================================================//
class USHashWriterCRC32 :
	public USHashWriter {
private:
	
	u32				mAlgorithm;
	u32				mSum;
	u8				mHash [ 4 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	enum {
		CRC32,
		CRC32B,
	};

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	u32				GetChecksum				();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
	void			SetAlgorithm			( u32 algorithm );
					~USHashWriterCRC32		();
					USHashWriterCRC32		();
};

#endif
