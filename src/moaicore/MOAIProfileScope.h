// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROFILESCOPE_H
#define	MOAIPROFILESCOPE_H

//================================================================//
// MOAIProfileScope
//================================================================//
#if USE_MOAI_PROFILER

	struct MOAIProfileScope {
	public:

		//----------------------------------------------------------------//
							MOAIProfileScope ( USHashedString& name );
							~MOAIProfileScope ();

		//----------------------------------------------------------------//
		USHashedString&		mName;

		//----------------------------------------------------------------//
		// Fix Visual Studio compiler warning
		MOAIProfileScope&		operator = ( const MOAIProfileScope& other ) { UNUSED ( other ); assert ( 0 ); return *this; }
	};

#endif

#if USE_MOAI_PROFILER
	#define PROFILE_SCOPE(name) static USHashedString __profile_scope_name(name); MOAIProfileScope __profile_scope(__profile_scope_name);
#else
	#define PROFILE_SCOPE(name)
#endif

#endif
