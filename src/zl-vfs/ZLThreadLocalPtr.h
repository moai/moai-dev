// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLTHREADLOCALPTR_H
#define ZLTHREADLOCALPTR_H

#include <pthread.h>

#ifdef __cplusplus

//================================================================//
// ZLThreadLocal
//================================================================//
template < typename TYPE >
class ZLThreadLocalPtr {
private:

	pthread_key_t	mKey;

#ifdef ANDROID
	TYPE*			mValue;
#endif

public:

	//----------------------------------------------------------------//
	TYPE* Get () {

		#ifdef ANDROID
			return this->mValue;
		#else
			TYPE** ref = ( TYPE** )pthread_getspecific ( this->mKey );
			return ref ? *ref : 0;
		#endif
	}

	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {
	
		#ifdef ANDROID
			this->mValue = assign;
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
	
		#ifndef ANDROID
			pthread_key_create ( &this->mKey, NULL );
		#endif
	}
	
	//----------------------------------------------------------------//
	~ZLThreadLocalPtr () {
	
		#ifndef ANDROID
			pthread_key_delete ( this->mKey );
		#endif
	}
};

#endif
#endif
