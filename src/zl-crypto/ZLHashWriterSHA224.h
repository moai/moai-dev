// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITERSHA224_H
#define ZLHASHWRITERSHA224_H

#include <zl-crypto/ZLHashWriterMbedTLS.h>

//================================================================//
// ZLHashWriterSHA224
//================================================================//
class ZLHashWriterSHA224 :
	public ZLHashWriterMbedTLS {

public:
	size_t			GetBlockSize();
	ZLHashWriterSHA224();

};

#endif