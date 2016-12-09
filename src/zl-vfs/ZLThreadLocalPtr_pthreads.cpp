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

//----------------------------------------------------------------//
void* ZLThreadLocalBase::GetPtr () {

	return pthread_getspecific ( *( pthread_key_t* )this->mKey );
}

//----------------------------------------------------------------//
void ZLThreadLocalBase::SetPtr ( void* value ) {

	pthread_setspecific ( *( pthread_key_t* )this->mKey, value );
}

//----------------------------------------------------------------//
ZLThreadLocalBase::ZLThreadLocalBase () {

	this->mKey = malloc ( sizeof ( pthread_key_t ));
	pthread_key_create (( pthread_key_t* )this->mKey, NULL );
}

//----------------------------------------------------------------//
ZLThreadLocalBase::~ZLThreadLocalBase () {

	// TODO: free handle memory
	// TODO: why does the pthread version use a handle at all? remove.
	pthread_key_delete ( *( pthread_key_t* )this->mKey );
	free ( this->mKey );
}

#endif
