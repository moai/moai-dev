// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_config.h>
#include <zl-vfs/ZLThreadLocalPtr.h>

#ifndef _WIN32

#include <pthread.h>

//================================================================//
// ZLThreadLocalBase
//================================================================//
class ZLThreadLocalBase {
protected:

	void*	mKey;

//----------------------------------------------------------------//
void* ZLThreadLocalBase::Get ( void* key ) {
}

//----------------------------------------------------------------//
void ZLThreadLocalBase::Set ( void* key, void* value ) {
}

//----------------------------------------------------------------//
ZLThreadLocalBase::ZLThreadLocalBase () {

	#ifdef _WIN32
		this->mKey = malloc ( sizeof DWORD );
		*( DWORD* )this->mKey = TlsAlloc ();
	#else
		this->mKey = malloc ( sizeof pthread_key_t );
		pthread_key_create (( pthread_key_t* )this->mKey, NULL );
	#endif
}

//----------------------------------------------------------------//
ZLThreadLocalBase::~ZLThreadLocalBase () {

	free ( this->mKey );
}

//================================================================//
// ZLThreadLocal
//================================================================//
template < typename TYPE >
class ZLThreadLocalPtr {
private:

	#ifdef _WIN32
		DWORD			mTLSIndex;
	#else
		pthread_key_t	mKey;
	#endif

public:

	//----------------------------------------------------------------//
	TYPE* Get () {

		#ifdef _WIN32

			return ( TYPE* )TlsGetValue ( this->mTLSIndex );
		#else

			TYPE** ref = ( TYPE** )pthread_getspecific ( this->mKey );
			return ref ? *ref : 0;
		#endif
	}

	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {

		#ifdef _WIN32

			TlsSetValue ( this->mTLSIndex, assign );
		#else

			TYPE** ref = ( TYPE** )pthread_getspecific ( this->mKey );
			if ( !ref ) {

				ref = ( TYPE** )malloc ( sizeof ( TYPE* ));
				pthread_setspecific ( this->mKey, ref );
			}
			assert ( ref );
			*ref = assign;
		#endif
	}

	//----------------------------------------------------------------//
	ZLThreadLocalPtr () {
	
		#ifdef _WIN32
			this->mTLSIndex = TlsAlloc ();
		#else
			pthread_key_create ( &this->mKey, NULL );
		#endif

	}
	
	//----------------------------------------------------------------//
	~ZLThreadLocalPtr () {

		#ifdef _WIN32
			TlsFree ( this->mTLSIndex );
		#else
			// TODO: free handle memory (pthread version)
			// TODO: why does the pthread version use a handle at all? remove.
			pthread_key_delete ( this->mKey );
		#endif
	}
};

#endif
