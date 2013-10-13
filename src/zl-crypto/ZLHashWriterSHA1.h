// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA1_H
#define ZLHASHWRITERSHA1_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterSHA1
//================================================================//
class ZLHashWriterSHA1 :
	public ZLHashWriter {
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
					~ZLHashWriterSHA1		();
					ZLHashWriterSHA1		();
};

#endif