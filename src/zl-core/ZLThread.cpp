// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLThread.h>
#include <zl-core//ZLThread_posix.h>
#include <zl-core//ZLThread_win32.h>

//================================================================//
// ZLThreadState
//================================================================//

//----------------------------------------------------------------//
u32 ZLThreadState::GetState () {
	
	#ifdef _WIN32
		return this->mState;
	#else
		u32 state;

		this->mMutex.Lock ();
		state = this->mState;
		this->mMutex.Unlock ();
		
		return state;
	#endif
}

//----------------------------------------------------------------//
bool ZLThreadState::IsPaused () {

	return ( this->GetState () == PAUSED );
}

//----------------------------------------------------------------//
bool ZLThreadState::IsRunning () {

	return ( this->GetState () == RUNNING );
}

//----------------------------------------------------------------//
bool ZLThreadState::IsStopped () {

	return ( this->GetState () == STOPPED );
}

//----------------------------------------------------------------//
ZLThreadState::ZLThreadState () :
	mState ( NEW ) {
}

//----------------------------------------------------------------//
ZLThreadState::~ZLThreadState () {
}

//----------------------------------------------------------------//
void ZLThreadState::SetState ( u32 state ) {
	
	#ifdef _WIN32
		this->mState = state;
	#else
		this->mMutex.Lock ();
		this->mState = state;
		this->mMutex.Unlock ();
	#endif
}

//================================================================//
// ZLThread
//================================================================//

 ZLThreadLocalImpl ZLThread::gThreadLocalStorage;

//----------------------------------------------------------------//
void ZLThread::Clear () {

	if ( this->mImpl ) {
		delete this->mImpl;
		this->mImpl = 0;
	}
}

//----------------------------------------------------------------//
ZLThread::Func ZLThread::GetMainFunc () {

	return this->mMain;
}

//----------------------------------------------------------------//
void* ZLThread::GetParam () {

	return this->mParam;
}

//----------------------------------------------------------------//
ZLThreadState* ZLThread::GetState () {

	return &this->mThreadState;
}

//----------------------------------------------------------------//
bool ZLThread::IsCurrent () const {

	if ( this->mImpl ) {
		return this->mImpl->IsCurrent ();
	}
	return false;
}

//----------------------------------------------------------------//
bool ZLThread::IsRunning () const {

	if ( this->mImpl ) {
		return this->mImpl->IsRunning ();
	}
	return false;
}

//----------------------------------------------------------------//
void ZLThread::Join () {

	if ( this->mImpl ) {
		this->mImpl->Join ();
	}
}

//----------------------------------------------------------------//
ZLThread::ZLThread () :
	mImpl ( 0 ) {
}

//----------------------------------------------------------------//
ZLThread::~ZLThread () {
	this->Clear ();
}

//----------------------------------------------------------------//
ZLThread* ZLThread::GetCurrentThread () {

	return ZLThread::gThreadLocalStorage.GetCurrentThread ();
}

//----------------------------------------------------------------//
void ZLThread::Sleep () {

	ZLThreadImpl::Sleep ();
}

//----------------------------------------------------------------//
void ZLThread::Start ( Func main, void* param, u32 stackSize ) {

	if ( this->mThreadState.GetState () == ZLThreadState::RUNNING ) return;
	this->mThreadState.SetState ( ZLThreadState::RUNNING );
	
	this->mMain = main;
	this->mParam = param;

	assert ( !this->mImpl );
	this->mImpl = new ZLThreadImpl ();
	this->mImpl->Start ( *this, stackSize );
}

//----------------------------------------------------------------//
void ZLThread::Stop () {

	this->mThreadState.SetState ( ZLThreadState::STOPPED );
}
