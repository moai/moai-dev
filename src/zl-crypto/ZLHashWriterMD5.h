// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERMD5_H
#define ZLHASHWRITERMD5_H

#include <zl-crypto/ZLHashWriterMbedTLS.h>

//================================================================//
// ZLHashWriterMD5
//================================================================//
class ZLHashWriterMD5 :
	public ZLHashWriterMbedTLS {

public:
	size_t			GetBlockSize			();
					ZLHashWriterMD5			();

};

#endif