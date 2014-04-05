// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <moai-util/MOAIThread_win32.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
DWORD __stdcall _launcher ( void* param ) {
	
	MOAIThread* thread = ( MOAIThread* )param;
	MOAIThread::Func func = thread->GetMainFunc ();
	MOAIThreadState* threadState = thread->GetState ();

	MOAIThread::gThreadLocalStorage.SetCurrentThread ( thread );
	
	func ( thread->GetParam (), *threadState );
	
	//delete ( threadState );
	
	return 0;
}

//================================================================//
// MOAIThreadImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIThreadImpl::Cleanup () {

	if ( !this->mThread ) return;
	CloseHandle ( this->mThread );
	this->mThread = 0;
	this->mID = 0;
}

//----------------------------------------------------------------//
void MOAIThreadImpl::Join () {

	WaitForSingleObject ( this->mThread, INFINITE );
}

//----------------------------------------------------------------//
bool MOAIThreadImpl::IsCurrent () const {

	return ( this->mID == GetCurrentThreadId ());
}

//----------------------------------------------------------------//
bool MOAIThreadImpl::IsRunning () const {

	if ( this->mThread ) {
		DWORD exitCode = 0;
		BOOL result = GetExitCodeThread ( this->mThread, &exitCode );
		assert ( result == TRUE );
		return ( exitCode == STILL_ACTIVE );
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIThreadImpl::Sleep () {

	::Sleep ( 0 );
}

//----------------------------------------------------------------//
void MOAIThreadImpl::Start ( MOAIThread& thread, u32 stackSize ) {

	assert ( !this->IsRunning ());

	this->mThread = CreateThread (
		NULL,
		stackSize,
		_launcher,
		&thread,
		0,
		&this->mID
	);

	assert ( this->mThread );
}

//----------------------------------------------------------------//
MOAIThreadImpl::MOAIThreadImpl () :
	mThread ( 0 ),
	mID ( 0 ) {
}

//----------------------------------------------------------------//
MOAIThreadImpl::~MOAIThreadImpl () {

	this->Cleanup ();
}

//================================================================//
// MOAIThreadLocalImpl
//================================================================//

//----------------------------------------------------------------//
MOAIThread* MOAIThreadLocalImpl::GetCurrentThread () const {

	return (MOAIThread*) ::TlsGetValue ( mTlsIndex );
}

//----------------------------------------------------------------//
MOAIThreadLocalImpl::MOAIThreadLocalImpl () {

	mTlsIndex = ::TlsAlloc ();
	assert ( mTlsIndex != 0 );
}

//----------------------------------------------------------------//
MOAIThreadLocalImpl::~MOAIThreadLocalImpl () {
	
	::TlsFree ( mTlsIndex );
}

//----------------------------------------------------------------//
void MOAIThreadLocalImpl::SetCurrentThread ( MOAIThread* thread ) {
	
	BOOL success = ::TlsSetValue( mTlsIndex, thread );
	assert ( success == TRUE );
}

#endif
