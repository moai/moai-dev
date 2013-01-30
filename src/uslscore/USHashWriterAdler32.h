// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERADLER32_H
#define USHASHWRITERADLER32_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterAdler32
//================================================================//
class USHashWriterAdler32 :
	public USHashWriter {
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
					~USHashWriterAdler32	();
					USHashWriterAdler32		();
};

#endif
