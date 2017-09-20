// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA384.h>


//================================================================//
// ZLHashWriterSHA384
//================================================================//


//----------------------------------------------------------------//
size_t ZLHashWriterSHA384::GetBlockSize () {
	return 64;
}


//----------------------------------------------------------------//
ZLHashWriterSHA384::ZLHashWriterSHA384 (): ZLHashWriterMbedTLS("SHA384") {
	
}

