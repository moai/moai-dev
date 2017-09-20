// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA224.h>


//================================================================//
// ZLHashWriterSHA224
//================================================================//

//----------------------------------------------------------------//
size_t ZLHashWriterSHA224::GetBlockSize () {

	return 64;
}

//----------------------------------------------------------------//
ZLHashWriterSHA224::ZLHashWriterSHA224 (): ZLHashWriterMbedTLS("SHA224") {
	
}
