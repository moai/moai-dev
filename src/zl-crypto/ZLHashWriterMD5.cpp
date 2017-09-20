// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterMD5.h>
#include <mbedtls/md5.h>

//================================================================//
// ZLHashWriterMD5
//================================================================//


//----------------------------------------------------------------//
size_t ZLHashWriterMD5::GetBlockSize () {

	return 64;
}

//----------------------------------------------------------------//
ZLHashWriterMD5::ZLHashWriterMD5 (): ZLHashWriterMbedTLS("MD5") {

}

