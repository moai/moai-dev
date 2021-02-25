// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifdef _WIN32

#include <zl-core/ZLMutex_win32.h>

//================================================================//
// ZLMutexImpl
//================================================================//

//----------------------------------------------------------------//
void ZLMutexImpl::Lock () {

	EnterCriticalSection ( &this->mCriticalSection );
}

//----------------------------------------------------------------//
ZLMutexImpl::ZLMutexImpl () {

	InitializeCriticalSection ( &this->mCriticalSection );
}

//----------------------------------------------------------------//
ZLMutexImpl::~ZLMutexImpl () {

	DeleteCriticalSection ( &this->mCriticalSection );
}

//----------------------------------------------------------------//
void ZLMutexImpl::Unlock () {

	LeaveCriticalSection ( &this->mCriticalSection );
}

#endif
