// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-http-client/host.h>
#include <moai-http-client/headers.h>

#if MOAI_WITH_ARES
	#include <ares.h>
#endif

//================================================================//
// aku-util
//================================================================//

static bool sIsInitialized = false;

//----------------------------------------------------------------//
void AKUFinalizeHttpClient () {
	
	if ( !sIsInitialized ) return;
	
	#if MOAI_WITH_LIBCURL
		curl_global_cleanup ();
	#endif
	
	sIsInitialized = false;
}

//----------------------------------------------------------------//
void AKUInitializeHttpClient () {

	if ( !sIsInitialized ) {
	
		#if MOAI_WITH_ARES
			ares_set_default_dns_addr ( 0x08080808 );
		#endif
		
		#if MOAI_WITH_LIBCURL
			curl_global_init ( CURL_GLOBAL_WIN32 | CURL_GLOBAL_SSL );
		#endif
	
		sIsInitialized = true;
	}

	#if MOAI_WITH_LIBCURL
		MOAIUrlMgrCurl::Affirm ();
		REGISTER_LUA_CLASS ( MOAIHttpTaskCurl )
	#endif

	#if MOAI_OS_NACL
		MOAIUrlMgrnaCl::Affirm ();
		REGISTER_LUA_CLASS ( MOAIHttpTaskNaCl )
	#endif
}
