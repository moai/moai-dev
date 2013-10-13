// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERMD5_H
#define ZLHASHWRITERMD5_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterMD5
//================================================================//
class ZLHashWriterMD5 :
	public ZLHashWriter {
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
					~ZLHashWriterMD5		();
					ZLHashWriterMD5			();
};

#endif