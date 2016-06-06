// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifndef _WIN32

#include <pthread.h>

#include <moai-util/MOAIConditionVariable_posix.h>
#include <moai-util/MOAIMutex_posix.h>

//================================================================//
// MOAIConditionVariableImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIConditionVariableImpl::Broadcast () {

	pthread_cond_broadcast ( &this->mConditional );
}

//----------------------------------------------------------------//
MOAIConditionVariableImpl::MOAIConditionVariableImpl () {

	memset ( &this->mConditional, 0, sizeof ( pthread_cond_t ));
	pthread_cond_init ( &this->mConditional, NULL );
}

//----------------------------------------------------------------//
MOAIConditionVariableImpl::~MOAIConditionVariableImpl () {

	pthread_cond_destroy ( &this->mConditional );
}

//----------------------------------------------------------------//
void MOAIConditionVariableImpl::Signal () {

	pthread_cond_signal ( &this->mConditional );
}

//----------------------------------------------------------------//
void MOAIConditionVariableImpl::Wait ( MOAIMutexImpl& mutexImpl ) {

	pthread_cond_wait ( &this->mConditional, &mutexImpl.mMutex );
}

#endif
