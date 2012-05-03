// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zipfs/zipfs_mutex.h>

#ifdef _WIN32

	#include <windows.h>
	
	//================================================================//
	// mutex
	//================================================================//

	//----------------------------------------------------------------//
	ZIPFS_MUTEX* zipfs_mutex_create () {

		return ( ZIPFS_MUTEX* )CreateMutex ( NULL, FALSE, NULL );
	}

	//----------------------------------------------------------------//
	void zipfs_mutex_destroy ( ZIPFS_MUTEX* mutex ) {

		CloseHandle (( HANDLE )mutex );
	}

	//----------------------------------------------------------------//
	void zipfs_mutex_lock ( ZIPFS_MUTEX* mutex ) {

		WaitForSingleObject (( HANDLE )mutex, INFINITE );
	}

	//----------------------------------------------------------------//
	void zipfs_mutex_unlock ( ZIPFS_MUTEX* mutex ) {

		ReleaseMutex (( HANDLE )mutex );
	}

#else

	#include <windows.h>
	
	//================================================================//
	// mutex
	//================================================================//

	//----------------------------------------------------------------//
	ZIPFS_MUTEX* zipfs_mutex_create () {

		pthread_mutex_t* mutex = ( pthread_mutex_t* )malloc ( sizeof ( pthread_mutex_t ));
		pthread_mutex_init (( pthread_mutex_t* )mutex, 0 );
		return ( ZIPFS_MUTEX* )mutex;
	}

	//----------------------------------------------------------------//
	void zipfs_mutex_destroy ( ZIPFS_MUTEX* mutex ) {

		pthread_mutex_destroy (( ZIPFS_MUTEX* )mutex );
	}

	//----------------------------------------------------------------//
	void zipfs_mutex_lock ( ZIPFS_MUTEX* mutex ) {

		pthread_mutex_lock (( ZIPFS_MUTEX* )mutex );
	}

	//----------------------------------------------------------------//
	void zipfs_mutex_unlock ( ZIPFS_MUTEX* mutex ) {

		pthread_mutex_unlock (( ZIPFS_MUTEX* )mutex );
	}

#endif