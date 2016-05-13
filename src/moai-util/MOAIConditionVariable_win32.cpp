// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifdef _WIN32

#include <moai-util/MOAIConditionVariable_win32.h>
#include <moai-util/MOAIMutex_win32.h>

//================================================================//
// MOAIConditionVariableImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIConditionVariableImpl::Broadcast () {

	WakeAllConditionVariable ( &this->mConditional );
}

//----------------------------------------------------------------//
MOAIConditionVariableImpl::MOAIConditionVariableImpl () {

	InitializeConditionVariable ( &this->mConditional );
}

//----------------------------------------------------------------//
MOAIConditionVariableImpl::~MOAIConditionVariableImpl () {
}

//----------------------------------------------------------------//
void MOAIConditionVariableImpl::Signal () {

	WakeConditionVariable ( &this->mConditional );
}

//----------------------------------------------------------------//
void MOAIConditionVariableImpl::Wait ( MOAIMutexImpl& mutexImpl ) {

	SleepConditionVariableCS ( &this->mConditional, &mutexImpl.mCriticalSection, INFINITE );
}

#endif
