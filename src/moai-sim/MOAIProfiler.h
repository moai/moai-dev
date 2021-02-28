// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILER_H
#define	MOAIPROFILER_H

#include <moai-sim/MOAIProfilerReport.h>
#include <moai-sim/MOAIProfilerContext.h>
#include <moai-sim/MOAIProfilerScope.h>

#if 0

//================================================================//
// MOAIProfiler
//================================================================//
class MOAIProfiler :
	public virtual ZLContextClass {
private:

	enum {
		MAX_NUM_CONTEXTS = 8,
	};

	s16							mRefCount;
	MOAIProfilerContext*		mContexts [ MAX_NUM_CONTEXTS ];
	ZLMutex					mContextMutex;

	//----------------------------------------------------------------//
	MOAIProfilerContext*		_GetProfilingContext		( const ZLHashedString& name, bool create = false );
								
public:

	static const ZLHashedString		kMainThreadName;

	//----------------------------------------------------------------//
	void						BeginFrame					();
	void						EnableProfiling				( const bool enable );
	void						EndFrame					();
	void						EnterScope					( const ZLHashedString& name );
	u32							GetNumProfileReports		() const;
	bool						InFrame						();
	void						LeaveScope					( const ZLHashedString& name );
	MOAIProfilerReport*			LockProfileReport			( u32 index );
								MOAIProfiler				();
								~MOAIProfiler				();
	void						UnlockProfileReport			( u32 index );
};

#endif
#endif
