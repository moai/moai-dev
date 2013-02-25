// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILER_H
#define	MOAIPROFILER_H

#include <moaicore/MOAIMutex.h>
#include <moaicore/MOAIProfileReport.h>
#include <moaicore/MOAIProfileScope.h>

//================================================================//
// MOAIProfiler
//================================================================//
class MOAIProfiler {
private:
	
#if USE_MOAI_PROFILER

	//================================================================//
	// Scope
	//================================================================//
	struct Scope {
		
		//----------------------------------------------------------------//
		Scope*						mNext;

		//----------------------------------------------------------------//
		Scope*						mParent;
		USHashedString				mName;
		USDeviceTime::TimeStamp		mDuration;

		//----------------------------------------------------------------//
									Scope ( const USHashedString& name = USHashedString::Empty );
		void						Reset ();
		void						Enter ( Scope* parent, const USHashedString& name );
		void						Leave ( const USHashedString& name );
	};
	
	//================================================================//
	// ProfilingContext
	//================================================================//
	class ProfilingContext {
	private:

		enum {

			NUM_FRAMES = 2
		};

		//================================================================//
		// Frame
		//================================================================//
		struct Frame {
									Frame () : mRootScope ( USHashedString ( "_root" ) ), mCurrentScope ( 0 ), mLastScope ( 0 ), mNumScopesEntered ( 0 ) {}
			
			Scope					mRootScope;

			Scope*					mCurrentScope;
			Scope*					mLastScope;

			u32						mNumScopesEntered;
		};
		
	public:

		//----------------------------------------------------------------//
									ProfilingContext ( const USHashedString& name );
									~ProfilingContext ();
		
		//----------------------------------------------------------------//
		void						BeginFrame ( const bool profilingEnabled );
		void						EndFrame ();
		bool						InFrame ();
	
		void						EnterScope ( const USHashedString& name );
		void						LeaveScope ( const USHashedString& name );

		MOAIProfileReport*			LockProfileReport ();
		void						UnlockProfileReport ();

		//----------------------------------------------------------------//
		Scope*						_CreateScope ();
		void						_RecycleScope ( Scope* scope );
		void						_DeletePool ();

		void						_LeaveScopes ( Frame& frame, Scope* targetParent );
	
		//----------------------------------------------------------------//
		USHashedString				mName;

		bool						mProfilingEnabled;

		Frame						mFrames [ NUM_FRAMES ];
		u32							mCurrentFrameIdx;
		
		Scope*						mScopePool;

		MOAIProfileReport			mProfileReport;
		
		MOAIMutex						mReportMutex;
		bool						mReportIsLocked;
	};

	typedef ProfilingContext*		ProfilingContextPtr;

#endif

public:
	//----------------------------------------------------------------//
									~MOAIProfiler ();

	void							BeginFrame ();
	void							EndFrame ();
	bool							InFrame ();
	
	void							EnterScope ( const USHashedString& name );
	void							LeaveScope ( const USHashedString& name );
	
	void							EnableProfiling ( const bool enable );

#if USE_MOAI_PROFILER
	
	//----------------------------------------------------------------//
	u32								GetNumProfileReports () const;
	MOAIProfileReport*				LockProfileReport ( u32 index );
	void							UnlockProfileReport ( u32 index );
	
#endif
	
	//----------------------------------------------------------------//
	static MOAIProfiler&			Get ();

private:

	//----------------------------------------------------------------//
									MOAIProfiler ();

#if USE_MOAI_PROFILER

	//----------------------------------------------------------------//
	ProfilingContext*				_GetProfilingContext ( const USHashedString& name, bool create = false );
	
	//----------------------------------------------------------------//
	enum {

		MAX_NUM_CONTEXTS = 8,
	};

	s16								mRefCount;

	ProfilingContextPtr				mContexts [ MAX_NUM_CONTEXTS ];
	MOAIMutex							mContextMutex;

#endif
	
	//----------------------------------------------------------------//
	static MOAIProfiler*			gInstance;
};

#endif
