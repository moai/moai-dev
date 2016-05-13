// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifdef _WIN32

#include <moai-util/MOAIMutex_win32.h>

//================================================================//
// MOAIMutexImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIMutexImpl::Lock () {

	EnterCriticalSection ( &this->mCriticalSection );
}

//----------------------------------------------------------------//
MOAIMutexImpl::MOAIMutexImpl () {

	InitializeCriticalSection ( &this->mCriticalSection );
}

//----------------------------------------------------------------//
MOAIMutexImpl::~MOAIMutexImpl () {

	DeleteCriticalSection ( &this->mCriticalSection );
}

//----------------------------------------------------------------//
void MOAIMutexImpl::Unlock () {

	LeaveCriticalSection ( &this->mCriticalSection );
}

#endif