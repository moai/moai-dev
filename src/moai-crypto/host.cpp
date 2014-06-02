// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-crypto/host.h>
#include <moai-crypto/headers.h>

#include <openssl/conf.h>
#include <openssl/crypto.h>

#ifndef OPENSSL_NO_ENGINE
	#include <openssl/engine.h>
#endif

#ifndef OPENSSL_NO_ERR
	#include <openssl/err.h>
#endif

#ifdef MOAI_OS_WINDOWS
	static HANDLE* ssl_mutexes;

	static void _locking_function ( int mode, int mutex_num, const char *file, int line ) {
	
		(void) line;
		(void) file;

		if (mode & 1) {  // 1 is CRYPTO_LOCK
			WaitForSingleObject ( ssl_mutexes[mutex_num], INFINITE ) == WAIT_OBJECT_0? 0 : -1;
		} else {
			ReleaseMutex( ssl_mutexes[mutex_num]);
		}
	}

	static unsigned long _ssl_id_callback ( ) {

		return ( unsigned long )GetCurrentThreadId ();
	}
#endif // MOAI_OS_WINDOWS

//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUCryptoAppFinalize () {
	
	#ifndef OPENSSL_NO_ENGINE
		ENGINE_cleanup ();
	#endif
	
	CONF_modules_unload ( 1 );
	
	#ifndef OPENSSL_NO_ERR
		ERR_free_strings ();
	#endif
	
	//EVP_cleanup ();
	//CRYPTO_cleanup_all_ex_data ();
}

//----------------------------------------------------------------//
void AKUCryptoAppInitialize () {

	#ifdef MOAI_OS_WINDOWS
		// Initialize locking callbacks, needed for thread safety.
		// http://www.openssl.org/support/faq.html#PROG1
		ssl_mutexes = ( HANDLE* ) malloc ( sizeof ( HANDLE ) * CRYPTO_num_locks());
	
		CRYPTO_set_locking_callback ( &_locking_function );
		CRYPTO_set_id_callback ( &_ssl_id_callback );
	#endif
}

//----------------------------------------------------------------//
void AKUCryptoContextInitialize () {

	// MOAI
	REGISTER_LUA_CLASS ( MOAIHashWriterCrypto )
}
