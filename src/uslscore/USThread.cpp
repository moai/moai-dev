// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USThread.h>
#include <uslscore/USThread_posix.h>
#include <uslscore/USThread_win32.h>

//================================================================//
// USThreadState
//================================================================//

//----------------------------------------------------------------//
u32 USThreadState::GetState () {

	u32 state;

	this->mMutex.Lock ();
	state = this->mState;
	this->mMutex.Unlock ();
	
	return state;
}

//----------------------------------------------------------------//
bool USThreadState::IsPaused () {

	return ( this->GetState () == PAUSED );
}

//----------------------------------------------------------------//
bool USThreadState::IsRunning () {

	return ( this->GetState () == RUNNING );
}

//----------------------------------------------------------------//
bool USThreadState::IsStopped () {

	return ( this->GetState () == STOPPED );
}

//----------------------------------------------------------------//
void USThreadState::SetState ( u32 state ) {

	this->mMutex.Lock ();
	this->mState = state;
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
USThreadState::USThreadState () :
	mState ( NEW ) {
}

//----------------------------------------------------------------//
USThreadState::~USThreadState () {
}

//================================================================//
// USThread
//================================================================//

//----------------------------------------------------------------//
void USThread::Clear () {

	if ( this->mImpl ) {
		delete this->mImpl;
		this->mImpl = 0;
	}
}

//----------------------------------------------------------------//
USThread::Func USThread::GetMainFunc () {

	return this->mMain;
}

//----------------------------------------------------------------//
void* USThread::GetParam () {

	return this->mParam;
}

//----------------------------------------------------------------//
USThreadState* USThread::GetState () {

	return &this->mThreadState;
}

//----------------------------------------------------------------//
bool USThread::IsCurrent () const {

	if ( this->mImpl ) {
		return this->mImpl->IsCurrent ();
	}
	return false;
}

//----------------------------------------------------------------//
bool USThread::IsRunning () const {

	if ( this->mImpl ) {
		return this->mImpl->IsRunning ();
	}
	return false;
}

//----------------------------------------------------------------//
void USThread::Join () {

	if ( this->mImpl ) {
		this->mImpl->Join ();
	}
}

//----------------------------------------------------------------//
void USThread::Sleep () {

	USThreadImpl::Sleep ();
}

//----------------------------------------------------------------//
void USThread::Start ( Func main, void* param, u32 stackSize ) {

	if ( this->mThreadState.GetState () == USThreadState::RUNNING ) return;
	this->mThreadState.SetState ( USThreadState::RUNNING );
	
	this->mMain = main;
	this->mParam = param;

	assert ( !this->mImpl );
	this->mImpl = new USThreadImpl ();
	this->mImpl->Start ( *this, stackSize );
}

//----------------------------------------------------------------//
void USThread::Stop () {

	this->mThreadState.SetState ( USThreadState::STOPPED );
}

//----------------------------------------------------------------//
USThread::USThread () :
	mImpl ( 0 ) {
}

//----------------------------------------------------------------//
USThread::~USThread () {
	this->Clear ();
}


