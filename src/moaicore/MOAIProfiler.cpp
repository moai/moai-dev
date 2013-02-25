// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIProfiler.h>
#include <moaicore/MOAIThread.h>
	
#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

//================================================================//
// MOAIProfiler
//================================================================//

MOAIProfiler* MOAIProfiler::gInstance = 0;

#if USE_MOAI_PROFILER

const USHashedString kMainThreadName = USHashedString ( "main" );

#endif

//----------------------------------------------------------------//
MOAIProfiler::MOAIProfiler ()
{
#if USE_MOAI_PROFILER

	mRefCount = 0;

	memset( mContexts, 0, sizeof ( ProfilingContextPtr ) * MAX_NUM_CONTEXTS );
	mContexts [ 0 ] = new ProfilingContext ( kMainThreadName );

#endif
}
	
//----------------------------------------------------------------//
MOAIProfiler::~MOAIProfiler () {

#if USE_MOAI_PROFILER

	mContextMutex.Lock ();

	for ( u32 i = 0; i < MAX_NUM_CONTEXTS; i++ ) {

		delete mContexts [ i ];
		mContexts [ i ] = 0;
	}

	mContextMutex.Unlock ();

#endif
}

#if USE_MOAI_PROFILER

//----------------------------------------------------------------//
MOAIProfiler::ProfilingContext* MOAIProfiler::_GetProfilingContext ( const USHashedString& name, bool create ) {

	// Are we looking for the context of the main thread?
	if ( name == kMainThreadName ) {

		// Yes, so there is no need to search the contexts and to (potentially) lock the mutex
		assert ( mContexts [ 0 ] && mContexts [ 0 ]->mName == name );
		return mContexts [ 0 ];
	}

	// Find the context with the given name
	ProfilingContext* context = 0;
	for ( u32 i = 1; i < MAX_NUM_CONTEXTS; i++ ) {

		ProfilingContext* curContext = mContexts [ i ];
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

				mContexts [ i ] = new ProfilingContext ( name );
				context = mContexts [ i ];
				break;
			}
		}
	
		mContextMutex.Unlock ();
	}

	return context;
}

#endif

//----------------------------------------------------------------//
void MOAIProfiler::BeginFrame () {
	
#if USE_MOAI_PROFILER
	
	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName, true );

	if ( context )
		context->BeginFrame ( mRefCount > 0 );

#endif
}

//----------------------------------------------------------------//
void MOAIProfiler::EndFrame () {
	
#if USE_MOAI_PROFILER

	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->EndFrame ();

#endif
}

//----------------------------------------------------------------//
bool MOAIProfiler::InFrame () {
	
#if USE_MOAI_PROFILER

	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	return context ? context->InFrame () : true;

#else

	return true;

#endif
}

//----------------------------------------------------------------//
void MOAIProfiler::EnterScope ( const USHashedString& name ) {

#if USE_MOAI_PROFILER
	
	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->EnterScope ( name );

#endif
}

//----------------------------------------------------------------//
void MOAIProfiler::LeaveScope ( const USHashedString& name ) {
	
#if USE_MOAI_PROFILER

	MOAIThread* curThread = MOAIThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->LeaveScope ( name );

#endif
}

//----------------------------------------------------------------//
void MOAIProfiler::EnableProfiling ( const bool enable )
{ 
#if USE_MOAI_PROFILER

	if ( enable ) {
		mRefCount++;
	}
	else {
		mRefCount = mRefCount > 0 ? mRefCount - 1 : 0;
	}

#endif
}

#if USE_MOAI_PROFILER

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
MOAIProfileReport* MOAIProfiler::LockProfileReport ( u32 index ) {
	
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

#endif

//----------------------------------------------------------------//
MOAIProfiler& MOAIProfiler::Get () {

	if ( gInstance == 0 ) {
		gInstance = new MOAIProfiler();
	}

	return *gInstance;
}
	
#if USE_MOAI_PROFILER

//================================================================//
// MOAIProfiler::Scope
//================================================================//
MOAIProfiler::Scope::Scope ( const USHashedString& name ) :
	mNext ( 0 ),
	mParent ( 0 ),
	mName ( name ),
	mDuration ( 0 ) {
}

//----------------------------------------------------------------//
void MOAIProfiler::Scope::Reset () {

	mNext = 0;
	mParent = 0;
	mDuration = 0;
}

//----------------------------------------------------------------//
void MOAIProfiler::Scope::Enter ( Scope* parent, const USHashedString& name ) {

	this->mParent = parent;
	this->mName = name;

	USDeviceTime::GetTimeStamp ( this->mDuration );
}

//----------------------------------------------------------------//
void MOAIProfiler::Scope::Leave ( const USHashedString& name ) {

	assert ( this->mName == name );
	
	USDeviceTime::TimeStamp timeLeft;
	USDeviceTime::GetTimeStamp ( timeLeft );
	this->mDuration = timeLeft - this->mDuration;
}

//================================================================//
// MOAIProfiler::ProfilingContext
//================================================================//
MOAIProfiler::ProfilingContext::ProfilingContext ( const USHashedString& name ) :
	mName ( name ),
	mProfilingEnabled ( false ),
	mCurrentFrameIdx ( 0 ),
	mScopePool ( 0 ),
	mProfileReport ( name ),
	mReportIsLocked ( false ) {
}

//----------------------------------------------------------------//
MOAIProfiler::ProfilingContext::~ProfilingContext () {

	_DeletePool ();
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::BeginFrame ( const bool profilingEnabled ) {
	
	if ( InFrame () ) {
		EndFrame ();
	}

	if ( mProfilingEnabled != profilingEnabled )
		mProfilingEnabled = profilingEnabled;

	if ( !mProfilingEnabled )
		return;

	// Select the next frame
	mCurrentFrameIdx = ( mCurrentFrameIdx + 1 ) % NUM_FRAMES;
	Frame& curFrame = mFrames [ mCurrentFrameIdx ];

	curFrame.mNumScopesEntered = 0;

	curFrame.mCurrentScope = &curFrame.mRootScope;
	curFrame.mCurrentScope->Enter ( 0, curFrame.mCurrentScope->mName );

	curFrame.mLastScope = curFrame.mCurrentScope;
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::EndFrame () {
	
	if ( !mProfilingEnabled )
		return;

	if ( !InFrame () ) {
		return;
	}
	
	Frame& curFrame = mFrames [ mCurrentFrameIdx ];

	assert( curFrame.mCurrentScope == &curFrame.mRootScope );

	curFrame.mCurrentScope->Leave ( curFrame.mCurrentScope->mName );
	curFrame.mCurrentScope = 0;
	curFrame.mLastScope = 0;

	if ( curFrame.mNumScopesEntered > 0 ) {

		// Don't allow the main thread to access the report while we are finializing it
		if ( mName != kMainThreadName ) {
	
			mReportMutex.Lock ();
		}

		// Analyze the data and update scope averages
		mProfileReport.BeginUpdate ();

		curFrame.mCurrentScope = 0;

		Scope* scope = curFrame.mRootScope.mNext;
		while ( scope ) {

			_LeaveScopes ( curFrame, scope->mParent );

			mProfileReport.EnterScope ( scope->mName );
			curFrame.mCurrentScope = scope;

			scope = scope->mNext;
		}

		// Leave the first scope
		_LeaveScopes ( curFrame, &curFrame.mRootScope );		
		curFrame.mCurrentScope = 0;

		mProfileReport.EndUpdate ();
	
		// The main thread can now access the report again
		if ( mName != kMainThreadName ) {
	
			mReportMutex.Unlock ();
		}
	}

	// Clean up and get ready for next frame
	Scope* scope = curFrame.mRootScope.mNext;
	while ( scope ) {

		Scope* curScope = scope;
		scope = scope->mNext;
		this->_RecycleScope ( curScope );
	}
}

//----------------------------------------------------------------//
bool MOAIProfiler::ProfilingContext::InFrame () {
	
	return mFrames [ mCurrentFrameIdx ].mCurrentScope != 0;
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::EnterScope ( const USHashedString& name ) {
	
	if ( !mProfilingEnabled )
		return;

	if ( !InFrame () ) {
		return;
	}
	
	Frame& curFrame = mFrames [ mCurrentFrameIdx ];

	curFrame.mNumScopesEntered++;

	Scope* scope = this->_CreateScope ();
	scope->Enter ( curFrame.mCurrentScope, name );

	curFrame.mLastScope->mNext = scope;
	curFrame.mLastScope = scope;

	curFrame.mCurrentScope = scope;
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::LeaveScope ( const USHashedString& name ) {

	if ( !mProfilingEnabled ) {
		return;
	}

	if ( !InFrame () ) {
		return;
	}

	Frame& curFrame = mFrames [ mCurrentFrameIdx ];

	curFrame.mCurrentScope->Leave ( name );

	curFrame.mCurrentScope = curFrame.mCurrentScope->mParent;
}

//----------------------------------------------------------------//
MOAIProfileReport* MOAIProfiler::ProfilingContext::LockProfileReport () {
	
	assert ( !mReportIsLocked );
	
	if ( !mProfilingEnabled )
		return 0;
	
	Frame& lastFrame = mFrames [ ( mCurrentFrameIdx + 1 ) % NUM_FRAMES ];
	if ( lastFrame.mNumScopesEntered == 0 )
		return 0;
	
	if ( mName != kMainThreadName ) {
	
		mReportMutex.Lock ();
	}

	mReportIsLocked = true;

	return &mProfileReport;
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::UnlockProfileReport () {
	
	if ( !mProfilingEnabled )
		return;
	
	Frame& lastFrame = mFrames [ ( mCurrentFrameIdx + 1 ) % NUM_FRAMES ];
	if ( lastFrame.mNumScopesEntered == 0 )
		return;

	// If this assert fails and mNumScopesEntered is greater than zero, then there must be a race between BeginFrame() and lock/unlock
	assert ( mReportIsLocked );
	
	if ( mName != kMainThreadName ) {
	
		mReportMutex.Unlock ();
	}

	mReportIsLocked = false;
}

//----------------------------------------------------------------//
MOAIProfiler::Scope* MOAIProfiler::ProfilingContext::_CreateScope () {
	
	Scope* scope = 0;

	if ( mScopePool ) {
		
		scope = mScopePool;
		mScopePool = mScopePool->mNext;

		scope->Reset ();
	}
	else {

		scope = new Scope ();
	}

	return scope;
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::_RecycleScope ( MOAIProfiler::Scope* scope ) {
	
	scope->mNext = mScopePool;
	mScopePool = scope;
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::_DeletePool () {

	while ( mScopePool ) {

		Scope* scope = mScopePool;
		mScopePool = mScopePool->mNext;
		delete scope;
	}
}

//----------------------------------------------------------------//
void MOAIProfiler::ProfilingContext::_LeaveScopes ( Frame& frame, MOAIProfiler::Scope* targetParent ) {
	
	// Walk up the tree to the common root and leave all the scopes
	while ( frame.mCurrentScope && targetParent != frame.mCurrentScope ) {

		const u32 durationMicroSec = USDeviceTime::GetDurationInMicroSeconds ( frame.mCurrentScope->mDuration );
		mProfileReport.LeaveScope ( frame.mCurrentScope->mName, durationMicroSec );

		frame.mCurrentScope = frame.mCurrentScope->mParent;
	}
}
	
#endif	

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif
