// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/host.h>
#include <moai-util/headers.h>

#if MOAI_WITH_OPENSSL
	#include <openssl/conf.h>
	#include <openssl/crypto.h>

	#ifndef OPENSSL_NO_ENGINE
		#include <openssl/engine.h>
	#endif

	#ifndef OPENSSL_NO_ERR
		#include <openssl/err.h>
	#endif

	#include <openssl/ssl.h>
#endif

//================================================================//
// aku-util
//================================================================//

static bool sIsInitialized = false;

//----------------------------------------------------------------//
void AKUFinalizeUtil () {
	
	if ( !sIsInitialized ) return;
	
	#if MOAI_WITH_OPENSSL
	
		#ifndef OPENSSL_NO_ENGINE
			ENGINE_cleanup ();
		#endif
		
		CONF_modules_unload ( 1 );
		
		#ifndef OPENSSL_NO_ERR
			ERR_free_strings ();
		#endif
		
		EVP_cleanup ();
		CRYPTO_cleanup_all_ex_data ();
	#endif
	
	sIsInitialized = false;
}

//----------------------------------------------------------------//
void AKUInitializeUtil () {

	if ( !sIsInitialized ) {
		
		#if MOAI_WITH_OPENSSL
			SSL_load_error_strings ();
			SSL_library_init ();
		#endif
	
		sIsInitialized = true;
	}

    MOAIMainThreadTaskSubscriber::Affirm ();
	
	MOAIMath::Affirm ();
	
	#if MOAI_WITH_TINYXML
		MOAIXmlParser::Affirm ();
	#endif
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIDataBuffer )
	REGISTER_LUA_CLASS ( MOAIDataBufferStream )
	REGISTER_LUA_CLASS ( MOAIDataIOTask )
	REGISTER_LUA_CLASS ( MOAIFileStream )
	REGISTER_LUA_CLASS ( MOAIFileSystem )
	REGISTER_LUA_CLASS ( MOAIHashWriter )
	REGISTER_LUA_CLASS ( MOAIMath )
	REGISTER_LUA_CLASS ( MOAIMemStream )
	REGISTER_LUA_CLASS ( MOAIStreamReader )
	REGISTER_LUA_CLASS ( MOAIStreamWriter )
	REGISTER_LUA_CLASS ( MOAITaskQueue )
	REGISTER_LUA_CLASS ( MOAITaskThread )
	
	#if MOAI_WITH_JANSSON
		REGISTER_LUA_CLASS ( MOAIJsonParser )
	#endif
	
	#if MOAI_WITH_GPB
		REGISTER_LUA_CLASS ( MOAIParser )
	#endif
	
	#if MOAI_WITH_TINYXML
  		REGISTER_LUA_CLASS ( MOAIXmlParser )
	#endif
}
