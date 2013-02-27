// Copyright (c) 2012 Double Fine Productions, Inc. All Rights Reserved.

#ifndef	PROFILER_H
#define	PROFILER_H

#include <uslscore/STLList.h>
#include <uslscore/USMutex.h>
#include <uslscore/USDeviceTime.h>
#include <uslscore/USProfileScope.h>
#include <uslscore/USProfileReport.h>
#include <uslscore/USProfileConfig.h>

//================================================================//
// USProfiler
//================================================================//
class USProfiler {
	
private:
	
#if PROFILE_ENABLED

	//----------------------------------------------------------------//
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
	
	//----------------------------------------------------------------//
	class ProfilingContext {

	private:

		enum {

			NUM_FRAMES = 2
		};

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

		USProfileReport*			LockProfileReport ();
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

		USProfileReport				mProfileReport;
		
		USMutex						mReportMutex;
		bool						mReportIsLocked;
	};

	typedef ProfilingContext*	ProfilingContextPtr;

#endif

public:
	//----------------------------------------------------------------//
									~USProfiler ();

	void							BeginFrame ();
	void							EndFrame ();
	bool							InFrame ();
	
	void							EnterScope ( const USHashedString& name );
	void							LeaveScope ( const USHashedString& name );
	
	void							EnableProfiling ( const bool enable );

#if PROFILE_ENABLED
	
	//----------------------------------------------------------------//
	u32								GetNumProfileReports () const;
	USProfileReport*				LockProfileReport ( u32 index );
	void							UnlockProfileReport ( u32 index );
	
#endif
	
	//----------------------------------------------------------------//
	static USProfiler&				Get ();

private:

	//----------------------------------------------------------------//
									USProfiler ();

#if PROFILE_ENABLED

	//----------------------------------------------------------------//
	ProfilingContext*				_GetProfilingContext ( const USHashedString& name, bool create = false );
	
	//----------------------------------------------------------------//
	enum {

		MAX_NUM_CONTEXTS = 8,
	};

	s16								mRefCount;

	ProfilingContextPtr				mContexts [ MAX_NUM_CONTEXTS ];
	USMutex							mContextMutex;

#endif
	
	//----------------------------------------------------------------//
	static USProfiler*				gInstance;
};

#endif
