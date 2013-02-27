// Copyright (c) 2012 Double Fine Productions, Inc. All Rights Reserved.

#ifndef	PROFILE_SCOPE_H
#define	PROFILE_SCOPE_H

#include <uslscore/USDeviceTime.h>
#include <uslscore/USHashedString.h>
#include <uslscore/USProfileConfig.h>

//================================================================//
// USProfileScope
//================================================================//

#if PROFILE_ENABLED

struct USProfileScope {
public:

	//----------------------------------------------------------------//
						USProfileScope ( USHashedString& name );
						~USProfileScope ();

	//----------------------------------------------------------------//
	USHashedString&		mName;

	//----------------------------------------------------------------//
	// Fix Visual Studio compiler warning
	USProfileScope&		operator = ( const USProfileScope& other ) { UNUSED ( other ); assert ( 0 ); return *this; }
};

#endif

#if PROFILE_ENABLED

#define PROFILE_SCOPE(name) static USHashedString __profile_scope_name(name); USProfileScope __profile_scope(__profile_scope_name);

#else

#define PROFILE_SCOPE(name)

#endif

#endif
