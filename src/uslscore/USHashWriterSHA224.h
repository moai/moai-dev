// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERSHA224_H
#define USHASHWRITERSHA224_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterSHA224
//================================================================//
class USHashWriterSHA224 :
	public USHashWriter {
private:
	
	void*			mAlgorithm;
	u8				mHash [ 28 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~USHashWriterSHA224		();
					USHashWriterSHA224		();
};

#endif
