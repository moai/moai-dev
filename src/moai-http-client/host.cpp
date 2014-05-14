// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-client/host.h>
#include <moai-http-client/headers.h>

#if MOAI_WITH_OPENSSL
	#include <openssl/ssl.h>
#endif

#if MOAI_WITH_ARES
	#include <ares.h>
#endif

#if MOAI_WITH_NSURL
	extern void RegisterMOAIHttpTaskNSURL ();
#endif

//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUHttpClientAppFinalize () {

	#if MOAI_WITH_LIBCURL
		curl_global_cleanup ();
	#endif
}

//----------------------------------------------------------------//
void AKUHttpClientAppInitialize () {

	#if MOAI_WITH_OPENSSL
		SSL_library_init ();
		SSL_load_error_strings ();
	#endif

	#if MOAI_WITH_ARES
		ares_set_default_dns_addr ( 0x08080808 );
	#endif
	
	#if MOAI_WITH_LIBCURL
		curl_global_init ( CURL_GLOBAL_WIN32 | CURL_GLOBAL_SSL );
	#endif
}

//----------------------------------------------------------------//
void AKUHttpClientContextInitialize () {

	#if MOAI_WITH_LIBCURL
		MOAIUrlMgrCurl::Affirm ();
		REGISTER_LUA_CLASS ( MOAIHttpTaskCurl )
	#endif

	#if MOAI_OS_NACL
		MOAIUrlMgrNaCl::Affirm ();
		REGISTER_LUA_CLASS ( MOAIHttpTaskNaCl )
	#endif
	
	#if MOAI_WITH_NSURL
		RegisterMOAIHttpTaskNSURL ();
	#endif
}

//----------------------------------------------------------------//
void AKUHttpClientUpdate () {

	#if MOAI_WITH_LIBCURL
		MOAIUrlMgrCurl::Get ().Process ();
	#endif

	#if MOAI_OS_NACL
		MOAIUrlMgrnaCl::Get ().Process ();
	#endif
}