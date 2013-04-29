// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfiler.h>
	
#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfiler
//================================================================//

const ZLHashedString MOAIProfiler::kMainThreadName ( "main" );

//----------------------------------------------------------------//
void MOAIProfiler::BeginFrame () {
	
	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	MOAIProfilerContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName, true );

	if ( context )
		context->BeginFrame ( mRefCount > 0 );
}

//----------------------------------------------------------------//
void MOAIProfiler::EnableProfiling ( const bool enable ) {

	if ( enable ) {
		mRefCount++;
	}
	else {
		mRefCount = mRefCount > 0 ? mRefCount - 1 : 0;
	}
}

//----------------------------------------------------------------//
void MOAIProfiler::EndFrame () {

	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	MOAIProfilerContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->EndFrame ();
}

//----------------------------------------------------------------//
void MOAIProfiler::EnterScope ( const ZLHashedString& name ) {
	
	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	MOAIProfilerContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->EnterScope ( name );
}

//----------------------------------------------------------------//
u32 MOAIProfiler::GetNumProfileReports () const {
	
	if ( mRefCount <= 0 )
		return 0;

	u32 count = 0;
	for ( u32 i = 0; i < MAX_NUM_CONTEXTS; i++ ) {

		count = mContexts [ i ] ? count + 1 : count;
	}

	return count;
}

//----------------------------------------------------------------//
MOAIProfilerContext* MOAIProfiler::_GetProfilingContext ( const ZLHashedString& name, bool create ) {

	// Are we looking for the context of the main thread?
	if ( name == kMainThreadName ) {

		// Yes, so there is no need to search the contexts and to (potentially) lock the mutex
		assert ( mContexts [ 0 ] && mContexts [ 0 ]->mName == name );
		return mContexts [ 0 ];
	}

	// Find the context with the given name
	MOAIProfilerContext* context = 0;
	for ( u32 i = 1; i < MAX_NUM_CONTEXTS; i++ ) {

		MOAIProfilerContext* curContext = mContexts [ i ];
		if ( curContext && curContext->mName == name ) {

			context = curContext;
			break;
		}
	}

	// Couldn't find the context, so let's try to create a new one
	if ( context == 0 && create ) {
		
		// Avoid race conditions between different threads
		mContextMutex.Lock ();

		for ( u32 i = 1; i < MAX_NUM_CONTEXTS; i++ ) {

			if ( mContexts [ i ] == 0 ) {

				mContexts [ i ] = new MOAIProfilerContext ( name );
				context = mContexts [ i ];
				break;
			}
		}
	
		mContextMutex.Unlock ();
	}

	return context;
}

//----------------------------------------------------------------//
bool MOAIProfiler::InFrame () {

	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	MOAIProfilerContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	return context ? context->InFrame () : true;
}

//----------------------------------------------------------------//
void MOAIProfiler::LeaveScope ( const ZLHashedString& name ) {

	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	MOAIProfilerContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->LeaveScope ( name );
}

//----------------------------------------------------------------//
MOAIProfilerReport* MOAIProfiler::LockProfileReport ( u32 index ) {
	
	if ( mRefCount <= 0 )
		return 0;

	// Note: This code assumes that no contexts will be removed while they are locked	
	u32 count = 0;
	for ( u32 i = 0; i < MAX_NUM_CONTEXTS; i++ ) {

		if ( mContexts [ i ] ) {

			if ( count == index ) {

				return mContexts [ i ]->LockProfileReport ();
			}

			count++;
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
MOAIProfiler::MOAIProfiler () {

	mRefCount = 0;

	memset( mContexts, 0, sizeof ( MOAIProfilerContext* ) * MAX_NUM_CONTEXTS );
	mContexts [ 0 ] = new MOAIProfilerContext ( kMainThreadName );
}
	
//----------------------------------------------------------------//
MOAIProfiler::~MOAIProfiler () {

	mContextMutex.Lock ();

	for ( u32 i = 0; i < MAX_NUM_CONTEXTS; i++ ) {

		delete mContexts [ i ];
		mContexts [ i ] = 0;
	}

	mContextMutex.Unlock ();
}

//----------------------------------------------------------------//
void MOAIProfiler::UnlockProfileReport ( u32 index ) {
	
	if ( mRefCount <= 0 )
		return;

	// Note: This code assumes that no contexts will be removed while they are locked	
	u32 count = 0;
	for ( u32 i = 0; i < MAX_NUM_CONTEXTS; i++ ) {

		if ( mContexts [ i ] ) {

			if ( count == index ) {

				mContexts [ i ]->UnlockProfileReport ();
				break;
			}

			count++;
		}
	}
}

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif
