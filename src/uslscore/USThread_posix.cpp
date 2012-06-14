// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <uslscore/USThread_posix.h>

//================================================================//
// local
//================================================================//

void* _launcher ( void* param );

//----------------------------------------------------------------//
void* _launcher ( void* param ) {

	USThread* thread = ( USThread* )param;
	USThread::Func func = thread->GetMainFunc ();
	USThreadState* threadState = thread->GetState ();
	
	func ( thread->GetParam (), *threadState );
	
	return 0;
}

//================================================================//
// USThreadImpl
//================================================================//

//----------------------------------------------------------------//
void USThreadImpl::Cleanup () {

	//if ( !this->mThread ) return;
	//CloseHandle ( this->mThread );
	//this->mThread = 0;
	//this->mID = 0;
}

//----------------------------------------------------------------//
bool USThreadImpl::IsCurrent () const {

	//return ( this->mID == GetCurrentThreadId ());
	return false;
}

//----------------------------------------------------------------//
bool USThreadImpl::IsRunning () const {

	//if ( this->mThread ) {
	//	DWORD exitCode = 0;
	//	BOOL result = GetExitCodeThread ( this->mThread, &exitCode );
	//	assert ( result == TRUE );
	//	return ( exitCode == STILL_ACTIVE );
	//}
	return false;
}

//----------------------------------------------------------------//
void USThreadImpl::Join () {

	pthread_join ( this->mThread, NULL );
}

//----------------------------------------------------------------//
void USThreadImpl::Sleep () {

#if defined( __APPLE__ )
	pthread_yield_np ();
#elif defined( __linux )
	#if defined( ANDROID ) | defined( NACL )
		sched_yield ();
	#else
		pthread_yield ();
	#endif
#else
	#error "No pthread yield function defined for this platform."
#endif

}

//----------------------------------------------------------------//
void USThreadImpl::Start ( USThread& thread, u32 stackSize ) {

	assert ( !this->IsRunning ());

	pthread_attr_t attr;
	pthread_attr_init ( &attr );
	
	if ( stackSize ) {
		pthread_attr_setstacksize ( &attr, stackSize );
	}

	pthread_create (
		&this->mThread,
		&attr,
		_launcher,
		&thread
	);

	assert ( this->mThread );
}

//----------------------------------------------------------------//
USThreadImpl::USThreadImpl () :
	mThread ( 0 ) {
}

//----------------------------------------------------------------//
USThreadImpl::~USThreadImpl () {

	this->Cleanup ();
}

#endif
