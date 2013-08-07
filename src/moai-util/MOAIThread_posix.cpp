// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifndef _WIN32

#include <moai-util/MOAIThread_posix.h>

//================================================================//
// local
//================================================================//

void* _launcher ( void* param );

//----------------------------------------------------------------//
void* _launcher ( void* param ) {

	MOAIThread* thread = ( MOAIThread* )param;
	MOAIThread::Func func = thread->GetMainFunc ();
	MOAIThreadState* threadState = thread->GetState ();
	
	MOAIThread::gThreadLocalStorage.SetCurrentThread ( thread );

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
void MOAIThreadImpl::Sleep () {

	#if defined( __APPLE__ )
		pthread_yield_np ();
	#elif defined( __linux ) | defined ( __FLASCC__ ) | defined ( __QNX__ ) | defined ( __EMSCRIPTEN__ )
		#if defined( ANDROID ) | defined( NACL ) | defined ( __QNX__ ) | defined ( __EMSCRIPTEN__ )
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

//----------------------------------------------------------------//
MOAIThreadImpl::MOAIThreadImpl () :
	mThread ( 0 ) {
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
	
	return ( MOAIThread* )pthread_getspecific ( mTlsKey );
}

//----------------------------------------------------------------//
MOAIThreadLocalImpl::MOAIThreadLocalImpl () {

	int result = pthread_key_create ( &mTlsKey, 0 );
	assert ( result == 0 );
}

//----------------------------------------------------------------//
MOAIThreadLocalImpl::~MOAIThreadLocalImpl () {
	
	pthread_key_delete ( mTlsKey );
}

//----------------------------------------------------------------//
void MOAIThreadLocalImpl::SetCurrentThread ( MOAIThread* thread ) {
	
	int result = pthread_setspecific ( mTlsKey, thread );
	assert ( result == 0 );
}

#endif
