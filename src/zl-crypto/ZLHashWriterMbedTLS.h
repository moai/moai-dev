// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITEREMBEDTLS_H
#define ZLHASHWRITEREMBEDTLS_H

#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriterMD5
//================================================================//
class ZLHashWriterMbedTLS :
	public ZLHashWriter {
private:
	
	const	void*			mDigestInfo;
	void*			mDigestContext;
	u8*				mHash;

	//----------------------------------------------------------------//
	void			FinalizeHash			();
	void			InitHash				();

public:

	//----------------------------------------------------------------//
	
	void*			GetHash					();
	size_t			GetHashSize				();
	void			HashBytes				( const void* buffer, size_t size );
					~ZLHashWriterMbedTLS();
					ZLHashWriterMbedTLS(cc8* algo);
};

#endif