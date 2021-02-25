// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifndef _WIN32

#include <pthread.h>

#include <zl-core/ZLConditionVariable_posix.h>
#include <zl-core/ZLMutex_posix.h>

//================================================================//
// ZLConditionVariableImpl
//================================================================//

//----------------------------------------------------------------//
void ZLConditionVariableImpl::Broadcast () {

	pthread_cond_broadcast ( &this->mConditional );
}

//----------------------------------------------------------------//
ZLConditionVariableImpl::ZLConditionVariableImpl () {

	memset ( &this->mConditional, 0, sizeof ( pthread_cond_t ));
	pthread_cond_init ( &this->mConditional, NULL );
}

//----------------------------------------------------------------//
ZLConditionVariableImpl::~ZLConditionVariableImpl () {

	pthread_cond_destroy ( &this->mConditional );
}

//----------------------------------------------------------------//
void ZLConditionVariableImpl::Signal () {

	pthread_cond_signal ( &this->mConditional );
}

//----------------------------------------------------------------//
void ZLConditionVariableImpl::Wait ( ZLMutexImpl& mutexImpl ) {

	pthread_cond_wait ( &this->mConditional, &mutexImpl.mMutex );
}

#endif
