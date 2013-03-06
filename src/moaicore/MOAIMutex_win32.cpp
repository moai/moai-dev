// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <moaicore/MOAIMutex_win32.h>

//================================================================//
// MOAIMutexImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIMutexImpl::Init () {
}

//----------------------------------------------------------------//
void MOAIMutexImpl::Lock () {

	WaitForSingleObject ( mMutexHandle, INFINITE );
}

//----------------------------------------------------------------//
MOAIMutexImpl::MOAIMutexImpl () {

	mMutexHandle = CreateMutex ( NULL, FALSE, NULL );
	assert ( mMutexHandle );
}

//----------------------------------------------------------------//
MOAIMutexImpl::~MOAIMutexImpl () {

	CloseHandle ( mMutexHandle );
	mMutexHandle = NULL;
}

//----------------------------------------------------------------//
void MOAIMutexImpl::Unlock () {

	ReleaseMutex ( mMutexHandle );
}

#endif