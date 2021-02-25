// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLMutex.h>
#include <zl-core/ZLMutex_posix.h>
#include <zl-core/ZLMutex_win32.h>

//================================================================//
// ZLMutex
//================================================================//

//----------------------------------------------------------------//
void ZLMutex::Lock () {

	this->mMutexImpl->Lock ();
}

//----------------------------------------------------------------//
ZLMutex::ZLMutex () {
	
	this->mMutexImpl = new ZLMutexImpl ();
}

//----------------------------------------------------------------//
ZLMutex::~ZLMutex () {

	delete this->mMutexImpl;
}

//----------------------------------------------------------------//
void ZLMutex::Unlock () {

	this->mMutexImpl->Unlock ();
}

//================================================================//
// MOAIAutoLock
//================================================================//

//----------------------------------------------------------------//
MOAIAutoLock::MOAIAutoLock ( ZLMutex& mutex ) :
	mMutex( mutex ) {
	this->mMutex.Lock ();
}

//----------------------------------------------------------------//
MOAIAutoLock::~MOAIAutoLock () {
	this->mMutex.Unlock ();
}

