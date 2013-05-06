// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERCONTEXT_H
#define	MOAIPROFILERCONTEXT_H

#include <moai-sim/MOAIProfilerReport.h>
#include <moai-sim/MOAIProfilerScope.h>

//================================================================//
// MOAIProfilerContext
//================================================================//
class MOAIProfilerContext {
private:

	friend class MOAIProfiler;

	enum {
		NUM_FRAMES = 2
	};

	//================================================================//
	// Frame
	//================================================================//
	struct Frame {
								
		
		MOAIProfilerScope			mRootScope;

		MOAIProfilerScope*			mCurrentScope;
		MOAIProfilerScope*			mLastScope;

		u32							mNumScopesEntered;
		
		//----------------------------------------------------------------//
		Frame () :
			mRootScope ( ZLHashedString ( "_root" )),
			mCurrentScope ( 0 ),
			mLastScope ( 0 ),
			mNumScopesEntered ( 0 ) {
		}
	};

	ZLHashedString				mName;

	bool						mProfilingEnabled;

	Frame						mFrames [ NUM_FRAMES ];
	u32							mCurrentFrameIdx;
	
	MOAIProfilerScope*			mScopePool;

	MOAIProfilerReport			mProfileReport;
	
	MOAIMutex					mReportMutex;
	bool						mReportIsLocked;

	//----------------------------------------------------------------//
	MOAIProfilerScope*			CreateScope				();
	void						DeletePool				();
	void						LeaveScopes				( Frame& frame, MOAIProfilerScope* targetParent );
	void						RecycleScope			( MOAIProfilerScope* scope );

public:
	
	//----------------------------------------------------------------//
	void						BeginFrame				( const bool profilingEnabled );
	void						EndFrame				();
	void						EnterScope				( const ZLHashedString& name );
	bool						InFrame					();
	void						LeaveScope				( const ZLHashedString& name );
	MOAIProfilerReport*			LockProfileReport		();
								MOAIProfilerContext		( const ZLHashedString& name );
								~MOAIProfilerContext	();
	void						UnlockProfileReport		();
};

#endif
