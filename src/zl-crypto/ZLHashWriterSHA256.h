// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA256_H
#define ZLHASHWRITERSHA256_H

#include <zl-crypto/ZLHashWriterMbedTLS.h>

//================================================================//
// ZLHashWriterSHA256
//================================================================//
class ZLHashWriterSHA256 :
	public ZLHashWriterMbedTLS {

public:
	size_t			GetBlockSize();
	ZLHashWriterSHA256();
};

#endif