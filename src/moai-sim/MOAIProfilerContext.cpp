// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfiler.h>
#include <moai-sim/MOAIProfilerContext.h>
	
#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerContext
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerContext::BeginFrame ( const bool profilingEnabled ) {
	
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
MOAIProfilerScope* MOAIProfilerContext::CreateScope () {
	
	MOAIProfilerScope* scope = 0;

	if ( mScopePool ) {
		
		scope = mScopePool;
		mScopePool = mScopePool->mNext;

		scope->Reset ();
	}
	else {

		scope = new MOAIProfilerScope ();
	}

	return scope;
}

//----------------------------------------------------------------//
void MOAIProfilerContext::DeletePool () {

	while ( mScopePool ) {

		MOAIProfilerScope* scope = mScopePool;
		mScopePool = mScopePool->mNext;
		delete scope;
	}
}

//----------------------------------------------------------------//
void MOAIProfilerContext::EndFrame () {
	
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
		if ( mName != MOAIProfiler::kMainThreadName ) {
	
			mReportMutex.Lock ();
		}

		// Analyze the data and update scope averages
		mProfileReport.BeginUpdate ();

		curFrame.mCurrentScope = 0;

		MOAIProfilerScope* scope = curFrame.mRootScope.mNext;
		while ( scope ) {

			LeaveScopes ( curFrame, scope->mParent );

			mProfileReport.EnterScope ( scope->mName );
			curFrame.mCurrentScope = scope;

			scope = scope->mNext;
		}

		// Leave the first scope
		LeaveScopes ( curFrame, &curFrame.mRootScope );		
		curFrame.mCurrentScope = 0;

		mProfileReport.EndUpdate ();
	
		// The main thread can now access the report again
		if ( mName != MOAIProfiler::kMainThreadName ) {
	
			mReportMutex.Unlock ();
		}
	}

	// Clean up and get ready for next frame
	MOAIProfilerScope* scope = curFrame.mRootScope.mNext;
	while ( scope ) {

		MOAIProfilerScope* curScope = scope;
		scope = scope->mNext;
		this->RecycleScope ( curScope );
	}
}

//----------------------------------------------------------------//
void MOAIProfilerContext::EnterScope ( const ZLHashedString& name ) {
	
	if ( !mProfilingEnabled )
		return;

	if ( !InFrame () ) {
		return;
	}
	
	Frame& curFrame = mFrames [ mCurrentFrameIdx ];

	curFrame.mNumScopesEntered++;

	MOAIProfilerScope* scope = this->CreateScope ();
	scope->Enter ( curFrame.mCurrentScope, name );

	curFrame.mLastScope->mNext = scope;
	curFrame.mLastScope = scope;

	curFrame.mCurrentScope = scope;
}

//----------------------------------------------------------------//
bool MOAIProfilerContext::InFrame () {
	
	return mFrames [ mCurrentFrameIdx ].mCurrentScope != 0;
}

//----------------------------------------------------------------//
void MOAIProfilerContext::LeaveScope ( const ZLHashedString& name ) {

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
void MOAIProfilerContext::LeaveScopes ( Frame& frame, MOAIProfilerScope* targetParent ) {
	
	// Walk up the tree to the common root and leave all the scopes
	while ( frame.mCurrentScope && targetParent != frame.mCurrentScope ) {

		const u64 startTimeMicroSec = ZLDeviceTime::GetTimeInMicroSeconds ( frame.mCurrentScope->mStartTime );
		const u32 durationMicroSec = ZLDeviceTime::GetDurationInMicroSeconds ( frame.mCurrentScope->mDuration );
		mProfileReport.LeaveScope ( frame.mCurrentScope->mName, startTimeMicroSec, durationMicroSec );

		frame.mCurrentScope = frame.mCurrentScope->mParent;
	}
}

//----------------------------------------------------------------//
MOAIProfilerReport* MOAIProfilerContext::LockProfileReport () {
	
	assert ( !mReportIsLocked );
	
	if ( !mProfilingEnabled )
		return 0;
	
	Frame& lastFrame = mFrames [ ( mCurrentFrameIdx + 1 ) % NUM_FRAMES ];
	if ( lastFrame.mNumScopesEntered == 0 )
		return 0;
	
	if ( mName != MOAIProfiler::kMainThreadName ) {
	
		mReportMutex.Lock ();
	}

	mReportIsLocked = true;

	return &mProfileReport;
}

//----------------------------------------------------------------//
MOAIProfilerContext::MOAIProfilerContext ( const ZLHashedString& name ) :
	mName ( name ),
	mProfilingEnabled ( false ),
	mCurrentFrameIdx ( 0 ),
	mScopePool ( 0 ),
	mProfileReport ( name ),
	mReportIsLocked ( false ) {
}

//----------------------------------------------------------------//
MOAIProfilerContext::~MOAIProfilerContext () {

	DeletePool ();
}

//----------------------------------------------------------------//
void MOAIProfilerContext::RecycleScope ( MOAIProfilerScope* scope ) {
	
	scope->mNext = mScopePool;
	mScopePool = scope;
}

//----------------------------------------------------------------//
void MOAIProfilerContext::UnlockProfileReport () {
	
	if ( !mProfilingEnabled )
		return;
	
	Frame& lastFrame = mFrames [ ( mCurrentFrameIdx + 1 ) % NUM_FRAMES ];
	if ( lastFrame.mNumScopesEntered == 0 )
		return;

	// If this assert fails and mNumScopesEntered is greater than zero, then there must be a race between BeginFrame() and lock/unlock
	assert ( mReportIsLocked );
	
	if ( mName != MOAIProfiler::kMainThreadName ) {
		mReportMutex.Unlock ();
	}

	mReportIsLocked = false;
}

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif
