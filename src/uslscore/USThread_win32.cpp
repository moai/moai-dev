// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <uslscore/USThread_win32.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
DWORD __stdcall _launcher ( void* param ) {
	
	USThread* thread = ( USThread* )param;
	USThread::Func func = thread->GetMainFunc ();
	USThreadState* threadState = thread->GetState ();
	
	func ( thread->GetParam (), *threadState );
	
	//delete ( threadState );
	
	return 0;
}

//================================================================//
// USThreadImpl
//================================================================//

//----------------------------------------------------------------//
void USThreadImpl::Cleanup () {

	if ( !this->mThread ) return;
	CloseHandle ( this->mThread );
	this->mThread = 0;
	this->mID = 0;
}

//----------------------------------------------------------------//
void USThreadImpl::Join () {

	WaitForSingleObject ( this->mThread, INFINITE );
}

//----------------------------------------------------------------//
bool USThreadImpl::IsCurrent () const {

	return ( this->mID == GetCurrentThreadId ());
}

//----------------------------------------------------------------//
bool USThreadImpl::IsRunning () const {

	if ( this->mThread ) {
		DWORD exitCode = 0;
		BOOL result = GetExitCodeThread ( this->mThread, &exitCode );
		assert ( result == TRUE );
		return ( exitCode == STILL_ACTIVE );
	}
	return false;
}

//----------------------------------------------------------------//
void USThreadImpl::Sleep () {

	::Sleep ( 0 );
}

//----------------------------------------------------------------//
void USThreadImpl::Start ( USThread& thread, u32 stackSize ) {

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
USThreadImpl::USThreadImpl () :
	mThread ( 0 ),
	mID ( 0 ) {
}

//----------------------------------------------------------------//
USThreadImpl::~USThreadImpl () {

	this->Cleanup ();
}

#endif
