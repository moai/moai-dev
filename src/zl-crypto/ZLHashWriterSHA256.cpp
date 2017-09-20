// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA256.h>


//================================================================//
// ZLHashWriterSHA256
//================================================================//

//----------------------------------------------------------------//
size_t ZLHashWriterSHA256::GetBlockSize () {
	return 64;
}

//----------------------------------------------------------------//
ZLHashWriterSHA256::ZLHashWriterSHA256 (): ZLHashWriterMbedTLS("SHA256") {
	
}

