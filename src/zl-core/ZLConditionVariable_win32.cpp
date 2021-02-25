// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifdef _WIN32

#include <zl-core/ZLConditionVariable_win32.h>
#include <zl-core/MOAIMutex_win32.h>

//================================================================//
// ZLConditionVariableImpl
//================================================================//

//----------------------------------------------------------------//
void ZLConditionVariableImpl::Broadcast () {

	WakeAllConditionVariable ( &this->mConditional );
}

//----------------------------------------------------------------//
ZLConditionVariableImpl::ZLConditionVariableImpl () {

	InitializeConditionVariable ( &this->mConditional );
}

//----------------------------------------------------------------//
ZLConditionVariableImpl::~ZLConditionVariableImpl () {
}

//----------------------------------------------------------------//
void ZLConditionVariableImpl::Signal () {

	WakeConditionVariable ( &this->mConditional );
}

//----------------------------------------------------------------//
void ZLConditionVariableImpl::Wait ( MOAIMutexImpl& mutexImpl ) {

	SleepConditionVariableCS ( &this->mConditional, &mutexImpl.mCriticalSection, INFINITE );
}

#endif
