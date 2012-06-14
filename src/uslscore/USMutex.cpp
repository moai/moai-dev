// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USMutex.h>
#include <uslscore/USMutex_posix.h>
#include <uslscore/USMutex_win32.h>

//================================================================//
// USMutex
//================================================================//

//----------------------------------------------------------------//
void USMutex::Affirm () {

	if ( !this->mImpl ) {
		this->mImpl = new USMutexImpl ();
		this->mImpl->Init ();
	}
}

//----------------------------------------------------------------//
void USMutex::Clear () {

	if ( this->mImpl ) {
		delete this->mImpl;
		this->mImpl = 0;
	}
}

//----------------------------------------------------------------//
void USMutex::Lock () {

	this->Affirm ();
	this->mImpl->Lock ();
}

//----------------------------------------------------------------//
void USMutex::Unlock () {

	this->Affirm ();
	this->mImpl->Unlock ();
}

//----------------------------------------------------------------//
USMutex::USMutex () :
	mImpl ( 0 ) {
}

//----------------------------------------------------------------//
USMutex::~USMutex () {
	this->Clear ();
}


