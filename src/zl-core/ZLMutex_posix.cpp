// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifndef _WIN32

#include <zl-core/ZLMutex_posix.h>

//================================================================//
// ZLMutexImpl
//================================================================//

//----------------------------------------------------------------//
void ZLMutexImpl::Lock () {

	pthread_mutex_lock ( &this->mMutex );
}

//----------------------------------------------------------------//
ZLMutexImpl::ZLMutexImpl () {

	memset ( &this->mMutex, 0, sizeof ( pthread_mutex_t ));
	pthread_mutex_init ( &this->mMutex, 0 );
}

//----------------------------------------------------------------//
ZLMutexImpl::~ZLMutexImpl () {

	pthread_mutex_destroy ( &this->mMutex );
}

//----------------------------------------------------------------//
void ZLMutexImpl::Unlock () {

	pthread_mutex_unlock ( &this->mMutex );
}

#endif
