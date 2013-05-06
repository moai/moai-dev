// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERWHIRLPOOL_H
#define ZLHASHWRITERWHIRLPOOL_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterWhirlpool
//================================================================//
class ZLHashWriterWhirlpool :
	public ZLHashWriter {
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
					~ZLHashWriterWhirlpool		();
					ZLHashWriterWhirlpool		();
};

#endif
