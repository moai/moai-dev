// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITEREMBEDTLS_H
#define ZLHASHWRITEREMBEDTLS_H

#include <zl-util/ZLHashWriter.h>

#define ZL_DECL_MBEDTLS_HASH_WRITER(name, initStr, blocksize)			\
	class ZLHashWriter##name :											\
		public ZLHashWriterMbedTLS {									\
	public:																\
		size_t GetBlockSize () { return blocksize; }					\
		ZLHashWriter##name () : ZLHashWriterMbedTLS ( initStr ) {}		\
	};

//================================================================//
// ZLHashWriterMD5
//================================================================//
class ZLHashWriterMbedTLS :
	public ZLHashWriter {
private:
	
	const void*		mDigestInfo;
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
					~ZLHashWriterMbedTLS	();
					ZLHashWriterMbedTLS		( cc8* algo );
};

ZL_DECL_MBEDTLS_HASH_WRITER ( MD5, "MD5", 64 )
ZL_DECL_MBEDTLS_HASH_WRITER ( SHA1, "SHA1", 64 )
ZL_DECL_MBEDTLS_HASH_WRITER ( SHA224, "SHA224", 64 )
ZL_DECL_MBEDTLS_HASH_WRITER ( SHA256, "SHA256", 64 )
ZL_DECL_MBEDTLS_HASH_WRITER ( SHA384, "SHA384", 64 )
ZL_DECL_MBEDTLS_HASH_WRITER ( SHA512, "SHA512", 64 )

#endif
