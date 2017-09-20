// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA512_H
#define ZLHASHWRITERSHA512_H

#include <zl-crypto/ZLHashWriterMbedTLS.h>

//================================================================//
// ZLHashWriterSHA512
//================================================================//
class ZLHashWriterSHA512 :
	public ZLHashWriterMbedTLS {

public:
	size_t			GetBlockSize();
	ZLHashWriterSHA512();
};

#endif
