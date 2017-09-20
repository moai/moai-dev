// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA1.h>


//================================================================//
// ZLHashWriterSHA1
//================================================================//


//----------------------------------------------------------------//
size_t ZLHashWriterSHA1::GetBlockSize () {

	return 64;
}

//----------------------------------------------------------------//
ZLHashWriterSHA1::ZLHashWriterSHA1 ():ZLHashWriterMbedTLS("SHA1") {

}

