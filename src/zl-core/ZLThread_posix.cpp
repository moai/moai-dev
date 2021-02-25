// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifndef _WIN32

#include <zl-core/ZLThread_posix.h>

//================================================================//
// local
//================================================================//

void* _launcher ( void* param );

//----------------------------------------------------------------//
void* _launcher ( void* param ) {

	ZLThread* thread = ( ZLThread* )param;
	ZLThread::Func func = thread->GetMainFunc ();
	ZLThreadState* threadState = thread->GetState ();
	
	ZLThread::gThreadLocalStorage.SetCurrentThread ( thread );

	func ( thread->GetParam (), *threadState );
	
	return 0;
}

//================================================================//
// ZLThreadImpl
//================================================================//

//----------------------------------------------------------------//
void ZLThreadImpl::Cleanup () {

	//if ( !this->mThread ) return;
	//CloseHandle ( this->mThread );
	//this->mThread = 0;
	//this->mID = 0;
}

//----------------------------------------------------------------//
bool ZLThreadImpl::IsCurrent () const {

	//return ( this->mID == GetCurrentThreadId ());
	return false;
}

//----------------------------------------------------------------//
bool ZLThreadImpl::IsRunning () const {

	//if ( this->mThread ) {
	//	DWORD exitCode = 0;
	//	BOOL result = GetExitCodeThread ( this->mThread, &exitCode );
	//	assert ( result == TRUE );
	//	return ( exitCode == STILL_ACTIVE );
	//}
	return false;
}

//----------------------------------------------------------------//
void ZLThreadImpl::Join () {

	pthread_join ( this->mThread, NULL );
}

//----------------------------------------------------------------//
void ZLThreadImpl::Sleep () {

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
void ZLThreadImpl::Start ( ZLThread& thread, u32 stackSize ) {

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
ZLThreadImpl::ZLThreadImpl () :
	mThread ( 0 ) {
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
	
	return ( ZLThread* )pthread_getspecific ( mTlsKey );
}

//----------------------------------------------------------------//
ZLThreadLocalImpl::ZLThreadLocalImpl () {

	int result = pthread_key_create ( &mTlsKey, 0 );

	UNUSED ( result );
	assert ( result == 0 );
}

//----------------------------------------------------------------//
ZLThreadLocalImpl::~ZLThreadLocalImpl () {
	
	pthread_key_delete ( mTlsKey );
}

//----------------------------------------------------------------//
void ZLThreadLocalImpl::SetCurrentThread ( ZLThread* thread ) {
	
	int result = pthread_setspecific ( mTlsKey, thread );

	UNUSED ( result );
	assert ( result == 0 );
}

#endif
