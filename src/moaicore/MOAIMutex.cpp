// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIMutex.h>
#include <moaicore/MOAIMutex_posix.h>
#include <moaicore/MOAIMutex_win32.h>

//================================================================//
// MOAIMutex
//================================================================//

//----------------------------------------------------------------//
void MOAIMutex::Affirm () {

	if ( !this->mImpl ) {
		this->mImpl = new MOAIMutexImpl ();
		this->mImpl->Init ();
	}
}

//----------------------------------------------------------------//
void MOAIMutex::Clear () {

	if ( this->mImpl ) {
		delete this->mImpl;
		this->mImpl = 0;
	}
}

//----------------------------------------------------------------//
void MOAIMutex::Lock () {

	this->Affirm ();
	this->mImpl->Lock ();
}

//----------------------------------------------------------------//
MOAIMutex::MOAIMutex () :
	mImpl ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMutex::~MOAIMutex () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIMutex::Unlock () {

	this->Affirm ();
	this->mImpl->Unlock ();
}
