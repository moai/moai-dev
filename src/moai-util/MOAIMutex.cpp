// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIMutex.h>
#include <moai-util/MOAIMutex_posix.h>
#include <moai-util/MOAIMutex_win32.h>

//================================================================//
// MOAIMutex
//================================================================//

//----------------------------------------------------------------//
void MOAIMutex::Lock () {

	this->mMutexImpl->Lock ();
}

//----------------------------------------------------------------//
MOAIMutex::MOAIMutex () {
	
	this->mMutexImpl = new MOAIMutexImpl ();
}

//----------------------------------------------------------------//
MOAIMutex::~MOAIMutex () {

	delete this->mMutexImpl;
}

//----------------------------------------------------------------//
void MOAIMutex::Unlock () {

	this->mMutexImpl->Unlock ();
}

//================================================================//
// MOAIAutoLock
//================================================================//

//----------------------------------------------------------------//
MOAIAutoLock::MOAIAutoLock ( MOAIMutex& mutex ) :
	mMutex( mutex ) {
	this->mMutex.Lock ();
}

//----------------------------------------------------------------//
MOAIAutoLock::~MOAIAutoLock () {
	this->mMutex.Unlock ();
}

