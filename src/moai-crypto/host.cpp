// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-crypto/host.h>
#include <moai-crypto/headers.h>


//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUCryptoAppFinalize () {
}

//----------------------------------------------------------------//
void AKUCryptoAppInitialize () {
}

//----------------------------------------------------------------//
void AKUCryptoContextInitialize () {

	// MOAI
	REGISTER_LUA_CLASS ( MOAIHashWriterCrypto )
}
