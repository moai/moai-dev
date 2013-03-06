// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <moaicore/MOAIMutex_posix.h>

//================================================================//
// MOAIMutexImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIMutexImpl::Init () {

	pthread_mutex_init ( &this->mMutex, 0 );
}

//----------------------------------------------------------------//
void MOAIMutexImpl::Lock () {

	pthread_mutex_lock ( &this->mMutex );
}

//----------------------------------------------------------------//
MOAIMutexImpl::MOAIMutexImpl () {

	memset ( &this->mMutex, 0, sizeof ( pthread_mutex_t ));
}

//----------------------------------------------------------------//
MOAIMutexImpl::~MOAIMutexImpl () {

	pthread_mutex_destroy ( &this->mMutex );
}

//----------------------------------------------------------------//
void MOAIMutexImpl::Unlock () {

	pthread_mutex_unlock ( &this->mMutex );
}

#endif
