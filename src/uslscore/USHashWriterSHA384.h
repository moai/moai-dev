// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITERSHA384_H
#define USHASHWRITERSHA384_H

#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriterSHA384
//================================================================//
class USHashWriterSHA384 :
	public USHashWriter {
private:
	
	void*			mAlgorithm;
	u8				mHash [ 48 ];

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	size_t			GetBlockSize			();
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~USHashWriterSHA384		();
					USHashWriterSHA384		();
};

#endif
