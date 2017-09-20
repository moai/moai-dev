// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA1_H
#define ZLHASHWRITERSHA1_H

#include <zl-crypto/ZLHashWriterMbedTLS.h>

//================================================================//
// ZLHashWriterSHA1
//================================================================//
class ZLHashWriterSHA1 :
	public ZLHashWriterMbedTLS {

public:
	size_t			GetBlockSize();
	ZLHashWriterSHA1();

};

#endif