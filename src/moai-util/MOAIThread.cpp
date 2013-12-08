// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIThread.h>
#include <moai-util/MOAIThread_posix.h>
#include <moai-util/MOAIThread_win32.h>

//================================================================//
// MOAIThreadState
//================================================================//

//----------------------------------------------------------------//
u32 MOAIThreadState::GetState () {
	
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
bool MOAIThreadState::IsPaused () {

	return ( this->GetState () == PAUSED );
}

//----------------------------------------------------------------//
bool MOAIThreadState::IsRunning () {

	return ( this->GetState () == RUNNING );
}

//----------------------------------------------------------------//
bool MOAIThreadState::IsStopped () {

	return ( this->GetState () == STOPPED );
}

//----------------------------------------------------------------//
MOAIThreadState::MOAIThreadState () :
	mState ( NEW ) {
}

//----------------------------------------------------------------//
MOAIThreadState::~MOAIThreadState () {
}

//----------------------------------------------------------------//
void MOAIThreadState::SetState ( u32 state ) {
	
	#ifdef _WIN32
		this->mState = state;
	#else
		this->mMutex.Lock ();
		this->mState = state;
		this->mMutex.Unlock ();
	#endif
}

//================================================================//
// MOAIThread
//================================================================//

 MOAIThreadLocalImpl MOAIThread::gThreadLocalStorage;

//----------------------------------------------------------------//
void MOAIThread::Clear () {

	if ( this->mImpl ) {
		delete this->mImpl;
		this->mImpl = 0;
	}
}

//----------------------------------------------------------------//
MOAIThread::Func MOAIThread::GetMainFunc () {

	return this->mMain;
}

//----------------------------------------------------------------//
void* MOAIThread::GetParam () {

	return this->mParam;
}

//----------------------------------------------------------------//
MOAIThreadState* MOAIThread::GetState () {

	return &this->mThreadState;
}

//----------------------------------------------------------------//
bool MOAIThread::IsCurrent () const {

	if ( this->mImpl ) {
		return this->mImpl->IsCurrent ();
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIThread::IsRunning () const {

	if ( this->mImpl ) {
		return this->mImpl->IsRunning ();
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIThread::Join () {

	if ( this->mImpl ) {
		this->mImpl->Join ();
	}
}

//----------------------------------------------------------------//
MOAIThread::MOAIThread () :
	mImpl ( 0 ),
	mName ( ZLHashedString::Empty ) {
}

//----------------------------------------------------------------//
MOAIThread::~MOAIThread () {
	this->Clear ();
}

//----------------------------------------------------------------//
MOAIThread* MOAIThread::GetCurrentThread () {

	return MOAIThread::gThreadLocalStorage.GetCurrentThread ();
}

//----------------------------------------------------------------//
void MOAIThread::Sleep () {

	MOAIThreadImpl::Sleep ();
}

//----------------------------------------------------------------//
void MOAIThread::Start ( Func main, void* param, u32 stackSize ) {

	if ( this->mThreadState.GetState () == MOAIThreadState::RUNNING ) return;
	this->mThreadState.SetState ( MOAIThreadState::RUNNING );
	
	this->mMain = main;
	this->mParam = param;

	assert ( !this->mImpl );
	this->mImpl = new MOAIThreadImpl ();
	this->mImpl->Start ( *this, stackSize );
}

//----------------------------------------------------------------//
void MOAIThread::Stop () {

	this->mThreadState.SetState ( MOAIThreadState::STOPPED );
}
