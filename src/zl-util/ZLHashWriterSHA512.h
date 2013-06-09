// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA512_H
#define ZLHASHWRITERSHA512_H
#if MOAI_WITH_LIBCRYPTO

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterSHA512
//================================================================//
class ZLHashWriterSHA512 :
	public ZLHashWriter {
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
					~ZLHashWriterSHA512		();
					ZLHashWriterSHA512		();
};

#endif
#endif
