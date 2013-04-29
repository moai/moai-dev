// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERCRC32_H
#define ZLHASHWRITERCRC32_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterCRC32
//================================================================//
class ZLHashWriterCRC32 :
	public ZLHashWriter {
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
					~ZLHashWriterCRC32		();
					ZLHashWriterCRC32		();
};

#endif
