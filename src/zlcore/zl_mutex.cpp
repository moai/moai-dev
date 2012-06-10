// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zlcore/zl_mutex.h>

#ifdef _WIN32

	#include <windows.h>
	
	//================================================================//
	// mutex
	//================================================================//

	//----------------------------------------------------------------//
	ZL_MUTEX* zl_mutex_create () {

		return ( ZL_MUTEX* )CreateMutex ( NULL, FALSE, NULL );
	}

	//----------------------------------------------------------------//
	void zl_mutex_destroy ( ZL_MUTEX* mutex ) {

		CloseHandle (( HANDLE )mutex );
	}

	//----------------------------------------------------------------//
	void zl_mutex_lock ( ZL_MUTEX* mutex ) {

		WaitForSingleObject (( HANDLE )mutex, INFINITE );
	}

	//----------------------------------------------------------------//
	void zl_mutex_unlock ( ZL_MUTEX* mutex ) {

		ReleaseMutex (( HANDLE )mutex );
	}

#else
	
	#include <pthread.h>
	
	//================================================================//
	// mutex
	//================================================================//

	//----------------------------------------------------------------//
	ZL_MUTEX* zl_mutex_create () {

		pthread_mutex_t* mutex = ( pthread_mutex_t* )calloc ( 1, sizeof ( pthread_mutex_t ));
		pthread_mutex_init (( pthread_mutex_t* )mutex, 0 );
		return ( ZL_MUTEX* )mutex;
	}

	//----------------------------------------------------------------//
	void zl_mutex_destroy ( ZL_MUTEX* mutex ) {

		pthread_mutex_destroy (( pthread_mutex_t* )mutex );
	}

	//----------------------------------------------------------------//
	void zl_mutex_lock ( ZL_MUTEX* mutex ) {

		pthread_mutex_lock (( pthread_mutex_t* )mutex );
	}

	//----------------------------------------------------------------//
	void zl_mutex_unlock ( ZL_MUTEX* mutex ) {

		pthread_mutex_unlock (( pthread_mutex_t* )mutex );
	}

#endif