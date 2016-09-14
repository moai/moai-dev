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

public:

	//----------------------------------------------------------------//
	TYPE* Get () {
	
		TYPE** ref = ( TYPE** )pthread_getspecific ( this->mKey );
		return ref ? *ref : 0;
	}

	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {
	
		TYPE** ref = ( TYPE** )pthread_getspecific ( this->mKey );
		if ( !ref ) {
			ref = ( TYPE** )malloc ( sizeof ( TYPE* ));
			pthread_setspecific ( this->mKey, ref );
		}
		assert ( ref );
		*ref = assign;
	}

	//----------------------------------------------------------------//
	ZLThreadLocalPtr () {
	
		pthread_key_create ( &this->mKey, NULL );
	}
	
	//----------------------------------------------------------------//
	~ZLThreadLocalPtr () {
	
		pthread_key_delete ( this->mKey );
	}
};

#endif
#endif
