// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILER_H
#define	MOAIPROFILER_H

#include <moaicore/MOAIMutex.h>
#include <moaicore/MOAIProfilerReport.h>
#include <moaicore/MOAIProfilerContext.h>
#include <moaicore/MOAIProfilerScope.h>

//================================================================//
// MOAIProfiler
//================================================================//
class MOAIProfiler {
public:

	static const USHashedString		kMainThreadName;

	//----------------------------------------------------------------//
	void							BeginFrame ();
	void							EndFrame ();
	bool							InFrame ();
	
	void							EnterScope ( const USHashedString& name );
	void							LeaveScope ( const USHashedString& name );
	
	void							EnableProfiling ( const bool enable );
									~MOAIProfiler ();

#if USE_MOAI_PROFILER
	
	//----------------------------------------------------------------//
	u32								GetNumProfileReports () const;
	MOAIProfilerReport*				LockProfileReport ( u32 index );
	void							UnlockProfileReport ( u32 index );
	
#endif
	
	//----------------------------------------------------------------//
	static MOAIProfiler&			Get ();

private:

	//----------------------------------------------------------------//
									MOAIProfiler ();

#if USE_MOAI_PROFILER

	//----------------------------------------------------------------//
	MOAIProfilerContext*			_GetProfilingContext ( const USHashedString& name, bool create = false );
	
	//----------------------------------------------------------------//
	enum {
		MAX_NUM_CONTEXTS = 8,
	};

	s16								mRefCount;

	MOAIProfilerContext*			mContexts [ MAX_NUM_CONTEXTS ];
	MOAIMutex						mContextMutex;

#endif
	
	//----------------------------------------------------------------//
	static MOAIProfiler*			gInstance;
};

#endif
