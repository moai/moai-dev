// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA224_H
#define ZLHASHWRITERSHA224_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterSHA224
//================================================================//
class ZLHashWriterSHA224 :
	public ZLHashWriter {
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
					~ZLHashWriterSHA224		();
					ZLHashWriterSHA224		();
};

#endif