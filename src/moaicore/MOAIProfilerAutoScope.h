// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILERAUTOSCOPE_H
#define	MOAIPROFILERAUTOSCOPE_H

#if USE_MOAI_PROFILER
	#define PROFILER_SCOPE(name)										\
		static USHashedString profileScopeName_#name ( name );			\
		MOAIProfilerAutoScope profileScope_#name (  );					\
		profileScope_#name.EnterScope ( profileScopeName_#name );
#else
	#define PROFILER_SCOPE(name)
#endif

//================================================================//
// MOAIProfilerAutoScope
//================================================================//
class MOAIProfilerAutoScope {
private:

	const USHashedString* mName;

public:

	//----------------------------------------------------------------//
	void	EnterScope				( const USHashedString& name );
			MOAIProfilerAutoScope	();
			~MOAIProfilerAutoScope	();
	
};

#endif
