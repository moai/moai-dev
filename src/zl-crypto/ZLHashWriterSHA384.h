// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA384_H
#define ZLHASHWRITERSHA384_H

#include <zl-crypto/ZLHashWriterMbedTLS.h>

//================================================================//
// ZLHashWriterSHA384
//================================================================//
class ZLHashWriterSHA384 :
	public ZLHashWriterMbedTLS {

public:
	size_t			GetBlockSize();
	ZLHashWriterSHA384();
};

#endif
