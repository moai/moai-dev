// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <moaicore/MOAIThread_posix.h>

//================================================================//
// local
//================================================================//

void* _launcher ( void* param );

//----------------------------------------------------------------//
void* _launcher ( void* param ) {

	MOAIThread* thread = ( MOAIThread* )param;
	MOAIThread::Func func = thread->GetMainFunc ();
	MOAIThreadState* threadState = thread->GetState ();
	
	func ( thread->GetParam (), *threadState );
	
	return 0;
}

//================================================================//
// MOAIThreadImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIThreadImpl::Cleanup () {

	//if ( !this->mThread ) return;
	//CloseHandle ( this->mThread );
	//this->mThread = 0;
	//this->mID = 0;
}

//----------------------------------------------------------------//
bool MOAIThreadImpl::IsCurrent () const {

	//return ( this->mID == GetCurrentThreadId ());
	return false;
}

//----------------------------------------------------------------//
bool MOAIThreadImpl::IsRunning () const {

	//if ( this->mThread ) {
	//	DWORD exitCode = 0;
	//	BOOL result = GetExitCodeThread ( this->mThread, &exitCode );
	//	assert ( result == TRUE );
	//	return ( exitCode == STILL_ACTIVE );
	//}
	return false;
}

//----------------------------------------------------------------//
void MOAIThreadImpl::Join () {

	pthread_join ( this->mThread, NULL );
}

//----------------------------------------------------------------//
MOAIThreadImpl::MOAIThreadImpl () :
	mThread ( 0 ) {
}

//----------------------------------------------------------------//
MOAIThreadImpl::~MOAIThreadImpl () {

	this->Cleanup ();
}

//----------------------------------------------------------------//
void MOAIThreadImpl::Sleep () {

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
void MOAIThreadImpl::Start ( MOAIThread& thread, u32 stackSize ) {

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

#endif
