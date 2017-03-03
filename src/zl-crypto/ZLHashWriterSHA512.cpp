// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA512.h>


//================================================================//
// ZLHashWriterSHA512
//================================================================//


//----------------------------------------------------------------//
size_t ZLHashWriterSHA512::GetBlockSize() {
	return 64;
}


//----------------------------------------------------------------//
ZLHashWriterSHA512::ZLHashWriterSHA512() : ZLHashWriterMbedTLS("SHA512") {

}
