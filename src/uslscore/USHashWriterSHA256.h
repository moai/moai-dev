// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERSHA256_H
#define USHASHWRITERSHA256_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterSHA256
//================================================================//
class USHashWriterSHA256 :
	public USHashWriter {
private:
	
	void*			mAlgorithm;
	u8				mHash [ 32 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~USHashWriterSHA256		();
					USHashWriterSHA256		();
};

#endif
