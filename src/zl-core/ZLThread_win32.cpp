// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifdef _WIN32

#include <zl-core/ZLThread_win32.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
DWORD __stdcall _launcher ( void* param ) {
	
	ZLThread* thread = ( ZLThread* )param;
	ZLThread::Func func = thread->GetMainFunc ();
	ZLThreadState* threadState = thread->GetState ();

	ZLThread::gThreadLocalStorage.SetCurrentThread ( thread );
	
	func ( thread->GetParam (), *threadState );
	
	//delete ( threadState );
	
	return 0;
}

//================================================================//
// ZLThreadImpl
//================================================================//

//----------------------------------------------------------------//
void ZLThreadImpl::Cleanup () {

	if ( !this->mThread ) return;
	CloseHandle ( this->mThread );
	this->mThread = 0;
	this->mID = 0;
}

//----------------------------------------------------------------//
void ZLThreadImpl::Join () {

	WaitForSingleObject ( this->mThread, INFINITE );
}

//----------------------------------------------------------------//
bool ZLThreadImpl::IsCurrent () const {

	return ( this->mID == GetCurrentThreadId ());
}

//----------------------------------------------------------------//
bool ZLThreadImpl::IsRunning () const {

	if ( this->mThread ) {
		DWORD exitCode = 0;
		BOOL result = GetExitCodeThread ( this->mThread, &exitCode );
		UNUSED ( result ); // TODO: should be fixed by assert undef
		assert ( result == TRUE );
		return ( exitCode == STILL_ACTIVE );
	}
	return false;
}

//----------------------------------------------------------------//
void ZLThreadImpl::Sleep () {

	::Sleep ( 0 );
}

//----------------------------------------------------------------//
void ZLThreadImpl::Start ( ZLThread& thread, u32 stackSize ) {

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
ZLThreadImpl::ZLThreadImpl () :
	mThread ( 0 ),
	mID ( 0 ) {
}

//----------------------------------------------------------------//
ZLThreadImpl::~ZLThreadImpl () {

	this->Cleanup ();
}

//================================================================//
// ZLThreadLocalImpl
//================================================================//

//----------------------------------------------------------------//
ZLThread* ZLThreadLocalImpl::GetCurrentThread () const {

	return (ZLThread*) ::TlsGetValue ( mTlsIndex );
}

//----------------------------------------------------------------//
ZLThreadLocalImpl::ZLThreadLocalImpl () {

	mTlsIndex = ::TlsAlloc ();
	assert ( mTlsIndex != 0 );
}

//----------------------------------------------------------------//
ZLThreadLocalImpl::~ZLThreadLocalImpl () {
	
	::TlsFree ( mTlsIndex );
}

//----------------------------------------------------------------//
void ZLThreadLocalImpl::SetCurrentThread ( ZLThread* thread ) {
	
	BOOL success = ::TlsSetValue( mTlsIndex, thread );
	UNUSED ( success ); // TODO: should be fixed by assert undef
	assert ( success == TRUE );
}

#endif
