// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERADLER32_H
#define ZLHASHWRITERADLER32_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterAdler32
//================================================================//
class ZLHashWriterAdler32 :
	public ZLHashWriter {
private:
	
	u32				mSum;
	u8				mHash [ 4 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	u32				GetChecksum				();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~ZLHashWriterAdler32	();
					ZLHashWriterAdler32		();
};

#endif
