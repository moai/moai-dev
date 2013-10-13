// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA384_H
#define ZLHASHWRITERSHA384_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterSHA384
//================================================================//
class ZLHashWriterSHA384 :
	public ZLHashWriter {
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
					~ZLHashWriterSHA384		();
					ZLHashWriterSHA384		();
};

#endif
