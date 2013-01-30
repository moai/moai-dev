// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERMD5_H
#define USHASHWRITERMD5_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterMD5
//================================================================//
class USHashWriterMD5 :
	public USHashWriter {
private:
	
	void*			mAlgorithm;
	u8				mHash [ 16 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~USHashWriterMD5		();
					USHashWriterMD5			();
};

#endif
