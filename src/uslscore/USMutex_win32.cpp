// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <uslscore/USMutex_win32.h>

//================================================================//
// USMutexImpl
//================================================================//

//----------------------------------------------------------------//
void USMutexImpl::Init () {
}

//----------------------------------------------------------------//
void USMutexImpl::Lock () {

	WaitForSingleObject ( mMutexHandle, INFINITE );
}

//----------------------------------------------------------------//
void USMutexImpl::Unlock () {

	ReleaseMutex ( mMutexHandle );
}

//----------------------------------------------------------------//
USMutexImpl::USMutexImpl () {

	mMutexHandle = CreateMutex ( NULL, FALSE, NULL );
	assert ( mMutexHandle );
}

//----------------------------------------------------------------//
USMutexImpl::~USMutexImpl () {

	CloseHandle ( mMutexHandle );
	mMutexHandle = NULL;
}

#endif