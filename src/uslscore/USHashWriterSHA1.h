// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERSHA1_H
#define USHASHWRITERSHA1_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterSHA1
//================================================================//
class USHashWriterSHA1 :
	public USHashWriter {
private:
	
	void*			mAlgorithm;
	u8				mHash [ 20 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~USHashWriterSHA1		();
					USHashWriterSHA1		();
};

#endif
