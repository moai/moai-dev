// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERWHIRLPOOL_H
#define USHASHWRITERWHIRLPOOL_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterWhirlpool
//================================================================//
class USHashWriterWhirlpool :
	public USHashWriter {
private:
	
	void*			mAlgorithm;
	u8				mHash [ 64 ];

	//----------------------------------------------------------------//
	void			FinalizeHash				();
	void			InitHash					();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	void*			GetHash						();
	size_t			GetHashSize					();
	void			HashBytes					( const void* buffer, size_t size );
					~USHashWriterWhirlpool		();
					USHashWriterWhirlpool		();
};

#endif
