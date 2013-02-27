// Copyright (c) 2012 Double Fine Productions, Inc. All Rights Reserved.

#include "pch.h"
#include <uslscore/USProfiler.h>
#include <uslscore/USThread.h>
	
#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

//================================================================//
// USProfiler
//================================================================//

USProfiler* USProfiler::gInstance = 0;

#if PROFILE_ENABLED

const USHashedString kMainThreadName = USHashedString ( "main" );

#endif

//----------------------------------------------------------------//
USProfiler::USProfiler ()
{
#if PROFILE_ENABLED

	mRefCount = 0;

	memset( mContexts, 0, sizeof ( ProfilingContextPtr ) * MAX_NUM_CONTEXTS );
	mContexts [ 0 ] = new ProfilingContext ( kMainThreadName );

#endif
}
	
//----------------------------------------------------------------//
USProfiler::~USProfiler () {

#if PROFILE_ENABLED

	mContextMutex.Lock ();

	for ( u32 i = 0; i < MAX_NUM_CONTEXTS; i++ ) {

		delete mContexts [ i ];
		mContexts [ i ] = 0;
	}

	mContextMutex.Unlock ();

#endif
}

#if PROFILE_ENABLED

//----------------------------------------------------------------//
USProfiler::ProfilingContext* USProfiler::_GetProfilingContext ( const USHashedString& name, bool create ) {

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
void USProfiler::BeginFrame () {
	
#if PROFILE_ENABLED
	
	USThread* curThread = USThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName, true );

	if ( context )
		context->BeginFrame ( mRefCount > 0 );

#endif
}

//----------------------------------------------------------------//
void USProfiler::EndFrame () {
	
#if PROFILE_ENABLED

	USThread* curThread = USThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->EndFrame ();

#endif
}

//----------------------------------------------------------------//
bool USProfiler::InFrame () {
	
#if PROFILE_ENABLED

	USThread* curThread = USThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	return context ? context->InFrame () : true;

#else

	return true;

#endif
}

//----------------------------------------------------------------//
void USProfiler::EnterScope ( const USHashedString& name ) {

#if PROFILE_ENABLED
	
	USThread* curThread = USThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->EnterScope ( name );

#endif
}

//----------------------------------------------------------------//
void USProfiler::LeaveScope ( const USHashedString& name ) {
	
#if PROFILE_ENABLED

	USThread* curThread = USThread::GetCurrentThread ();
	ProfilingContext* context = _GetProfilingContext ( curThread ? curThread->GetName () : kMainThreadName );

	if ( context )
		context->LeaveScope ( name );

#endif
}

//----------------------------------------------------------------//
void USProfiler::EnableProfiling ( const bool enable )
{ 
#if PROFILE_ENABLED

	if ( enable ) {
		mRefCount++;
	}
	else {
		mRefCount = mRefCount > 0 ? mRefCount - 1 : 0;
	}

#endif
}

#if PROFILE_ENABLED

//----------------------------------------------------------------//
u32 USProfiler::GetNumProfileReports () const {
	
	if ( mRefCount <= 0 )
		return 0;

	u32 count = 0;
	for ( u32 i = 0; i < MAX_NUM_CONTEXTS; i++ ) {

		count = mContexts [ i ] ? count + 1 : count;
	}

	return count;
}

//----------------------------------------------------------------//
USProfileReport* USProfiler::LockProfileReport ( u32 index ) {
	
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
void USProfiler::UnlockProfileReport ( u32 index ) {
	
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
USProfiler& USProfiler::Get () {

	if ( gInstance == 0 ) {
		gInstance = new USProfiler();
	}

	return *gInstance;
}
	
#if PROFILE_ENABLED

//================================================================//
// USProfiler::Scope
//================================================================//
USProfiler::Scope::Scope ( const USHashedString& name ) :
	mNext ( 0 ),
	mParent ( 0 ),
	mName ( name ),
	mStartTime ( 0 ),
	mDuration ( 0 ) {
}

//----------------------------------------------------------------//
void USProfiler::Scope::Reset () {

	mNext = 0;
	mParent = 0;
	mStartTime = 0;
	mDuration = 0;
}

//----------------------------------------------------------------//
void USProfiler::Scope::Enter ( Scope* parent, const USHashedString& name ) {

	this->mParent = parent;
	this->mName = name;

	USDeviceTime::GetTimeStamp ( this->mStartTime );
}

//----------------------------------------------------------------//
void USProfiler::Scope::Leave ( const USHashedString& name ) {

	assert ( this->mName == name );
	
	USDeviceTime::TimeStamp timeLeft;
	USDeviceTime::GetTimeStamp ( timeLeft );
	this->mDuration = timeLeft - this->mStartTime;
}

//================================================================//
// USProfiler::ProfilingContext
//================================================================//
USProfiler::ProfilingContext::ProfilingContext ( const USHashedString& name ) :
	mName ( name ),
	mProfilingEnabled ( false ),
	mCurrentFrameIdx ( 0 ),
	mScopePool ( 0 ),
	mProfileReport ( name ),
	mReportIsLocked ( false ) {
}

//----------------------------------------------------------------//
USProfiler::ProfilingContext::~ProfilingContext () {

	_DeletePool ();
}

//----------------------------------------------------------------//
void USProfiler::ProfilingContext::BeginFrame ( const bool profilingEnabled ) {
	
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
void USProfiler::ProfilingContext::EndFrame () {
	
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
bool USProfiler::ProfilingContext::InFrame () {
	
	return mFrames [ mCurrentFrameIdx ].mCurrentScope != 0;
}

//----------------------------------------------------------------//
void USProfiler::ProfilingContext::EnterScope ( const USHashedString& name ) {
	
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
void USProfiler::ProfilingContext::LeaveScope ( const USHashedString& name ) {

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
USProfileReport* USProfiler::ProfilingContext::LockProfileReport () {
	
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
void USProfiler::ProfilingContext::UnlockProfileReport () {
	
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
USProfiler::Scope* USProfiler::ProfilingContext::_CreateScope () {
	
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
void USProfiler::ProfilingContext::_RecycleScope ( USProfiler::Scope* scope ) {
	
	scope->mNext = mScopePool;
	mScopePool = scope;
}

//----------------------------------------------------------------//
void USProfiler::ProfilingContext::_DeletePool () {

	while ( mScopePool ) {

		Scope* scope = mScopePool;
		mScopePool = mScopePool->mNext;
		delete scope;
	}
}

//----------------------------------------------------------------//
void USProfiler::ProfilingContext::_LeaveScopes ( Frame& frame, USProfiler::Scope* targetParent ) {
	
	// Walk up the tree to the common root and leave all the scopes
	while ( frame.mCurrentScope && targetParent != frame.mCurrentScope ) {

		const u64 startTimeMicroSec = USDeviceTime::GetTimeInMicroSeconds ( frame.mCurrentScope->mStartTime );
		const u32 durationMicroSec = USDeviceTime::GetDurationInMicroSeconds ( frame.mCurrentScope->mDuration );
		mProfileReport.LeaveScope ( frame.mCurrentScope->mName, startTimeMicroSec, durationMicroSec );

		frame.mCurrentScope = frame.mCurrentScope->mParent;
	}
}
	
#endif	

#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif
