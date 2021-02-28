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
// ZLScopedLock
//================================================================//

//----------------------------------------------------------------//
ZLScopedLock::ZLScopedLock ( ZLMutex& mutex ) :
	mMutex ( &mutex ) {
	this->mMutex->Lock ();
}

//----------------------------------------------------------------//
ZLScopedLock::ZLScopedLock ( const ZLScopedLock& other ) {
	this->mMutex = other.mMutex;
	(( ZLScopedLock& )other ).mMutex = NULL;
}

//----------------------------------------------------------------//
ZLScopedLock::~ZLScopedLock () {
	if ( this->mMutex ) {
		this->mMutex->Unlock ();
	}
}

