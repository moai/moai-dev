// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA256_H
#define ZLHASHWRITERSHA256_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterSHA256
//================================================================//
class ZLHashWriterSHA256 :
	public ZLHashWriter {
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
					~ZLHashWriterSHA256		();
					ZLHashWriterSHA256		();
};

#endif