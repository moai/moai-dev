// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERSHA512_H
#define USHASHWRITERSHA512_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterSHA512
//================================================================//
class USHashWriterSHA512 :
	public USHashWriter {
private:
	
	void*			mAlgorithm;
	u8				mHash [ 64 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~USHashWriterSHA512		();
					USHashWriterSHA512		();
};

#endif
